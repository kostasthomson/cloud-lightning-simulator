#include <mpi.h>
#include <netw.h>
#include <omp.h>
#include <resource.h>
#include <stat.h>
#include <task.h>
#include <improvedvRM.h>
#include <cmath>
#include <cstdlib>
#include <list>

void improvedvRM::print()
{
  if (alloc) {
    for (auto& it : *res) {
      (*it).print();
    }
  }
}

improvedvRM::improvedvRM()
  : alloc(0),
    numberOfResources(0),
    numberOfFunctions(0),
    optNumOfRes(0),
    pollIntervalvRM(0.0),
    queue(nullptr),
    res(nullptr),
    availableProcessors(nullptr),
    totalProcessors(nullptr),
    availableMemory(nullptr),
    totalMemory(nullptr),
    availableAccelerators(nullptr),
    totalAccelerators(nullptr),
    availableStorage(nullptr),
    totalStorage(nullptr),
    sPMSA(nullptr),
    Fs(nullptr),
    Ws(nullptr),
    C(0.0),
    P(0.0),
    Pi(0.0),
    Cacc(0.0),
    Pacc(0.0),
    Piacc(0.0),
    SI(0.0),
    dep_strategy(0)
{
}

improvedvRM::improvedvRM(const int& start, const int& end, const int& type, resource** resources, const double& L_pollIntervalvRM,
         const double& L_C, const double& L_Cacc, const double& L_P, const double& L_Pacc, const double& L_Pi, const double& L_Piacc, const int& L_optNumOfRes,
         const int& L_numberOfFunctions, const double* L_Ws, const int& L_dep_strategy)
{
  alloc = 1;
  numberOfResources = end - start;
  optNumOfRes = L_optNumOfRes;
  numberOfFunctions = L_numberOfFunctions;
  pollIntervalvRM = L_pollIntervalvRM;
  C = L_C;
  P = L_P;
  Pi = L_Pi;
  Cacc = L_Cacc;
  Pacc = L_Pacc;
  Piacc = L_Piacc;
  queue = new list<task>[1];
  res = new list<resource*>[1];

  for (int i = start; i < end; i++) {
    res->push_back(&resources[type][i]);
  }

  availableProcessors = new double[numberOfResources];
  totalProcessors = new double[numberOfResources];
  availableMemory = new double[numberOfResources];
  totalMemory = new double[numberOfResources];
  availableAccelerators = new double[numberOfResources];
  totalAccelerators = new double[numberOfResources];
  availableStorage = new double[numberOfResources];
  totalStorage = new double[numberOfResources];
  sPMSA = new double[8];

  for (int i = 0; i < 8; i++) {
    sPMSA[i] = 0.0;
  }

  Fs = new double[numberOfFunctions];
  Ws = new double[numberOfFunctions];

  for (int i = 0; i < numberOfFunctions; i++) {
    Ws[i] = L_Ws[i];
    Fs[i] = 0.0;
  }

  SI = 0.0;
  dep_strategy = L_dep_strategy;
  updateStateInfo(0.0);
}

