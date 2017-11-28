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

#include <omp.h>
#include <improvedpSwitch.h>
#include <resource.h>
#include <stat.h> // for stat
#include <task.h> // for task
#include <improvedvRM.h>
#include <cstdlib> // for atoi, getenv, rand, RAND_MAX

class netw;
class resource;

improvedpSwitch::improvedpSwitch()
  : alloc(0),
    numberOfvRMs(0),
    numberOfFunctions(0),
    pollIntervalpSwitch(0.0),
    vRMs(nullptr),
    Fs(nullptr),
    Ws(nullptr),
    availableProcessors(nullptr),
    totalProcessors(nullptr),
    availableMemory(nullptr),
    totalMemory(nullptr),
    availableAccelerators(nullptr),
    totalAccelerators(nullptr),
    availableStorage(nullptr),
    totalStorage(nullptr),
    sPMSA(nullptr),
    SIs(nullptr),
    SI(0.0),
    C(0.0),
    P(0.0),
    Pi(0.0),
    Cacc(0.0),
    Pacc(0.0),
    Piacc(0.0)
{
}

improvedpSwitch::improvedpSwitch(const int& start, const int& end, const int& type, list<improvedvRM>** LvRMs,
                 const double& L_pollIntervalpSwitch, const double& L_C, const double& L_Cacc, const double& L_P, const double& L_Pacc, const double& L_Pi,
                 const double& L_Piacc, const int& L_numberOfFunctions, const double* L_Ws)
{
  alloc = 1;
  numberOfvRMs = end - start;
  numberOfFunctions = L_numberOfFunctions;
  pollIntervalpSwitch = L_pollIntervalpSwitch;
  list<improvedvRM>::iterator it = LvRMs[type]->begin();
  vRMs = new list<improvedvRM*>[1];

  for (int i = 0; i < start; i++) {
    it++;
  }

  for (int i = start; i < end; i++) {
    vRMs->push_back(&(*it));
    it++;
  }

  Ws = new double[numberOfFunctions];
  Fs = new double[numberOfFunctions];

  for (int i = 0; i < numberOfFunctions; i++) {
    Ws[i] = L_Ws[i];
    Fs[i] = 0.0;
  }

  availableProcessors = new double[numberOfvRMs];
  totalProcessors = new double[numberOfvRMs];
  availableMemory = new double[numberOfvRMs];
  totalMemory = new double[numberOfvRMs];
  availableAccelerators = new double[numberOfvRMs];
  totalAccelerators = new double[numberOfvRMs];
  availableStorage = new double[numberOfvRMs];
  totalStorage = new double[numberOfvRMs];
  SIs = new double[numberOfvRMs];

  for (int i = 0; i < numberOfvRMs; i++) {
    availableProcessors[i] = 0.0;
    totalProcessors[i] = 0.0;
    availableMemory[i] = 0.0;
    totalMemory[i] = 0.0;
    availableAccelerators[i] = 0.0;
    totalAccelerators[i] = 0.0;
    availableStorage[i] = 0.0;
    totalStorage[i] = 0.0;
    SIs[i] = 0.0;
  }

  SI = 0.0;
  sPMSA = new double[8];

  for (int i = 0; i < 8; i++) {
    sPMSA[i] = 0.0;
  }

  C = L_C;
  P = L_P;
  Pi = L_Pi;
  Cacc = L_Cacc;
  Pacc = L_Pacc;
  Piacc = L_Piacc;
  updateStateInfo(0.0);
}

