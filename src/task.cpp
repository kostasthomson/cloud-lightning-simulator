#include <task.h>
#include <cmath>
#include <cstdlib>
#include <cstdlib>
#include <iostream>
using namespace std;

task::task()
{
  type = -1;
  numOfAvailImpl = 0;
  availImpl = NULL;
  reqIns = 0.0;
  numOfVMs = 0;
  reqPMNS = NULL;
  typeactPMN = NULL;
  minmaxactPMN = NULL;
  rhoAcc = NULL;
  avAcc = NULL;
  alloc = 0;
  resourceIDs = NULL;
  cUtilPMNr = NULL;
}

task::task(const task& t)
{
  int i, j;
  alloc = t.galloc();
  if (alloc) {
    type = t.gtype();
    numOfAvailImpl = t.gnumOfAvailImpl();
    availImpl = new int[numOfAvailImpl];
    for (i = 0; i < numOfAvailImpl; i++) {
      availImpl[i] = t.gavailImpl()[i];
    }
    reqIns = t.greqIns();
    numOfVMs = t.gnumOfVMs();
    reqPMNS = new double[4];
    for (i = 0; i < 4; i++) {
      reqPMNS[i] = t.greqPMNS()[i];
    }
    typeactPMN = new int[3];
    for (i = 0; i < 3; i++) {
      typeactPMN[i] = t.gtypeactPMN()[i];
    }
    minmaxactPMN = new double*[3];
    for (i = 0; i < 3; i++) {
      minmaxactPMN[i] = new double[2];
    }
    for (i = 0; i < 3; i++) {
      for (j = 0; j < 2; j++) {
        minmaxactPMN[i][j] = t.gminmaxactPMN()[i][j];
      }
    }
    rhoAcc = new double[numOfAvailImpl];
    avAcc = new int[numOfAvailImpl];
    for (i = 0; i < numOfAvailImpl; i++) {
      rhoAcc[i] = t.grhoAcc()[i];
      avAcc[i] = t.gavAcc()[i];
    }
    if (t.gresourceIDs() != NULL) {
      resourceIDs = new int[numOfVMs];
      for (i = 0; i < numOfVMs; i++)
        resourceIDs[i] = t.gresourceIDs()[i];
    } else
      resourceIDs = NULL;
    cUtilPMNr = new double[4];
    for (i = 0; i < 4; i++)
      cUtilPMNr[i] = t.gcUtilPMNr()[i];
  }
}

task& task::operator=(const task& t)
{
  int i, j;
  if (this != &t) {
    if (alloc) {
      numOfAvailImpl = 0;
      type = -1;
      delete[] availImpl;
      availImpl = NULL;
      reqIns = 0.0;
      numOfVMs = 0;
      delete[] reqPMNS;
      reqPMNS = NULL;
      delete[] typeactPMN;
      typeactPMN = NULL;
      for (i = 0; i < 3; i++) {
        delete[] minmaxactPMN[i];
      }
      delete[] minmaxactPMN;
      minmaxactPMN = NULL;
      delete[] rhoAcc;
      delete[] avAcc;
      rhoAcc = NULL;
      avAcc = NULL;
      alloc = 0;
      if (resourceIDs != NULL) {
        delete[] resourceIDs;
      }
      delete[] cUtilPMNr;
      cUtilPMNr = NULL;
    }
    alloc = t.galloc();
    if (alloc) {
      type = t.gtype();
      numOfAvailImpl = t.gnumOfAvailImpl();
      availImpl = new int[numOfAvailImpl];
      for (i = 0; i < numOfAvailImpl; i++) {
        availImpl[i] = t.gavailImpl()[i];
      }
      reqIns = t.greqIns();
      numOfVMs = t.gnumOfVMs();
      reqPMNS = new double[4];
      for (i = 0; i < 4; i++) {
        reqPMNS[i] = t.greqPMNS()[i];
      }
      typeactPMN = new int[3];
      for (i = 0; i < 3; i++) {
        typeactPMN[i] = t.gtypeactPMN()[i];
      }
      minmaxactPMN = new double*[3];
      for (i = 0; i < 3; i++) {
        minmaxactPMN[i] = new double[2];
      }
      for (i = 0; i < 3; i++) {
        for (j = 0; j < 2; j++) {
          minmaxactPMN[i][j] = t.gminmaxactPMN()[i][j];
        }
      }
      rhoAcc = new double[numOfAvailImpl];
      avAcc = new int[numOfAvailImpl];
      for (i = 0; i < numOfAvailImpl; i++) {
        rhoAcc[i] = t.grhoAcc()[i];
        avAcc[i] = t.gavAcc()[i];
      }
      if (t.gresourceIDs() != NULL) {
        resourceIDs = new int[numOfVMs];
        for (i = 0; i < numOfVMs; i++)
          resourceIDs[i] = t.gresourceIDs()[i];
      } else {
        resourceIDs = NULL;
      }
      cUtilPMNr = new double[4];
      for (i = 0; i < 4; i++) {
        cUtilPMNr[i] = t.gcUtilPMNr()[i];
      }
    }
  }
  return *this;
}