improvedvRM::improvedvRM(const improvedvRM& t)
{
  if (t.galloc()) {
    alloc = 1;
    numberOfResources = t.getNumberOfResources();
    optNumOfRes = t.goptNumOfRes();
    numberOfFunctions = t.getNumberOfFunctions();
    C = t.gC();
    P = t.gP();
    Pi = t.gPi();
    Cacc = t.gCacc();
    Pacc = t.gPacc();
    Piacc = t.gPiacc();
    pollIntervalvRM = t.gpollIntervalvRM();
    queue = new list<task>[1];
    res = new list<resource*>[1];

    for (list<task>::iterator it = t.gqueue()->begin(); it != t.gqueue()->end(); it++) {
      queue->push_back(*it);
    }

    for (list<resource*>::iterator it = t.gres()->begin(); it != t.gres()->end(); it++) {
      res->push_back(*it);
    }

    availableProcessors = new double[numberOfResources];
    totalProcessors = new double[numberOfResources];
    availableMemory = new double[numberOfResources];
    totalMemory = new double[numberOfResources];
    availableAccelerators = new double[numberOfResources];
    totalAccelerators = new double[numberOfResources];
    availableStorage = new double[numberOfResources];
    totalStorage = new double[numberOfResources];

    for (int i = 0; i < numberOfResources; i++) {
      availableProcessors[i] = t.getAvailableProcessors()[i];
      totalProcessors[i] = t.getTotalProcessors()[i];
      availableMemory[i] = t.getAvailableMemory()[i];
      totalMemory[i] = t.getTotalMemory()[i];
      availableStorage[i] = t.getAvailableStorage()[i];
      totalStorage[i] = t.getTotalStorage()[i];
      availableAccelerators[i] = t.getAvailableAccelerators()[i];
      totalAccelerators[i] = t.getTotalAccelerators()[i];
    }

    sPMSA = new double[8];

    for (int i = 0; i < 8; i++) {
      sPMSA[i] = t.gsPMSA()[i];
    }

    Fs = new double[numberOfFunctions];
    Ws = new double[numberOfFunctions];

    for (int i = 0; i < numberOfFunctions; i++) {
      Fs[i] = t.gFs()[i];
      Ws[i] = t.gWs()[i];
    }

    SI = t.gSI();
    dep_strategy = t.gdep_strategy();
  }
}

improvedvRM& improvedvRM::operator=(const improvedvRM& t)
{
  if (this != &t) {
    if (alloc) {
      alloc = 0;
      numberOfResources = 0;
      optNumOfRes = 0;
      numberOfFunctions = 0;
      pollIntervalvRM = 0.0;
      C = 0.0;
      P = 0.0;
      Pi = 0.0;
      Cacc = 0.0;
      Pacc = 0.0;
      Piacc = 0.0;
      queue->clear();
      delete[] queue;
      res->clear();
      delete[] res;
      queue = nullptr;
      res = nullptr;
      delete[] availableProcessors;
      delete[] totalProcessors;
      delete[] availableMemory;
      delete[] totalMemory;
      delete[] availableStorage;
      delete[] totalStorage;
      delete[] availableAccelerators;
      delete[] totalAccelerators;
      delete[] sPMSA;
      delete[] Fs;
      delete[] Ws;
      availableProcessors = nullptr;
      totalProcessors = nullptr;
      availableMemory = nullptr;
      totalMemory = nullptr;
      availableAccelerators = nullptr;
      totalAccelerators = nullptr;
      availableStorage = nullptr;
      totalStorage = nullptr;
      sPMSA = nullptr;
      Fs = nullptr;
      Ws = nullptr;
      SI = 0.0;
      dep_strategy = 0;
    }
    alloc = t.galloc();
    if (alloc) {
      numberOfResources = t.getNumberOfResources();
      optNumOfRes = t.goptNumOfRes();
      numberOfFunctions = t.getNumberOfFunctions();
      C = t.gC();
      P = t.gP();
      Pi = t.gPi();
      Cacc = t.gCacc();
      Pacc = t.gPacc();
      Piacc = t.gPiacc();
      pollIntervalvRM = t.gpollIntervalvRM();
      queue = new list<task>[1];
      res = new list<resource*>[1];

      for (list<task>::iterator it = t.gqueue()->begin(); it != t.gqueue()->end(); it++) {
        queue->push_back(*it);
      }
      //  queue[0]=t.gqueue()[0];

      for (list<resource*>::iterator it = t.gres()->begin(); it != t.gres()->end(); it++) {
        res->push_back(*it);
      }
      //  res[0]=t.gres()[0];
      availableProcessors = new double[numberOfResources];
      totalProcessors = new double[numberOfResources];
      availableMemory = new double[numberOfResources];
      totalMemory = new double[numberOfResources];
      availableAccelerators = new double[numberOfResources];
      totalAccelerators = new double[numberOfResources];
      availableStorage = new double[numberOfResources];
      totalStorage = new double[numberOfResources];

      for (int i = 0; i < numberOfResources; i++) {
        availableProcessors[i] = t.getAvailableProcessors()[i];
        totalProcessors[i] = t.getTotalProcessors()[i];
        availableMemory[i] = t.getAvailableMemory()[i];
        totalMemory[i] = t.getTotalMemory()[i];
        availableStorage[i] = t.getAvailableStorage()[i];
        totalStorage[i] = t.getTotalStorage()[i];
        availableAccelerators[i] = t.getAvailableAccelerators()[i];
        totalAccelerators[i] = t.getTotalAccelerators()[i];
      }
      sPMSA = new double[8];

      for (int i = 0; i < 8; i++) {
        sPMSA[i] = t.gsPMSA()[i];
      }

      Fs = new double[numberOfFunctions];
      Ws = new double[numberOfFunctions];

      for (int i = 0; i < numberOfFunctions; i++) {
        Fs[i] = t.gFs()[i];
        Ws[i] = t.gWs()[i];
      }

      SI = t.gSI();
      dep_strategy = t.gdep_strategy();
    }
  }
  return *this;
}