improvedpSwitch::improvedpSwitch(const improvedpSwitch& t)
{
  if (t.galloc()) {
    alloc = 1;
    numberOfvRMs = t.getNumberOfvRMs();
    numberOfFunctions = t.getNumberOfFunctions();
    pollIntervalpSwitch = t.gpollIntervalpSwitch();
    vRMs = new list<improvedvRM*>[1];
    for (list<improvedvRM*>::iterator it = t.getvRMs()->begin(); it != t.getvRMs()->end(); it++) {
      vRMs->push_back(*it);
    }
    Ws = new double[numberOfFunctions];
    Fs = new double[numberOfFunctions];

    for (int i = 0; i < numberOfFunctions; i++) {
      Ws[i] = t.gWs()[i];
      Fs[i] = t.gFs()[i];
    }

    availableProcessors = new double[numberOfvRMs];
    totalProcessors = new double[numberOfvRMs];
    availableMemory = new double[numberOfvRMs];
    totalMemory = new double[numberOfvRMs];
    availableAccelerators = new double[numberOfvRMs];
    totalAccelerators = new double[numberOfvRMs];
    availableStorage = new double[numberOfvRMs];
    totalStorage = new double[numberOfvRMs];
    SIs = new double[numberOfvRMs];

    for (int i = 0; i < numberOfvRMs; i++) {
      availableProcessors[i] = t.getAvailableProcessors()[i];
      totalProcessors[i] = t.getTotalProcessors()[i];
      availableMemory[i] = t.getAvailableMemory()[i];
      totalMemory[i] = t.getTotalMemory()[i];
      availableStorage[i] = t.getAvailableStorage()[i];
      totalStorage[i] = t.getTotalStorage()[i];
      availableAccelerators[i] = t.getAvailableAccelerators()[i];
      totalAccelerators[i] = t.getTotalAccelerators()[i];
      SIs[i] = t.gSIs()[i];
    }

    SI = t.gSI();
    sPMSA = new double[8];

    for (int i = 0; i < 8; i++) {
      sPMSA[i] = t.gsPMSA()[i];
    }

    C = t.gC();
    P = t.gP();
    Pi = t.gPi();
    Cacc = t.gCacc();
    Pacc = t.gPacc();
    Piacc = t.gPiacc();
  }
}

improvedpSwitch& improvedpSwitch::operator=(const improvedpSwitch& t)
{
  if (this != &t) {
    if (alloc) {
      alloc = 0;
      numberOfvRMs = 0;
      numberOfFunctions = 0;
      pollIntervalpSwitch = 0.0;
      vRMs->clear();
      delete[] vRMs;
      vRMs = nullptr;
      delete[] Ws;
      delete[] Fs;
      Ws = nullptr;
      Fs = nullptr;
      delete[] availableProcessors;
      delete[] totalProcessors;
      delete[] availableMemory;
      delete[] totalMemory;
      delete[] availableStorage;
      delete[] totalStorage;
      delete[] availableAccelerators;
      delete[] totalAccelerators;
      delete[] sPMSA;
      delete[] SIs;
      availableProcessors = nullptr;
      totalProcessors = nullptr;
      availableMemory = nullptr;
      totalMemory = nullptr;
      availableAccelerators = nullptr;
      totalAccelerators = nullptr;
      availableStorage = nullptr;
      totalStorage = nullptr;
      sPMSA = nullptr;
      SIs = nullptr;
      SI = 0.0;
      C = 0.0;
      P = 0.0;
      Pi = 0.0;
      Cacc = 0.0;
      Pacc = 0.0;
      Piacc = 0.0;
    }
    alloc = t.galloc();
    if (alloc) {
      numberOfvRMs = t.getNumberOfvRMs();
      numberOfFunctions = t.getNumberOfFunctions();
      pollIntervalpSwitch = t.gpollIntervalpSwitch();
      vRMs = new list<improvedvRM*>[1];
      for (list<improvedvRM*>::iterator it = t.getvRMs()->begin(); it != t.getvRMs()->end(); it++) {
        vRMs->push_back(*it);
      }
      Ws = new double[numberOfFunctions];
      Fs = new double[numberOfFunctions];

      for (int i = 0; i < numberOfFunctions; i++) {
        Ws[i] = t.gWs()[i];
        Fs[i] = t.gFs()[i];
      }

      availableProcessors = new double[numberOfvRMs];
      totalProcessors = new double[numberOfvRMs];
      availableMemory = new double[numberOfvRMs];
      totalMemory = new double[numberOfvRMs];
      availableAccelerators = new double[numberOfvRMs];
      totalAccelerators = new double[numberOfvRMs];
      availableStorage = new double[numberOfvRMs];
      totalStorage = new double[numberOfvRMs];
      SIs = new double[numberOfvRMs];

      for (int i = 0; i < numberOfvRMs; i++) {
        availableProcessors[i] = t.getAvailableProcessors()[i];
        totalProcessors[i] = t.getTotalProcessors()[i];
        availableMemory[i] = t.getAvailableMemory()[i];
        totalMemory[i] = t.getTotalMemory()[i];
        availableStorage[i] = t.getAvailableStorage()[i];
        totalStorage[i] = t.getTotalStorage()[i];
        availableAccelerators[i] = t.getAvailableAccelerators()[i];
        totalAccelerators[i] = t.getTotalAccelerators()[i];
        SIs[i] = t.gSIs()[i];
      }

      SI = t.gSI();
      sPMSA = new double[8];

      for (int i = 0; i < 8; i++) {
        sPMSA[i] = t.gsPMSA()[i];
      }

      C = t.gC();
      P = t.gP();
      Pi = t.gPi();
      Cacc = t.gCacc();
      Pacc = t.gPacc();
      Piacc = t.gPiacc();
    }
  }
  return *this;
}