task::~task()
{
  int i;
  if (alloc) {
    type = -1;
    numOfAvailImpl = 0;
    delete[] availImpl;
    availImpl = NULL;
    reqIns = 0.0;
    numOfVMs = 0;
    delete[] reqPMNS;
    reqPMNS = NULL;
    delete[] typeactPMN;
    typeactPMN = NULL;
    for (i = 0; i < 3; i++)
      delete[] minmaxactPMN[i];
    delete[] minmaxactPMN;
    minmaxactPMN = NULL;
    delete[] avAcc;
    delete[] rhoAcc;
    rhoAcc = NULL;
    avAcc = NULL;
    alloc = 0;
    if (resourceIDs != NULL) {
      delete[] resourceIDs;
    }
    resourceIDs = NULL;
    delete[] cUtilPMNr;
    cUtilPMNr = NULL;
  }
}

task::task(const int& L_type, const int& L_numOfAvailImpl, const int* L_availImpl, const double& L_reqIns,
           const int& L_numOfVMs, const double& L_reqP, const double& L_reqM, const double& L_reqN,
           const double& L_reqS, const int& L_typeactP, const int& L_typeactM, const int& L_typeactN,
           const double* L_minmaxactP, const double* L_minmaxactM, const double* L_minmaxactN, const int* L_avAcc,
           const double* L_rhoAcc)
{
  int i;
  alloc = 1;
  type = L_type;
  numOfAvailImpl = L_numOfAvailImpl;
  availImpl = new int[numOfAvailImpl];
  for (i = 0; i < numOfAvailImpl; i++) {
    availImpl[i] = L_availImpl[i];
  }
  reqIns = L_reqIns;
  numOfVMs = L_numOfVMs;
  reqPMNS = new double[4];
  reqPMNS[0] = L_reqP;
  reqPMNS[1] = L_reqM;
  reqPMNS[2] = L_reqN;
  reqPMNS[3] = L_reqS;
  typeactPMN = new int[3];
  typeactPMN[0] = L_typeactP;
  typeactPMN[1] = L_typeactM;
  typeactPMN[2] = L_typeactN;
  minmaxactPMN = new double*[3];
  for (i = 0; i < 3; i++) {
    minmaxactPMN[i] = new double[2];
  }
  for (i = 0; i < 2; i++) {
    minmaxactPMN[0][i] = L_minmaxactP[i];
  }
  for (i = 0; i < 2; i++) {
    minmaxactPMN[1][i] = L_minmaxactM[i];
  }
  for (i = 0; i < 2; i++) {
    minmaxactPMN[2][i] = L_minmaxactN[i];
  }
  rhoAcc = new double[numOfAvailImpl];
  avAcc = new int[numOfAvailImpl];
  for (i = 0; i < numOfAvailImpl; i++) {
    avAcc[i] = L_avAcc[i];
    rhoAcc[i] = L_rhoAcc[i];
  }
  resourceIDs = NULL;
  cUtilPMNr = new double[4];
  for (i = 0; i < 4; i++) {
    cUtilPMNr[i] = 0.0;
  }
  // print();
}

void task::reduceIns(const double& amount) { reqIns -= amount; }

double task::getactP()
{
  double r = ((double)rand() / RAND_MAX);
  switch (typeactPMN[0]) {
    case 1:
      return (minmaxactPMN[0][0] + (minmaxactPMN[0][1] - minmaxactPMN[0][0]) * r);
      break;
    case 2:
      return 0;
      break;
    default:
      return 0;
      break;
  }
}

double task::getactM()
{
  double r = ((double)rand() / RAND_MAX);
  switch (typeactPMN[1]) {
    case 1:
      return (minmaxactPMN[1][0] + (minmaxactPMN[1][1] - minmaxactPMN[1][0]) * r);
      break;
    case 2:
      return 0;
      break;
    default:
      return 0;
      break;
  }
}