improvedvRM::~improvedvRM()
{
  if (alloc) {
    alloc = 0;
    numberOfResources = 0;
    optNumOfRes = 0;
    numberOfFunctions = 0;
    pollIntervalvRM = 0.0;
    C = 0.0;
    P = 0.0;
    Pi = 0.0;
    Cacc = 0.0;
    Pacc = 0.0;
    Piacc = 0.0;
    queue->clear();
    delete[] queue;
    queue = nullptr;
    res->clear();
    delete[] res;
    res = nullptr;
    delete[] availableProcessors;
    delete[] totalProcessors;
    delete[] availableMemory;
    delete[] totalMemory;
    delete[] availableStorage;
    delete[] totalStorage;
    delete[] availableAccelerators;
    delete[] totalAccelerators;
    delete[] sPMSA;
    delete[] Fs;
    delete[] Ws;
    availableProcessors = nullptr;
    totalProcessors = nullptr;
    availableMemory = nullptr;
    totalMemory = nullptr;
    availableAccelerators = nullptr;
    totalAccelerators = nullptr;
    availableStorage = nullptr;
    totalStorage = nullptr;
    sPMSA = nullptr;
    Fs = nullptr;
    Ws = nullptr;
    SI = 0.0;
    dep_strategy = 0;
  }
}

