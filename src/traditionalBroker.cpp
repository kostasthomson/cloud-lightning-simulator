/* Copyright 2017 The CloudLightning Simulation Framework Authors. All
Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include <cell.h>
#include <inputs.h>
#include <mpi.h>
#include <netw.h>
#include <omp.h>
#include <power.h>
#include <resource.h>
#include <stat.h>
#include <task.h>
#include <traditionalBroker.h>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <list>
#include <decisionLogEnhanced.h>

using std::cout;
using std::endl;
using std::min;

traditionalBroker::traditionalBroker()
    : baseBroker(),
      cellId(0),
      pollInterval(0.0),
      Ps(nullptr),
      Pis(nullptr),
      Cs(nullptr),
      Paccs(nullptr),
      Piaccs(nullptr),
      Caccs(nullptr),
      availableProcesses(nullptr),
      totalProcesses(nullptr),
      availableMemory(nullptr),
      totalMemory(nullptr),
      availableAccelerators(nullptr),
      totalAccelerators(nullptr),
      availableStorage(nullptr),
      totalStorage(nullptr),
      queue(nullptr)
{
}

traditionalBroker::traditionalBroker(const traditionalBroker &t)
{
    if (t.galloc())
    {
        alloc = 1;
        numberOfTypes = t.getNumberOfTypes();
        pollInterval = t.gpollInterval();
        types = new int[numberOfTypes];
        numberOfResourcesPerType = new int[numberOfTypes];

        for (int i = 0; i < numberOfTypes; i++)
        {
            types[i] = t.getTypes()[i];
            numberOfResourcesPerType[i] = t.getNumberOfResourcesPerType()[i];
        }

        availableProcesses = new double *[numberOfTypes];
        totalProcesses = new double *[numberOfTypes];
        availableMemory = new double *[numberOfTypes];
        totalMemory = new double *[numberOfTypes];
        availableAccelerators = new double *[numberOfTypes];
        totalAccelerators = new double *[numberOfTypes];
        availableStorage = new double *[numberOfTypes];
        totalStorage = new double *[numberOfTypes];
        availableNetwork = t.getAvailableNetwork();
        totalNetwork = t.getTotalNetwork();

        for (int i = 0; i < numberOfTypes; i++)
        {
            availableProcesses[i] = new double[numberOfResourcesPerType[i]];
            totalProcesses[i] = new double[numberOfResourcesPerType[i]];
            availableMemory[i] = new double[numberOfResourcesPerType[i]];
            totalMemory[i] = new double[numberOfResourcesPerType[i]];
            availableAccelerators[i] = new double[numberOfResourcesPerType[i]];
            totalAccelerators[i] = new double[numberOfResourcesPerType[i]];
            availableStorage[i] = new double[numberOfResourcesPerType[i]];
            totalStorage[i] = new double[numberOfResourcesPerType[i]];
        }

        for (int i = 0; i < numberOfTypes; i++)
        {
            for (int j = 0; j < numberOfResourcesPerType[i]; j++)
            {
                availableProcesses[i][j] = t.getAvailableProcesses()[i][j];
                totalProcesses[i][j] = t.getTotalProcesses()[i][j];
                availableMemory[i][j] = t.getAvailableMemory()[i][j];
                totalMemory[i][j] = t.getTotalMemory()[i][j];
                availableAccelerators[i][j] = t.getAvailableAccelerators()[i][j];
                totalAccelerators[i][j] = t.getTotalAccelerators()[i][j];
                availableStorage[i][j] = t.getAvailableStorage()[i][j];
                totalStorage[i][j] = t.getTotalStorage()[i][j];
            }
        }

        queue = new list<task>[1];
        for (auto &it : *t.gqueue())
        {
            queue[0].push_back(it);
        }
    }
}

traditionalBroker &traditionalBroker::operator=(const traditionalBroker &t)
{
    if (this != &t)
    {
        if (alloc)
        {
            alloc = 0;
            pollInterval = 0.0;
            delete[] types;
            delete[] numberOfResourcesPerType;
            types = nullptr;
            numberOfResourcesPerType = nullptr;

            for (int i = 0; i < numberOfTypes; i++)
            {
                delete[] availableProcesses[i];
                delete[] totalProcesses[i];
                delete[] availableMemory[i];
                delete[] totalMemory[i];
                delete[] availableAccelerators[i];
                delete[] totalAccelerators[i];
                delete[] availableStorage[i];
                delete[] totalStorage[i];
            }

            delete[] availableProcesses;
            delete[] totalProcesses;
            delete[] availableMemory;
            delete[] totalMemory;
            delete[] availableAccelerators;
            delete[] totalAccelerators;
            delete[] availableStorage;
            delete[] totalStorage;
            availableProcesses = nullptr;
            totalProcesses = nullptr;
            availableMemory = nullptr;
            totalMemory = nullptr;
            availableAccelerators = nullptr;
            totalAccelerators = nullptr;
            availableStorage = nullptr;
            totalStorage = nullptr;
            availableNetwork = 0.0;
            totalNetwork = 0.0;
            numberOfTypes = 0;
            (*queue).clear();
            delete[] queue;
        }
        alloc = t.galloc();
        if (alloc)
        {
            numberOfTypes = t.getNumberOfTypes();
            pollInterval = t.gpollInterval();
            types = new int[numberOfTypes];
            numberOfResourcesPerType = new int[numberOfTypes];

            for (int i = 0; i < numberOfTypes; i++)
            {
                types[i] = t.getTypes()[i];
                numberOfResourcesPerType[i] = t.getNumberOfResourcesPerType()[i];
            }

            availableProcesses = new double *[numberOfTypes];
            totalProcesses = new double *[numberOfTypes];
            availableMemory = new double *[numberOfTypes];
            totalMemory = new double *[numberOfTypes];
            availableAccelerators = new double *[numberOfTypes];
            totalAccelerators = new double *[numberOfTypes];
            availableStorage = new double *[numberOfTypes];
            totalStorage = new double *[numberOfTypes];
            availableNetwork = t.getAvailableNetwork();
            totalNetwork = t.getTotalNetwork();

            for (int i = 0; i < numberOfTypes; i++)
            {
                availableProcesses[i] = new double[numberOfResourcesPerType[i]];
                totalProcesses[i] = new double[numberOfResourcesPerType[i]];
                availableMemory[i] = new double[numberOfResourcesPerType[i]];
                totalMemory[i] = new double[numberOfResourcesPerType[i]];
                availableAccelerators[i] = new double[numberOfResourcesPerType[i]];
                totalAccelerators[i] = new double[numberOfResourcesPerType[i]];
                availableStorage[i] = new double[numberOfResourcesPerType[i]];
                totalStorage[i] = new double[numberOfResourcesPerType[i]];
            }

            for (int i = 0; i < numberOfTypes; i++)
            {
                for (int j = 0; j < numberOfResourcesPerType[i]; j++)
                {
                    availableProcesses[i][j] = t.getAvailableProcesses()[i][j];
                    totalProcesses[i][j] = t.getTotalProcesses()[i][j];
                    availableMemory[i][j] = t.getAvailableMemory()[i][j];
                    totalMemory[i][j] = t.getTotalMemory()[i][j];
                    availableAccelerators[i][j] = t.getAvailableAccelerators()[i][j];
                    totalAccelerators[i][j] = t.getTotalAccelerators()[i][j];
                    availableStorage[i][j] = t.getAvailableStorage()[i][j];
                    totalStorage[i][j] = t.getTotalStorage()[i][j];
                }
            }
            queue = new list<task>[1];
            for (auto &it : *t.gqueue())
            {
                queue[0].push_back(it);
            }
        }
    }
    return *this;
}

traditionalBroker::~traditionalBroker()
{
    int i;
    if (alloc)
    {
        alloc = 0;
        pollInterval = 0.0;
        delete[] types;
        delete[] numberOfResourcesPerType;
        types = nullptr;
        numberOfResourcesPerType = nullptr;
        for (i = 0; i < numberOfTypes; i++)
        {
            delete[] availableProcesses[i];
            delete[] totalProcesses[i];
            delete[] availableMemory[i];
            delete[] totalMemory[i];
            delete[] availableAccelerators[i];
            delete[] totalAccelerators[i];
            delete[] availableStorage[i];
            delete[] totalStorage[i];
        }
        delete[] availableProcesses;
        delete[] totalProcesses;
        delete[] availableMemory;
        delete[] totalMemory;
        delete[] availableAccelerators;
        delete[] totalAccelerators;
        delete[] availableStorage;

        delete[] totalStorage;
        availableProcesses = nullptr;
        totalProcesses = nullptr;
        availableMemory = nullptr;
        totalMemory = nullptr;
        availableAccelerators = nullptr;
        totalAccelerators = nullptr;
        availableStorage = nullptr;
        totalStorage = nullptr;
        availableNetwork = 0.0;
        totalNetwork = 0.0;
        numberOfTypes = 0;
        (*queue).clear();
        delete[] queue;

        delete[] Ps;
        delete[] Pis;
        delete[] Cs;
        delete[] Paccs;
        delete[] Piaccs;
        delete[] Caccs;
        Ps = nullptr;
        Pis = nullptr;
        Cs = nullptr;
        Paccs = nullptr;
        Piaccs = nullptr;
        Caccs = nullptr;
    }
}

void traditionalBroker::init(const cell *clCell, const siminputs *si)
{
    baseBroker::init(clCell);

    cellId = clCell->gID();
    decisionLogger.init("output/traditional", cellId);

    pollInterval = si->cinp->binp[0].pollIntervalCellM;

    Ps = new double[numberOfTypes];
    Pis = new double[numberOfTypes];
    Cs = new double[numberOfTypes];
    Paccs = new double[numberOfTypes];
    Piaccs = new double[numberOfTypes];
    Caccs = new double[numberOfTypes];

    for (int i = 0; i < numberOfTypes; i++)
    {
        Cs[i] = clCell->getResources()[i][0].getComputeCapability() /
                clCell->getResources()[i][0].getTotalProcessors();
        Caccs[i] = clCell->getResources()[i][0].getAcceleratorComputeCapability();
        double oz = 1.0;
        Ps[i] = clCell->getPowerConsumption()[i].modelCPU(oz) /
                clCell->getResources()[i][0].getTotalProcessors();
        Paccs[i] = clCell->getPowerConsumption()[i].gaccPmax();
        oz = 0.0;
        Pis[i] = clCell->getPowerConsumption()[i].modelCPU(oz) /
                 clCell->getResources()[i][0].getTotalProcessors();
        Piaccs[i] = clCell->getPowerConsumption()[i].gaccPmin();
    }

    availableProcesses = new double *[numberOfTypes];
    totalProcesses = new double *[numberOfTypes];
    availableMemory = new double *[numberOfTypes];
    totalMemory = new double *[numberOfTypes];
    availableAccelerators = new double *[numberOfTypes];
    totalAccelerators = new double *[numberOfTypes];
    availableStorage = new double *[numberOfTypes];
    totalStorage = new double *[numberOfTypes];
    availableNetwork = 0.0;
    totalNetwork = 0.0;

    for (int i = 0; i < numberOfTypes; i++)
    {
        availableProcesses[i] = new double[numberOfResourcesPerType[i]];
        totalProcesses[i] = new double[numberOfResourcesPerType[i]];
        availableMemory[i] = new double[numberOfResourcesPerType[i]];
        totalMemory[i] = new double[numberOfResourcesPerType[i]];
        availableAccelerators[i] = new double[numberOfResourcesPerType[i]];
        totalAccelerators[i] = new double[numberOfResourcesPerType[i]];
        availableStorage[i] = new double[numberOfResourcesPerType[i]];
        totalStorage[i] = new double[numberOfResourcesPerType[i]];
    }
    queue = new list<task>[1];
}

void traditionalBroker::print() const
{
    if (alloc)
    {
        cout << "     Broker Poll Interval for Resources: " << pollInterval << endl;
    }
}

void traditionalBroker::updateStateInfo(const cell *clCell, const double &tstep)
{
    int i, j;
    int omp_thr = atoi(getenv("OMP_NUM_THREADS"));
    if (alloc)
    {
        if (((int)tstep % (int)pollInterval) == 0)
        {
            for (i = 0; i < numberOfTypes; i++)
            {
#pragma omp parallel for default(shared) private(j) num_threads(omp_thr) schedule(static)
                for (j = 0; j < numberOfResourcesPerType[i]; j++)
                {
                    availableProcesses[i][j] = clCell[0].getResources()[i][j].getAvailableProcessors();
                    totalProcesses[i][j] = clCell[0].getResources()[i][j].getTotalProcessors();
                    availableMemory[i][j] = clCell[0].getResources()[i][j].getAvailableMemory();
                    totalMemory[i][j] = clCell[0].getResources()[i][j].getTotalMemory();
                    availableAccelerators[i][j] = clCell[0].getResources()[i][j].getAvailableAccelerators();
                    totalAccelerators[i][j] = clCell[0].getResources()[i][j].getTotalAccelerators();
                    availableStorage[i][j] = clCell[0].getResources()[i][j].getAvailableStorage();
                    totalStorage[i][j] = clCell[0].getResources()[i][j].getTotalStorage();
                }
            }
            availableNetwork = clCell[0].getNetwork()[0].getAvailableNetwork();
            totalNetwork = clCell[0].getNetwork()[0].getTotalNetwork();
        }
    }
}

void traditionalBroker::deploy(resource **resources, netw *network, stat *stats, task &_task)
{
    int type = -1, i, j;
    int *IDs;
    int tID;
    int L_ID = -1;
    double *reqPMNS;
    int avAcc, L_numberOfVMs, L_availImpl;
    reqPMNS = _task.greqPMNS();
    avAcc = _task.gavAcc()[0];
    L_availImpl = _task.getAvailableImplementations()[0];
    L_numberOfVMs = _task.getNumberOfVMs();
    int omp_thr = atoi(getenv("OMP_NUM_THREADS"));

    for (i = 0; i < numberOfTypes; i++)
    {
        if (types[i] == L_availImpl)
        {
            type = i;
            _task.remapType(&i, 1);
            break;
        }
    }

    if (type == -1)
    {
        cout << "Broker::deploy catastrophic error: " << L_availImpl << endl;
        exit(0);
    }

    double avail_cpu = 0.0, total_cpu = 0.0;
    double avail_mem = 0.0, total_mem = 0.0;
    double avail_storage = 0.0, total_storage = 0.0;
    double avail_acc = 0.0, total_acc = 0.0;
    int running_vms = 0;
    double overcommit_cpu = 0.0, overcommit_mem = 0.0;
    for (j = 0; j < numberOfResourcesPerType[type]; j++)
    {
        avail_cpu += availableProcesses[type][j];
        total_cpu += totalProcesses[type][j];
        avail_mem += availableMemory[type][j];
        total_mem += totalMemory[type][j];
        avail_storage += availableStorage[type][j];
        total_storage += totalStorage[type][j];
        avail_acc += availableAccelerators[type][j];
        total_acc += totalAccelerators[type][j];
        running_vms += resources[type][j].getRunningVMs();
    }

    double util_cpu = (total_cpu > 0) ? 1.0 - (avail_cpu / total_cpu) : 0.0;
    double util_mem = (total_mem > 0) ? 1.0 - (avail_mem / total_mem) : 0.0;
    double util_network = (totalNetwork > 0) ? 1.0 - (availableNetwork / totalNetwork) : 0.0;
    int num_tasks = (int)queue->size();

    double rho_acc = 0.0;
    if (_task.getNumberOfAvailableImplementations() > 0)
    {
        int accIdx = 0;
        for (int k = 0; k < _task.getNumberOfAvailableImplementations(); k++)
        {
            if (_task.getAvailableImplementations()[k] == types[type])
            {
                accIdx = k;
                break;
            }
        }
        rho_acc = _task.grhoAcc()[accIdx];
    }

    L_ID = network[0].probe(reqPMNS[2]);
    if (L_ID == -1)
    {
        decisionLogger.log(L_numberOfVMs, reqPMNS[0], reqPMNS[1], reqPMNS[3], reqPMNS[2],
                           avAcc, rho_acc, _task.grequestedInstructions(),
                           -1, false,
                           util_cpu, util_mem, avail_cpu, avail_mem, avail_storage, avail_acc,
                           total_cpu, total_mem, total_storage, total_acc,
                           availableNetwork, totalNetwork, util_network,
                           Pis[type], Ps[type], Piaccs[type], Paccs[type],
                           Cs[type], Caccs[type],
                           running_vms, overcommit_cpu, overcommit_mem, num_tasks, 0.0);
        stats[type].rejectedTasks++;
        return;
    }

    availableNetwork -= reqPMNS[2];
    IDs = new int[L_numberOfVMs];
    for (j = 0; j < L_numberOfVMs; j++)
        IDs[j] = -1;

    for (j = 0; j < L_numberOfVMs; j++)
    {
        L_ID = -1;
        tID = -1;
#pragma omp parallel default(shared) private(i, tID) num_threads(omp_thr)
        {
            tID = -1;
            i = omp_get_thread_num();
            while (i < numberOfResourcesPerType[type] && L_ID == -1)
            {
                if (availableProcesses[type][i] >= reqPMNS[0] &&
                    availableMemory[type][i] >= reqPMNS[1] &&
                    availableStorage[type][i] >= reqPMNS[3] &&
                    availableAccelerators[type][i] >= avAcc)
                {
                    tID = resources[type][i].probe(reqPMNS[0], reqPMNS[1], reqPMNS[3], avAcc);
                    if (tID == i)
                    {
#pragma omp single nowait
                        {
                            if (L_ID == -1)
                            {
                                L_ID = i;
#pragma omp flush(L_ID)
                                IDs[j] = i;
                            }
                        }
                    }
                }
                i += omp_thr;
            }
        }

        if (L_ID == -1)
        {
            break;
        }
        else
        {
            availableProcesses[type][L_ID] -= reqPMNS[0];
            availableMemory[type][L_ID] -= reqPMNS[1];
            availableStorage[type][L_ID] -= reqPMNS[3];
            availableAccelerators[type][L_ID] -= avAcc;
        }
    }

    if (L_ID == -1)
    {
        for (j = 0; j < L_numberOfVMs; j++)
        {
            if (IDs[j] == -1)
            {
                break;
            }
            availableProcesses[type][IDs[j]] += reqPMNS[0];
            availableMemory[type][IDs[j]] += reqPMNS[1];
            availableStorage[type][IDs[j]] += reqPMNS[3];
            availableAccelerators[type][IDs[j]] += avAcc;
        }
        availableNetwork += reqPMNS[2];
        decisionLogger.log(L_numberOfVMs, reqPMNS[0], reqPMNS[1], reqPMNS[3], reqPMNS[2],
                           avAcc, rho_acc, _task.grequestedInstructions(),
                           -1, false,
                           util_cpu, util_mem, avail_cpu, avail_mem, avail_storage, avail_acc,
                           total_cpu, total_mem, total_storage, total_acc,
                           availableNetwork, totalNetwork, util_network,
                           Pis[type], Ps[type], Piaccs[type], Paccs[type],
                           Cs[type], Caccs[type],
                           running_vms, overcommit_cpu, overcommit_mem, num_tasks, 0.0);
        stats[type].rejectedTasks++;
    }
    else
    {
        for (j = 0; j < L_numberOfVMs; j++)
        {
            resources[type][IDs[j]].deploy(_task);
        }

        network[0].deploy(_task);
        _task.attachResources(IDs);
        enque(_task);
        stats[type].acceptedTasks++;

        double estimatedDuration = 10.0;
        double energy_kwh = computeEstimatedEnergy(reqPMNS[0], L_numberOfVMs, estimatedDuration, Ps, Pis, type);
        decisionLogger.log(L_numberOfVMs, reqPMNS[0], reqPMNS[1], reqPMNS[3], reqPMNS[2],
                           avAcc, rho_acc, _task.grequestedInstructions(),
                           types[type], true,
                           util_cpu, util_mem, avail_cpu, avail_mem, avail_storage, avail_acc,
                           total_cpu, total_mem, total_storage, total_acc,
                           availableNetwork, totalNetwork, util_network,
                           Pis[type], Ps[type], Piaccs[type], Paccs[type],
                           Cs[type], Caccs[type],
                           running_vms, overcommit_cpu, overcommit_mem, num_tasks, energy_kwh);
    }

    delete[] IDs;
}

void traditionalBroker::enque(const task &_task)
{
    if (alloc)
    {
        queue->push_back(_task);
    }
}

void traditionalBroker::timestep(const cell *clCell)
{
    int i, j, rID, type = -1;
    double insR, insRa;
    double procUtil;
    double rhoAcc, L_totalPowerConsumption;
    int active, L_numberOfVMs;
    int totalAcc, len;
    int omp_thr = atoi(getenv("OMP_NUM_THREADS"));
    list<task>::iterator it;
    len = (int)(*queue).size();
    int chunk = 50;
    double L_net = 0.0;
    if (alloc)
    {
        for (i = 0; i < numberOfTypes; i++)
        {
#pragma omp parallel for default(shared) private(j) num_threads(omp_thr) schedule(static, chunk)
            for (j = 0; j < numberOfResourcesPerType[i]; j++)
            {
                if (clCell->getResources()[i][j].getRunningVMs() > 0)
                    clCell->getResources()[i][j].initializeRunningQuantities();
            }
        }

        clCell->getNetwork()[0].initializeRunningQuantities();

        L_net = 0.0;
        //	#pragma omp parallel for default(shared) private(i,it,type) num_threads(omp_thr) schedule(static,chunk)
        // reduction(+:L_net)
        for (it = queue->begin(); it != queue->end(); it++)
        {
            type = (it->getAvailableImplementations())[0];

            it->compcUtilPMNr();
            L_net += it->gcUtilPMNr()[2];

            for (j = 0; j < it->getNumberOfVMs(); j++)
            {
                rID = it->gresourceIDs()[j];
                clCell->getResources()[type][rID].incrementRunningQuantities(it->gcUtilPMNr()[0], it->gcUtilPMNr()[1],
                                                                             it->gcUtilPMNr()[3]);
            }
            // clCell->getNetwork()[0].incrementRunningQuantities(it->gcUtilPMNr()[2]);
        }
        clCell->getNetwork()[0].incrementRunningQuantities(L_net);

        for (i = 0; i < numberOfTypes; i++)
        {
#pragma omp parallel for default(shared) private(j) num_threads(omp_thr) schedule(static, chunk)
            for (j = 0; j < numberOfResourcesPerType[i]; j++)
            {
                if (clCell->getResources()[i][j].getRunningVMs() > 0)
                {
                    clCell->getResources()[i][j].compcurrentCompCapPerProc();
                    clCell->getResources()[i][j].compcurrentCompCapPerAcc();
                }
            }
        }

        for (i = 0; i < numberOfTypes; i++)
        {
            L_totalPowerConsumption = 0.0;
#pragma omp parallel for default(shared) private(j, procUtil, rhoAcc, active, totalAcc) num_threads(omp_thr) \
    schedule(static) reduction(+ : L_totalPowerConsumption)
            for (j = 0; j < numberOfResourcesPerType[i]; j++)
            {
                procUtil = clCell->getResources()[i][j].getActualUtilizedProcessors() /
                           clCell->getResources()[i][j].getTotalProcessors();
                rhoAcc = clCell->getResources()[i][j].getActualRhoAccelerators();
                active = clCell->getResources()[i][j].getActive();
                totalAcc = clCell->getResources()[i][j].getTotalAccelerators();
                L_totalPowerConsumption += clCell->getPowerConsumption()[i].consumption(procUtil, rhoAcc, active, totalAcc);
            }
            clCell->getStats()[i].totalPowerConsumption += L_totalPowerConsumption;
        }

#pragma omp parallel default(shared) private(i, it, j, rID, insR, insRa, type, L_numberOfVMs) num_threads(omp_thr)
        {
            int tid = omp_get_thread_num();
            it = queue->begin();
            for (i = 0; i < tid; i++)
                it++;
            i = tid;
            double ocP = clCell->getResources()[type][0].getOvercommitmentProcessors();
            while (i < len)
            {
                type = (it->getAvailableImplementations())[0];

                rID = (it->gresourceIDs())[0];
                insR = clCell->getResources()[type][rID].getCurrentCompCapPerProc();
                insRa = clCell->getResources()[type][rID].getCurrentCompCapPerAcc();
                L_numberOfVMs = it->getNumberOfVMs();
                double L_vCPU = it->greqPMNS()[0];
                for (j = 1; j < L_numberOfVMs; j++)
                {
                    rID = (it->gresourceIDs())[j];
                    insR = min(insR, clCell->getResources()[type][rID].getCurrentCompCapPerProc());
                    insRa = min(insRa, clCell->getResources()[type][rID].getCurrentCompCapPerAcc());
                }
                it->reduceIns(L_numberOfVMs * insR * min(it->gcUtilPMNr()[0] / L_vCPU * ocP, 1.0) * L_vCPU +
                              L_numberOfVMs * insRa * ((it->gcUtilPMNr())[3]));
                for (j = 0; j < omp_thr; j++)
                {
                    i++;
                    it++;
                }
            }
        }
        it = queue->begin();
        while (it != queue->end())
        {
            if ((it->grequestedInstructions()) <= 0.0)
            {
                type = (it->getAvailableImplementations())[0];
                L_numberOfVMs = it->getNumberOfVMs();
                for (j = 0; j < L_numberOfVMs; j++)
                {
                    rID = (it->gresourceIDs())[j];
                    clCell->getResources()[type][rID].unload(it);
                }

                clCell->getNetwork()[0].unload(it);

                it = queue->erase(it);
            }
            else
                ++it;
        }
    }
}

double traditionalBroker::gpollInterval() const { return pollInterval; }
double **traditionalBroker::getAvailableProcesses() const { return availableProcesses; }
double **traditionalBroker::getTotalProcesses() const { return totalProcesses; }
double **traditionalBroker::getAvailableMemory() const { return availableMemory; }
double **traditionalBroker::getTotalMemory() const { return totalMemory; }
double **traditionalBroker::getAvailableAccelerators() const { return availableAccelerators; }
double **traditionalBroker::getTotalAccelerators() const { return totalAccelerators; }
double **traditionalBroker::getAvailableStorage() const { return availableStorage; }
double **traditionalBroker::getTotalStorage() const { return totalStorage; }
list<task> *traditionalBroker::gqueue() const { return queue; }