improvedpSwitch::~improvedpSwitch()
{
  if (alloc) {
    alloc = 0;
    numberOfvRMs = 0;
    numberOfFunctions = 0;
    pollIntervalpSwitch = 0.0;
    vRMs->clear();
    delete[] vRMs;
    delete[] Fs;
    delete[] Ws;
    vRMs = nullptr;
    Fs = nullptr;
    Ws = nullptr;
    delete[] availableProcessors;
    delete[] totalProcessors;
    delete[] availableMemory;
    delete[] totalMemory;
    delete[] availableStorage;
    delete[] totalStorage;
    delete[] availableAccelerators;
    delete[] totalAccelerators;
    delete[] sPMSA;
    delete[] SIs;
    availableProcessors = nullptr;
    totalProcessors = nullptr;
    availableMemory = nullptr;
    totalMemory = nullptr;
    availableAccelerators = nullptr;
    totalAccelerators = nullptr;
    availableStorage = nullptr;
    totalStorage = nullptr;
    sPMSA = nullptr;
    SIs = nullptr;
    SI = 0.0;
    P = 0.0;
    C = 0.0;
    Pi = 0.0;
    Cacc = 0.0;
    Pacc = 0.0;
    Piacc = 0.0;
  }
}

void improvedpSwitch::computeFs()
{
  if (alloc) {
    for (int i = 0; i < numberOfFunctions; i++) {
      Fs[i] = 0.0;
    }
    list<improvedvRM*>::iterator it = vRMs->begin();
    for (int i = 0; i < numberOfvRMs; i++) {
      for (int j = 0; j < numberOfFunctions; j++) {
        Fs[j] += (*it)->gFs()[j];
      }
      it++;
    }
    for (int j = 0; j < numberOfFunctions; j++) {
      Fs[j] /= numberOfvRMs;
    }
  }
}

void improvedpSwitch::computeSI()
{
  if (alloc) {
    SI = (1e-4) * (((double)rand()) / RAND_MAX);
    for (int i = 0; i < numberOfFunctions; i++) {
      SI += Ws[i] * Fs[i];
    }
  }
}