void improvedvRM::obtainresources(list<resource*>& ores, double& remProc, double& remMem, double& remSto, double& remAcc)
{
  if (alloc) {
    if (remProc <= 0.0 && remMem <= 0.0 && remSto <= 0.0 && remAcc <= 0) {
      return;
    }
    list<resource*>::iterator it = res->begin();
    int i = 0;
    while (it != res->end()) {
      // If the resource is allowed to be movable to a different improvedvRM
      if ((*it)->getMovable() == 1) {
        ores.push_back(*it);
        numberOfResources--;
        remProc -= totalProcessors[i];
        remMem -= totalMemory[i];
        remSto -= totalStorage[i];
        remAcc -= totalAccelerators[i];
        sPMSA[0] -= totalProcessors[i];
        sPMSA[1] -= totalProcessors[i];
        sPMSA[2] -= totalMemory[i];
        sPMSA[3] -= totalMemory[i];
        sPMSA[4] -= totalStorage[i];
        sPMSA[5] -= totalStorage[i];
        sPMSA[6] -= totalAccelerators[i];
        sPMSA[7] -= totalAccelerators[i];

        double* availableProcessors2 = new double[numberOfResources];
        double* totalProcessors2 = new double[numberOfResources];
        double* availableMemory2 = new double[numberOfResources];
        double* totalMemory2 = new double[numberOfResources];
        double* availableAccelerators2 = new double[numberOfResources];
        double* totalAccelerators2 = new double[numberOfResources];
        double* availableStorage2 = new double[numberOfResources];
        double* totalStorage2 = new double[numberOfResources];

        int k = 0;
        for (int j = 0; j < i; j++) {
          availableProcessors2[k] = availableProcessors[j];
          totalProcessors2[k] = totalProcessors[j];
          availableMemory2[k] = availableMemory[j];
          totalMemory2[k] = totalMemory[j];
          availableAccelerators2[k] = availableAccelerators[j];
          totalAccelerators2[k] = totalAccelerators[j];
          availableStorage2[k] = availableStorage[j];
          totalStorage2[k] = totalStorage[j];
          k++;
        }
        for (int j = i + 1; j < numberOfResources + 1; j++) {
          availableProcessors2[k] = availableProcessors[j];
          totalProcessors2[k] = totalProcessors[j];
          availableMemory2[k] = availableMemory[j];
          totalMemory2[k] = totalMemory[j];
          availableAccelerators2[k] = availableAccelerators[j];
          totalAccelerators2[k] = totalAccelerators[j];
          availableStorage2[k] = availableStorage[j];
          totalStorage2[k] = totalStorage[j];
          k++;
        }
        delete[] availableProcessors;
        delete[] totalProcessors;
        delete[] availableMemory;
        delete[] totalMemory;
        delete[] availableAccelerators;
        delete[] totalAccelerators;
        delete[] availableStorage;
        delete[] totalStorage;
        availableProcessors = availableProcessors2;
        totalProcessors = totalProcessors2;
        availableMemory = availableMemory2;
        totalMemory = totalMemory2;
        availableAccelerators = availableAccelerators2;
        totalAccelerators = totalAccelerators2;
        availableStorage = availableStorage2;
        totalStorage = totalStorage2;
        availableProcessors2 = nullptr;
        totalProcessors2 = nullptr;
        availableMemory2 = nullptr;
        totalMemory2 = nullptr;
        availableAccelerators2 = nullptr;
        totalAccelerators2 = nullptr;
        availableStorage2 = nullptr;
        totalStorage2 = nullptr;
        computeFs();
        computeSI();
        it = res->erase(it);
        if (remProc <= 0.0 && remMem <= 0.0 && remSto <= 0.0 && remAcc <= 0) {
          break;
        }
      } else {
        ++it;
        i++;
      }
    }
  }
}