double task::getactN()
{
  double r = ((double)rand() / RAND_MAX);
  switch (typeactPMN[2]) {
    case 1:
      return (minmaxactPMN[2][0] + (minmaxactPMN[2][1] - minmaxactPMN[2][0]) * r);
      break;
    case 2:
      return 0;
      break;
    default:
      return 0;
      break;
  }
}

int task::gtype() const { return type; }
int task::gnumOfAvailImpl() const { return numOfAvailImpl; }
int* task::gavailImpl() const { return availImpl; }
double task::greqIns() const { return reqIns; }
int task::gnumOfVMs() const { return numOfVMs; }
double* task::greqPMNS() const { return reqPMNS; }
int* task::gtypeactPMN() const { return typeactPMN; }
double** task::gminmaxactPMN() const { return minmaxactPMN; }
int* task::gavAcc() const { return avAcc; }
double* task::grhoAcc() const { return rhoAcc; }
int task::galloc() const { return alloc; }
int* task::gresourceIDs() const { return resourceIDs; }
double* task::gcUtilPMNr() const { return cUtilPMNr; }

void task::compcUtilPMNr()
{
  if (alloc) {
    cUtilPMNr[0] = getactP() * reqPMNS[0];
    cUtilPMNr[1] = getactM() * reqPMNS[1];
    cUtilPMNr[2] = getactN() * reqPMNS[2];
    cUtilPMNr[3] = rhoAcc[0] * ((double)avAcc[0]);
  }
}

void task::attachResources(const int* IDs)
{
  int i;
  if (alloc) {
    if (resourceIDs == NULL) {
      resourceIDs = new int[numOfVMs];
      for (i = 0; i < numOfVMs; i++) {
        resourceIDs[i] = IDs[i];
      }
    }
  }
}

void task::detachResources()
{
  if (alloc) {
    if (resourceIDs != NULL) {
      delete[] resourceIDs;
      resourceIDs = NULL;
    }
  }
}

void task::remapType(const int* type, const int& num)
{
  int i;
  if (alloc) {
    for (i = 0; i < num; i++)
      availImpl[i] = type[i];
  }
}

void task::reduceImpl(const int* type)
{
  if (alloc) {
    numOfAvailImpl = 1;
    delete[] availImpl;
    availImpl = new int[1];
    availImpl[0] = *type;
    int L_avAcc = avAcc[*type];
    delete[] avAcc;
    avAcc = new int[1];
    avAcc[0] = L_avAcc;
    double L_rhoAcc = rhoAcc[*type];
    delete[] rhoAcc;
    rhoAcc = new double[1];
    rhoAcc[0] = L_rhoAcc;
  }
}

void task::print() const
{
  if (alloc) {
    cout << "-----------------------------------------------" << endl;
    cout << "Task type (oil, genomics etc): " << type << endl;
    cout << "Number of available implementations: " << numOfAvailImpl << endl;
    cout << "Available Implementations: ";
    for (int i = 0; i < numOfAvailImpl; i++) {
      cout << availImpl[i] << " ";
    }
    cout << endl;
    cout << "Number of Instructions: " << reqIns << endl;
    cout << "Number of VMs: " << numOfVMs << endl;
    cout << "vCPUs per VM: " << reqPMNS[0] << endl;
    cout << "Memory per VM: " << reqPMNS[1] << " GBytes" << endl;
    cout << "Storage per VM: " << reqPMNS[3] << " TBytes" << endl;
    cout << "Network per App: " << reqPMNS[2] << " Gbps" << endl;
    cout << "Type of Actual Utilization (Proc,Mem,Sto): ";
    for (int i = 0; i < 3; i++) {
      cout << typeactPMN[i] << " ";
    }
    cout << endl;
    cout << "Minimum - Maximum actual utilization Processors: " << minmaxactPMN[0][0] << " " << minmaxactPMN[0][1]
         << endl;
    cout << "Minimum - Maximum actual utilization Memory: " << minmaxactPMN[1][0] << " " << minmaxactPMN[1][1] << endl;
    cout << "Minimum - Maximum actual utilization Network: " << minmaxactPMN[2][0] << " " << minmaxactPMN[2][1] << endl;
    cout << "Accelerator support per Implementation: ";
    for (int i = 0; i < numOfAvailImpl; i++) {
      cout << avAcc[i] << " ";
    }
    cout << endl;
    cout << "Actual accelerator usage: ";
    for (int i = 0; i < numOfAvailImpl; i++) {
      cout << rhoAcc[i] << " ";
    }
    cout << endl;
    cout << "-----------------------------------------------" << endl;
  }
}