void improvedpSwitch::updateStateInfo(const double& tstep)
{
  int i;
  if (alloc) {
    // On every time interval
    if (((int)tstep % (int)pollIntervalpSwitch) == 0) {
      for (i = 0; i < 8; i++) {
        sPMSA[i] = 0.0;
      }
      i = 0;
      for (list<improvedvRM*>::iterator it = vRMs->begin(); it != vRMs->end(); it++) {
        availableProcessors[i] = ((*it)->gsPMSA())[0];
        totalProcessors[i] = ((*it)->gsPMSA())[1];
        availableMemory[i] = ((*it)->gsPMSA())[2];
        totalMemory[i] = ((*it)->gsPMSA())[3];
        availableStorage[i] = ((*it)->gsPMSA())[4];
        totalStorage[i] = ((*it)->gsPMSA())[5];
        availableAccelerators[i] = ((*it)->gsPMSA())[6];
        totalAccelerators[i] = ((*it)->gsPMSA())[7];
        SIs[i] = ((*it)->gSI());
        i++;
      }

      for (i = 0; i < (int)vRMs->size(); i++) {
        sPMSA[0] += availableProcessors[i];
        sPMSA[1] += totalProcessors[i];
        sPMSA[2] += availableMemory[i];
        sPMSA[3] += totalMemory[i];
        sPMSA[4] += availableStorage[i];
        sPMSA[5] += totalStorage[i];
        sPMSA[6] += availableAccelerators[i];
        sPMSA[7] += totalAccelerators[i];
      }
      computeFs();
      computeSI();
    }
  }
}

double improvedpSwitch::deassessmentFunctions(const double& dNu,const double& availNu, const double& totNu, const double& dAcc,
                                      const double& availableAccelerators, const double& totalAccelerators, const int& choice)
{
  switch (choice) {
    case 0:
      {
        double Ln=(Pi*totNu+(P-Pi)*(totNu-availNu)+Piacc*availableAccelerators+Pacc*(totalAccelerators-availableAccelerators));
        double Kn=C*totNu+Cacc*totalAccelerators;
        return (Kn*(dNu*(P-Pi)+dAcc*Pacc))/(Ln*Ln);
      }
      break;
    default:
      return 0.0;
  }
}

int improvedpSwitch::probe(const double& Proc, const double& Mem, const double& Sto, const int& Acc)
{
  if (Proc <= sPMSA[0] && Mem <= sPMSA[2] && Sto <= sPMSA[4] && Acc <= (int)sPMSA[6]) {
    return 1;
  } else {
    return -1;
  }
}