void improvedvRM::attachresources(list<resource*>& ores)
{
  if (alloc) {
    int nar = (int)ores.size();
    int i = numberOfResources;
    if (nar <= 0) {
      return;
    }
    numberOfResources += nar;

    double* availableProcessors2 = new double[numberOfResources];
    double* totalProcessors2 = new double[numberOfResources];
    double* availableMemory2 = new double[numberOfResources];
    double* totalMemory2 = new double[numberOfResources];
    double* availableAccelerators2 = new double[numberOfResources];
    double* totalAccelerators2 = new double[numberOfResources];
    double* availableStorage2 = new double[numberOfResources];
    double* totalStorage2 = new double[numberOfResources];

    list<resource*>::iterator it = ores.begin();
    while (it != ores.end()) {
      sPMSA[0] += (*it)->getTotalProcessors();
      sPMSA[1] += (*it)->getTotalProcessors();
      sPMSA[2] += (*it)->getTotalMemory();
      sPMSA[3] += (*it)->getTotalMemory();
      sPMSA[4] += (*it)->getTotalStorage();
      sPMSA[5] += (*it)->getTotalStorage();
      sPMSA[6] += (double)(*it)->getTotalAccelerators();
      sPMSA[7] += (double)(*it)->getTotalAccelerators();
      availableProcessors2[i] = (*it)->getTotalProcessors();
      totalProcessors2[i] = (*it)->getTotalProcessors();
      availableMemory2[i] = (*it)->getTotalMemory();
      totalMemory2[i] = (*it)->getTotalMemory();
      availableAccelerators2[i] = (double)(*it)->getTotalAccelerators();
      totalAccelerators2[i] = (double)(*it)->getTotalAccelerators();
      availableStorage2[i] = (*it)->getTotalStorage();
      totalStorage2[i] = (*it)->getTotalStorage();
      res->push_back(*it);
      i++;
      it++;
    }

    for (int j = 0; j < numberOfResources - nar; j++) {
      availableProcessors2[j] = availableProcessors[j];
      totalProcessors2[j] = totalProcessors[j];
      availableMemory2[j] = availableMemory[j];
      totalMemory2[j] = totalMemory[j];
      availableAccelerators2[j] = availableAccelerators[j];
      totalAccelerators2[j] = totalAccelerators[j];
      availableStorage2[j] = availableStorage[j];
      totalStorage2[j] = totalStorage[j];
    }
    delete[] availableProcessors;
    delete[] totalProcessors;
    delete[] availableMemory;
    delete[] totalMemory;
    delete[] availableAccelerators;
    delete[] totalAccelerators;
    delete[] availableStorage;
    delete[] totalStorage;
    availableProcessors = availableProcessors2;
    totalProcessors = totalProcessors2;
    availableMemory = availableMemory2;
    totalMemory = totalMemory2;
    availableAccelerators = availableAccelerators2;
    totalAccelerators = totalAccelerators2;
    availableStorage = availableStorage2;
    totalStorage = totalStorage2;
    availableProcessors2 = nullptr;
    totalProcessors2 = nullptr;
    availableMemory2 = nullptr;
    totalMemory2 = nullptr;
    availableAccelerators2 = nullptr;
    totalAccelerators2 = nullptr;
    availableStorage2 = nullptr;
    totalStorage2 = nullptr;

    // After the resources were attached to the improvedvRM, re-compute the assessment functions and the suitability index
    computeFs();
    computeSI();
  }
}

