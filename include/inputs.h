#ifndef INPUTS_H
#define INPUTS_H
#include <fstream>
#include <string>

using std::string;
using std::fstream;
using std::ifstream;
using std::ios;

class brinputs
{
 public:
  int alloc;
  int numberOfFunctions;
  double* Ws;
  int initResPervRM;
  int initvRMPerpSwitch;
  int initpSwitchPerpRouter;
  double pollIntervalCellM;
  double pollIntervalpRouter;
  double pollIntervalpSwitch;
  double pollIntervalvRM;
  int vRMdeploystrategy;
  brinputs();
  brinputs(const brinputs& t);
  brinputs& operator=(const brinputs& t);
  ~brinputs();
  void parse(const string& outname, int cell_id);
  void print();
  void printfile(const string& outname, const ios::openmode& mode, int sosmIntegration);
};

class appinputs
{
 public:
  int alloc;
  double* minmaxJobsPerSec;
  int numOfApps;
  int* numberOfAvailableImplementationsPerApp;
  int** availableImplementationsPerApp;
  int** minmaxVMPerApp;
  double** minmaxInsPerApp;
  double** minmaxProcPerVM;
  double** minmaxMemPerVM;
  double** minmaxStoPerVM;
  double** minmaxNetPerApp;
  int* typeOfActP;
  int* typeOfActM;
  int* typeOfActN;
  double** minmaxActP;
  double** minmaxActM;
  double** minmaxActN;
  int** accelerator;
  double** rhoAcc;
  void parse(const string& fname);
  void print();
  void printfile(const string& outname, const ios::openmode& mode);
  appinputs();
  ~appinputs();
  appinputs& operator=(const appinputs& t);
  appinputs(const appinputs& t);
};

class netinputs
{
 public:
  int alloc;
  double netBW;
  double overCommitmentNetwork;
  void print();
  netinputs();
  ~netinputs();
  netinputs& operator=(const netinputs& t);
  netinputs(const netinputs& t);
};

class powinputs
{
 public:
  int alloc;
  int typeCpu;
  int typeAcc;
  double cpuPmin, cpuPmax, cpuC;
  int numOfPoints;
  double *cpubins, *cpuP;
  int accelerator;
  double accPmin, accPmax, accC;
  powinputs();
  powinputs(const powinputs& t);
  ~powinputs();
  powinputs& operator=(const powinputs& t);
};

class resinputs
{
 public:
  int alloc;
  double numOfProcUnits;
  double totalMemory;
  double totalStorage;
  double overcommitmentProcessors;
  double overcommitmentMemory;
  double computeCapability;
  int accelerator;
  double acceleratorComputeCapability;
  int totalAccelerators;
  int type;
  resinputs();
  resinputs(const resinputs& t);
  ~resinputs();
  resinputs& operator=(const resinputs& t);
};

class cellinputs
{
 public:
  int alloc;
  int ID;
  class resinputs* rinp;
  class powinputs* pinp;
  class netinputs* ninp;
  class brinputs* binp;
  int numberOfTypes;
  int* types;
  int* numberOfResourcesPerType;
  cellinputs();
  cellinputs(const cellinputs& t);
  ~cellinputs();
  cellinputs& operator=(const cellinputs& t);
};

class siminputs
{
 public:
  class cellinputs* cinp;
  int alloc;
  int numOfCells;
  double maxTime;
  int sosmIntegration;
  double updateInterval;

  siminputs();
  siminputs(const siminputs& t);
  void parse(const string& fname, const string& bname);
  ~siminputs();
  siminputs& operator=(const siminputs& t);
  void print();
  void printfile(const string& outname, const ios::openmode& mode);
};

#endif