void improvedpSwitch::deploy(resource** resources, netw* network, stat* stats, task& task_)
{
  if (alloc) {
    int L_numberOfVMs = task_.getNumberOfVMs();
    double* L_reqPMNS = task_.greqPMNS();
    double maxSI = 0.0, ssum = 0.0, *maxSIs;
    int L_avAcc = task_.gavAcc()[0];
    int omp_thr = atoi(getenv("OMP_NUM_THREADS"));
    int i, tid, j;
    int choice = -1, *choices;
    list<improvedvRM *>::iterator itt = vRMs->begin(), itf, itff;
    choices = new int[omp_thr];
    maxSIs = new double[omp_thr];

    // Initialize the choices and maxSIs arrays
    for (i = 0; i < omp_thr; i++) {
      choices[i] = -1;
      maxSIs[i] = 0.0;
    }

    // The total requested units = number of task VMs * task units
    double reqProc = L_numberOfVMs * L_reqPMNS[0];
    double reqMem = L_numberOfVMs * L_reqPMNS[1];
    double reqSto = L_numberOfVMs * L_reqPMNS[3];
    int reqAcc = L_numberOfVMs * L_avAcc;

// Partition the list of vRMs per thread. Then, for every vRM, determine their maximum SI. Set the
// index number of the vRM with the maximum SI in the array choices[thread_number]
#pragma omp parallel default(shared) private(i, tid) num_threads(omp_thr)
    {
      tid = omp_get_thread_num();
      for (i = (tid * numberOfvRMs) / omp_thr; i < ((tid + 1) * numberOfvRMs) / omp_thr; i++) {
        if (maxSIs[tid] < SIs[i] && reqProc <= availableProcessors[i] && reqMem <= availableMemory[i] &&
            reqSto <= availableStorage[i] && reqAcc <= availableAccelerators[i]) {
          maxSIs[tid] = SIs[i];
          choices[tid] = i;
        }
      }
    }

    choice = choices[0];
    maxSI = maxSIs[0];

    // From the list of vRMs with the maximum SI per thread, selected the highest for the task deployment
    for (i = 1; i < omp_thr; i++) {
      if (maxSI < maxSIs[i]) {
        maxSI = maxSIs[i];
        choice = choices[i];
      }
    }

    // If no suitable vRM was found, begin Self-Organization and reorganize resources
    // First step, locate the vRM with the highest suitability index (lets call this chosenvRM)
    // Then, move resources from vRMs 0 - (chosenvRM - 1) to chosenvRM
    // If there are still additional resources needed to satisfy the task, move them from vRMs
    // (chosenvRM + 1) - numberOfvRMs to chosenvRM
    // If there are still additional resources needed, reject the task
    if (choice == -1) {
      for (i = 0; i < omp_thr; i++) {
        choices[i] = -1;
        maxSIs[i] = 0.0;
      }
// First, find a suitable vRM even if it does not satisfy all the task's requested units
#pragma omp parallel default(shared) private(i, tid) num_threads(omp_thr)
      {
        tid = omp_get_thread_num();
        for (i = (tid * numberOfvRMs) / omp_thr; i < ((tid + 1) * numberOfvRMs) / omp_thr; i++) {
          if (maxSIs[tid] < SIs[i]) {
            maxSIs[tid] = SIs[i];
            choices[tid] = i;
          }
        }
      }
      choice = choices[0];
      maxSI = maxSIs[0];
      for (i = 1; i < omp_thr; i++) {
        if (maxSI < maxSIs[i]) {
          maxSI = maxSIs[i];
          choice = choices[i];
        }
      }

      // Reduce the task's units from the list of available units
      double remProc = reqProc - availableProcessors[choice];
      double remMem = reqMem - availableMemory[choice];
      double remSto = reqSto - availableStorage[choice];
      double remAcc = (double)reqAcc - availableAccelerators[choice];

      // The pores list contains the resources of all vRMs needed to satisfy the task's requirments
      list<resource*> ores;

      // The ores list contains the resources of the current vRM
      list<resource*> pores;

      itt = vRMs->begin();
      for (i = 0; i < choice; i++) {
        // Obtain the list of resources belonging to every vRM, until all the task's requested resources are satisfied
        (*itt)->obtainresources(pores, remProc, remMem, remSto, remAcc);

        // If the vRM has at least one resource, recalculate its suitability index
        if (pores.size() > 0) {
          ssum = 0.0;
          for (j = 0; j < 1; j++) {
            ssum += Ws[j] * deassessmentFunctions(
                                -((double)pores.size()) * ((*pores.begin())->getTotalProcessors()),
                                availableProcessors[i], totalAccelerators[i],
                                -((double)pores.size()) * ((double)(*pores.begin())->getTotalAccelerators()), (double)availableAccelerators[i], (double)totalAccelerators[i], j);
          }

          totalProcessors[i] -= ((double)pores.size()) * ((*pores.begin())->getTotalProcessors());
          totalMemory[i] -= ((double)pores.size()) * ((*pores.begin())->getTotalMemory());
          totalStorage[i] -= ((double)pores.size()) * ((*pores.begin())->getTotalStorage());
          totalAccelerators[i] -= ((double)pores.size()) * ((double)(*pores.begin())->getTotalAccelerators());
          availableProcessors[i] -= ((double)pores.size()) * ((*pores.begin())->getTotalProcessors());
          availableMemory[i] -= ((double)pores.size()) * ((*pores.begin())->getTotalMemory());
          availableStorage[i] -= ((double)pores.size()) * ((*pores.begin())->getTotalStorage());
          availableAccelerators[i] -= ((double)pores.size()) * ((double)(*pores.begin())->getTotalAccelerators());
          SIs[i] += ssum;

          // At the end of the ores list, add the pores list
          ores.splice(ores.end(), pores);
        }
        // If all remaining units are below 0, break the vRM loop
        if (remProc <= 0.0 && remMem <= 0.0 && remSto <= 0.0 && remAcc <= 0.0) {
          break;
        }
        // Move to the next vRM
        itt++;
      }
      // Save the last vRM needed to satisfy the task's unit requirements to itff
      itff = itt;

      // Go to the next vRM
      itt++;

      // If there are still some of the task's requirements unsatisfied but we are already at the vRM with the
      // highest suitability index
      if (!(remProc <= 0.0 && remMem <= 0.0 && remSto <= 0.0 && remAcc <= 0.0)) {
        // Iterate the list of the remaining vRMs until the last
        for (i = choice + 1; i < numberOfvRMs; i++) {
          // Obtain the list of resources belonging to every vRM, until all the task's requested resources are satisfied
          (*itt)->obtainresources(pores, remProc, remMem, remSto, remAcc);
          if (pores.size() > 0) {
            ssum = 0.0;

              for (j = 0; j < 1; j++) {
                ssum += Ws[j] * deassessmentFunctions(
                                  -((double)pores.size()) * ((*pores.begin())->getTotalProcessors()),
                                  availableProcessors[i], totalProcessors[i],
                                  -((double)pores.size()) * ((double)(*pores.begin())->getTotalAccelerators()), (double)availableAccelerators[i], (double)totalAccelerators[i], j);
              }

            totalProcessors[i] -= ((double)pores.size()) * ((*pores.begin())->getTotalProcessors());
            totalMemory[i] -= ((double)pores.size()) * ((*pores.begin())->getTotalMemory());
            totalStorage[i] -= ((double)pores.size()) * ((*pores.begin())->getTotalStorage());
            totalAccelerators[i] -= ((double)pores.size()) * ((double)(*pores.begin())->getTotalAccelerators());
            availableProcessors[i] -= ((double)pores.size()) * ((*pores.begin())->getTotalProcessors());
            availableMemory[i] -= ((double)pores.size()) * ((*pores.begin())->getTotalMemory());
            availableStorage[i] -= ((double)pores.size()) * ((*pores.begin())->getTotalStorage());
            availableAccelerators[i] -= ((double)pores.size()) * ((double)(*pores.begin())->getTotalAccelerators());
            SIs[i] += ssum;
            // At the end of the ores list, add the pores list
            ores.splice(ores.end(), pores);
          }
          if (remProc <= 0.0 && remMem <= 0.0 && remSto <= 0.0 && remAcc <= 0.0) {
            break;
          }

          itt++;
        }
      }
      // If there was a number of resources obtained that satisfy the task's unit requirements
      if (ores.size() > 0) {
        // Add these resources to the last vRM needed to satisfy the task's unit requirements
        (*itff)->attachresources(ores);

        // And then recalculate that vRMs assessment functions and suitability index
        ssum = 0.0;
        for(j=0;j<1;j++){
          ssum+=Ws[j] * deassessmentFunctions(((double)ores.size())*((*ores.begin())->getTotalProcessors()), availableProcessors[choice], totalProcessors[choice],
                                  ((double)ores.size())*((double)(*ores.begin())->getTotalAccelerators()), (double)availableAccelerators[choice], (double)totalAccelerators[choice], j);
        }
        SIs[choice] += ssum;

        totalProcessors[choice] -= ((double)ores.size()) * ((*ores.begin())->getTotalProcessors());
        totalMemory[choice] -= ((double)ores.size()) * ((*ores.begin())->getTotalMemory());
        totalStorage[choice] -= ((double)ores.size()) * ((*ores.begin())->getTotalStorage());
        totalAccelerators[choice] -= ((double)ores.size()) * ((double)(*ores.begin())->getTotalAccelerators());
        availableProcessors[choice] -= ((double)ores.size()) * ((*ores.begin())->getTotalProcessors());
        availableMemory[choice] -= ((double)ores.size()) * ((*ores.begin())->getTotalMemory());
        availableStorage[choice] -= ((double)ores.size()) * ((*ores.begin())->getTotalStorage());
        availableAccelerators[choice] -= ((double)ores.size()) * ((double)(*ores.begin())->getTotalAccelerators());

        ores.clear();
      } else {
        choice = -1;
      }
    }
    delete[] choices;
    delete[] maxSIs;

    // If after SOSM there is still no suitable vRM found, reject the task
    if (choice == -1) {
      stats[task_.getAvailableImplementations()[0]].rejectedTasks++;
      return;
    }

    // Compute again the assessment functions and suitability index of the modified vRM
    itf = vRMs->begin();
    for (i = 0; i < choice; i++) {
      itf++;
    }
    ssum = 0.0;
    for(i = 0; i < 1; i++)
    {
      ssum += Ws[i] * deassessmentFunctions(-reqProc, sPMSA[0], sPMSA[1], -reqAcc, sPMSA[6], sPMSA[7], i);
    }
    SI += ssum;
    ssum = 0.0;
    for(i = 0; i < 1; i++)
    {
      ssum += Ws[i] * deassessmentFunctions(-reqProc, availableProcessors[choice], totalProcessors[choice],
                                  -(double)reqAcc, (double)availableAccelerators[choice], (double)totalAccelerators[choice], i);
    }
    SIs[choice] += ssum;

    availableProcessors[choice] -= reqProc;
    availableMemory[choice] -= reqMem;
    availableStorage[choice] -= reqSto;
    availableAccelerators[choice] -= reqAcc;
    sPMSA[0] -= reqProc;
    sPMSA[2] -= reqMem;
    sPMSA[4] -= reqSto;
    sPMSA[6] -= (double)reqAcc;

    // Recursively call the vRM::deploy method of the selected vRM
    (*itf)->deploy(resources, network, stats, task_);
  }
}