void improvedvRM::updateStateInfo(const double& tstep)
{
  int i;
  if (alloc) {
    if (((int)tstep % (int)pollIntervalvRM) == 0) {
      for (i = 0; i < 8; i++) {
        sPMSA[i] = 0.0;
      }
      i = 0;
      for (list<resource*>::iterator it = res->begin(); it != res->end(); it++) {
        availableProcessors[i] = (*it)->getAvailableProcessors();
        totalProcessors[i] = (*it)->getTotalProcessors();
        availableMemory[i] = (*it)->getAvailableMemory();
        totalMemory[i] = (*it)->getTotalMemory();
        availableAccelerators[i] = (double)(*it)->getAvailableAccelerators();
        totalAccelerators[i] = (double)(*it)->getTotalAccelerators();
        availableStorage[i] = (*it)->getAvailableStorage();
        totalStorage[i] = (*it)->getTotalStorage();
        i++;
      }
      for (i = 0; i < (int)res->size(); i++) {
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

int improvedvRM::galloc() const { return alloc; }
int improvedvRM::getNumberOfResources() const { return numberOfResources; }
int improvedvRM::goptNumOfRes() const { return optNumOfRes; }
double improvedvRM::gpollIntervalvRM() const { return pollIntervalvRM; }
list<task>* improvedvRM::gqueue() const { return queue; }
list<resource*>* improvedvRM::gres() const { return res; }
double* improvedvRM::getAvailableProcessors() const { return availableProcessors; }
double* improvedvRM::getTotalProcessors() const { return totalProcessors; }
double* improvedvRM::getAvailableMemory() const { return availableMemory; }
double* improvedvRM::getTotalMemory() const { return totalMemory; }
double* improvedvRM::getAvailableAccelerators() const { return availableAccelerators; }
double* improvedvRM::getTotalAccelerators() const { return totalAccelerators; }
double* improvedvRM::getAvailableStorage() const { return availableStorage; }
double* improvedvRM::getTotalStorage() const { return totalStorage; }
double* improvedvRM::gsPMSA() const { return sPMSA; }
double improvedvRM::gC() const { return C; }
double improvedvRM::gP() const { return P; }
double improvedvRM::gPi() const { return Pi; }
double improvedvRM::gCacc() const { return Cacc; }
double improvedvRM::gPacc() const { return Pacc; }
double improvedvRM::gPiacc() const { return Piacc; }
int improvedvRM::getNumberOfFunctions() const { return numberOfFunctions; }
double* improvedvRM::gFs() const { return Fs; }
double* improvedvRM::gWs() const { return Ws; }
double improvedvRM::gSI() const { return SI; }
int improvedvRM::gdep_strategy() const { return dep_strategy; }
void improvedvRM::computeFs()
{
  if (alloc) {
    for (int i = 0; i < numberOfFunctions; i++) {
      Fs[i] = assessfuncs(i);
    }
  }
}

void improvedvRM::computeSI()
{
  if (alloc) {
    SI = (1e-4) * (((double)rand()) / RAND_MAX);
    for (int i = 0; i < numberOfFunctions; i++)
      SI += Ws[i] * Fs[i];
  }
}

double improvedvRM::assessfuncs(const int& choice)
{
    switch (choice) {
      case 0:
        return (C*sPMSA[1]+Cacc*sPMSA[7])/(Pi*sPMSA[1]+(P-Pi)*(sPMSA[1]-sPMSA[0])+Piacc*sPMSA[6]+Pacc*(sPMSA[7]-sPMSA[6]));
        break;
      default:
        return 0.0;
    }
}

double improvedvRM::deassessmentFunctions(const double& dNu, const double& dAcc, const int& choice)
{
    switch (choice) {
      case 0:{
          double Ln = (Pi * sPMSA[1] + (P - Pi) * (sPMSA[1] - sPMSA[0]) + Piacc * sPMSA[6] + Pacc * (sPMSA[7] - sPMSA[6]));
          double Kn = C * sPMSA[1] + Cacc * sPMSA[7];
          return (Kn * (dNu * (P - Pi) + dAcc * Pacc)) / (Ln * Ln);
        }
        break;
      default:
        return 0.0;
    }
}

int improvedvRM::deploy_strategy(list<resource*>::iterator* it, int* IDs, const int& nVMs, const double& Proc,
                         const double& Mem, const double& Sto, const int& Acc)
{
  if (alloc) {
    int L_ID = -1, i, j, rem;
    list<resource*>::iterator itt;
    switch (dep_strategy) {
      // Task compaction strategy
      case 1:
        for (i = 0; i < nVMs; i++) {
          L_ID = -1;
          itt = res->begin();
          for (j = 0; j < numberOfResources; j++) {
            if (availableProcessors[j] >= Proc && availableMemory[j] >= Mem && availableStorage[j] >= Sto &&
                availableAccelerators[j] >= Acc) {
              L_ID = (*itt)->probe(Proc, Mem, Sto, Acc);
              if (L_ID != -1) {
                IDs[i] = j;
                it[i] = itt;
                break;
              }
            }
            itt++;
          }
          if (L_ID == -1) {
            break;
          } else {
            availableProcessors[j] -= Proc;
            availableMemory[j] -= Mem;
            availableStorage[j] -= Sto;
            availableAccelerators[j] -= Acc;
          }
        }
        if (L_ID == -1) {
          for (i = 0; i < nVMs; i++) {
            if (IDs[i] == -1) {
              break;
            }
            availableProcessors[IDs[i]] += Proc;
            availableMemory[IDs[i]] += Mem;
            availableStorage[IDs[i]] += Sto;
            availableAccelerators[IDs[i]] += Acc;
          }
          return (-1);
        }
        return (1);

      // Isotropy preservation strategy
      case 2:
        rem = nVMs;
        j = 0;
        i = 0;
        while (rem != 0 && i < nVMs) {
          j = 0;
          itt = res->begin();
          while (j < numberOfResources) {
            if (availableProcessors[j] >= Proc && availableMemory[j] >= Mem && availableStorage[j] >= Sto &&
                availableAccelerators[j] >= Acc) {
              L_ID = (*itt)->probe(Proc, Mem, Sto, Acc);
              if (L_ID != -1) {
                IDs[nVMs - rem] = j;
                it[nVMs - rem] = itt;
                availableProcessors[j] -= Proc;
                availableMemory[j] -= Mem;
                availableStorage[j] -= Sto;
                availableAccelerators[j] -= Acc;
                rem--;
              }
            }
            if (rem == 0)
              break;
            j++;
            itt++;
          }
          i++;
        }
        if (rem != 0) {
          for (i = 0; i < nVMs; i++) {
            if (IDs[i] == -1) {
              break;
            }
            availableProcessors[IDs[i]] += Proc;
            availableMemory[IDs[i]] += Mem;
            availableStorage[IDs[i]] += Sto;
            availableAccelerators[IDs[i]] += Acc;
          }
          return (-1);
        }
        return (1);
    }
  }
  return (0);
}

void improvedvRM::deploy(resource** resources, netw* network, stat* stats, task& task_)
{
  if (alloc) {
    int L_numberOfVMs = task_.getNumberOfVMs();
    int type = task_.getAvailableImplementations()[0];
    double* L_reqPMNS = task_.greqPMNS();
    double ssum = 0.0;
    int L_avAcc = task_.gavAcc()[0];
    int L_ID = -1;
    int* IDs;
    list<resource *>::iterator *it, itt;

    // Check if the task's processes, memory, storage and accelerator are less or equal than the improvedvRM's
    L_ID = network[0].probe(L_reqPMNS[2]);

    // If not, reject the task
    if (L_ID == -1) {
      stats[type].rejectedTasks++;
      return;
    }
    IDs = new int[L_numberOfVMs];
    it = new list<resource*>::iterator[L_numberOfVMs];
    for (int i = 0; i < L_numberOfVMs; i++) {
      IDs[i] = -1;
    }

    // Position L_numberOfVMs VMs on the improvedvRM resources
    L_ID = deploy_strategy(it, IDs, L_numberOfVMs, L_reqPMNS[0], L_reqPMNS[1], L_reqPMNS[3], L_avAcc);

    // If the VMs could not be positioned, reject the task
    if (L_ID == -1) {
      stats[type].rejectedTasks++;
    } else {
      // Deploy task on a resource
      // Set the ID of each VM as equal to the ID of the resource (match VMs to the appropriate resources)
      for (int i = 0; i < L_numberOfVMs; i++) {
        (*it[i])->deploy(task_);
        IDs[i] = (*it[i])->gID();
      }

      // Deploy task on network
      network[0].deploy(task_);

      // Add the resources-VMs couples to the task
      task_.attachResources(IDs);

      // Add the task to the list of tasks of the improvedvRM
      enque(task_);

      // Increase the number of active tasks of the improvedvRM
      stats[type].acceptedTasks++;

      // Update the assessment functions and the suitability index of the improvedvRM
      ssum = 0.0;
      for (int i = 0; i < 1; i++) {
        ssum += Ws[i] * deassessmentFunctions(-L_numberOfVMs * L_reqPMNS[0], -(double)L_numberOfVMs * L_avAcc, i);
      }
      SI += ssum;

      sPMSA[0] -= L_numberOfVMs * L_reqPMNS[0];
      sPMSA[2] -= L_numberOfVMs * L_reqPMNS[1];
      sPMSA[4] -= L_numberOfVMs * L_reqPMNS[3];
      sPMSA[6] -= (double)L_numberOfVMs * L_avAcc;
    }

    delete[] IDs;
    delete[] it;
  }
}

int improvedvRM::probe(const double& Proc, const double& Mem, const double& Sto, const int& Acc)
{
  if (Proc <= sPMSA[0] && Mem <= sPMSA[2] && Sto <= sPMSA[4] && Acc <= (int)sPMSA[6]) {
    return 1;
  } else {
    return -1;
  }
}

void improvedvRM::enque(const task& task_)
{
  if (alloc) {
    queue->push_back(task_);
  }
}