int improvedpSwitch::galloc() const { return alloc; }
int improvedpSwitch::getNumberOfvRMs() const { return numberOfvRMs; }
int improvedpSwitch::getNumberOfFunctions() const { return numberOfFunctions; }
double improvedpSwitch::gpollIntervalpSwitch() const { return pollIntervalpSwitch; }
double* improvedpSwitch::gFs() const { return Fs; }
double* improvedpSwitch::gWs() const { return Ws; }
list<improvedvRM*>* improvedpSwitch::getvRMs() const { return vRMs; }
double* improvedpSwitch::getAvailableProcessors() const { return availableProcessors; }
double* improvedpSwitch::getTotalProcessors() const { return totalProcessors; }
double* improvedpSwitch::getAvailableMemory() const { return availableMemory; }
double* improvedpSwitch::getTotalMemory() const { return totalMemory; }
double* improvedpSwitch::getAvailableAccelerators() const { return availableAccelerators; }
double* improvedpSwitch::getTotalAccelerators() const { return totalAccelerators; }
double* improvedpSwitch::getAvailableStorage() const { return availableStorage; }
double* improvedpSwitch::getTotalStorage() const { return totalStorage; }
double* improvedpSwitch::gsPMSA() const { return sPMSA; }
double* improvedpSwitch::gSIs() const { return SIs; }
double improvedpSwitch::gSI() const { return SI; }
double improvedpSwitch::gC() const { return C; }
double improvedpSwitch::gP() const { return P; }
double improvedpSwitch::gPi() const { return Pi; }
double improvedpSwitch::gCacc() const { return Cacc; }
double improvedpSwitch::gPacc() const { return Pacc; }
double improvedpSwitch::gPiacc() const { return Piacc; }