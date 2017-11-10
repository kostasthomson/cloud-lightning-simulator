#ifndef IMPROVEDPROUTER_H
#define IMPROVEDPROUTER_H
#include <list>

class netw;
class resource;
class stat;
class task;
class improvedpSwitch;

using std::list;

class improvedpRouter
{
 private:
  int alloc;
  int numberOfpSwitches;
  int numberOfFunctions;
  double pollIntervalpRouter;
  list<improvedpSwitch*>* pSwitches;
  double* Fs;
  double* Ws;
  double* availableProcessors;
  double* totalProcessors;
  double* availableMemory;
  double* totalMemory;
  double* availableAccelerators;
  double* totalAccelerators;
  double* availableStorage;
  double* totalStorage;
  double* sPMSA;
  double SI, *SIs;
  double C, P, Pi, Cacc, Pacc, Piacc;

 public:
  improvedpRouter();

  ~improvedpRouter();

  improvedpRouter(const int& start, const int& end, const int& type, list<improvedpSwitch>** LpSwitches,
          const double& pollIntervalpRouter, const double& L_C, const double& L_Cacc, const double& L_P, const double& L_Pacc, const double& L_Pi,
          const double& L_Piacc, const int& L_numberOfFunctions, const double* L_Ws);

  improvedpRouter(const improvedpRouter& t);

  improvedpRouter& operator=(const improvedpRouter& t);

  /// Updates the state information of the cell, by calling recursively the pSwitch::updateStateInfo method
  void updateStateInfo(const double& tstep);

  /// Deploys the tasks to the appropriate vRMs, by calling recursively the pSwitch::deploy method
  void deploy(resource** resources, netw* network, stat* stats, task& task_);

  double deassessmentFunctions(const double& dNu, const double& availNu, const double& totNu, const double& dAcc, const double& availableAccelerators,
                               const double& totalAccelerators, const int& choice);

  /// Returns success if the task's processes, memory, storage and accelerator are less or equal than the improvedpRouter's
  int probe(const double& Proc, const double& Mem, const double& Sto, const int& Acc);

  /// Computes the assesment functions, by calling recursively the pSwitch::computeFs method
  void computeFs();

  /// Computes the suitability index
  void computeSI();

  void print() const;

  int galloc() const;
  int getNumberOfpSwitches() const;
  int getNumberOfFunctions() const;
  double gpollIntervalpRouter() const;
  list<improvedpSwitch*>* gpSwitches() const;
  double* gFs() const;
  double* gWs() const;
  double* getAvailableProcessors() const;
  double* getTotalProcessors() const;
  double* getAvailableMemory() const;
  double* getTotalMemory() const;
  double* getAvailableAccelerators() const;
  double* getTotalAccelerators() const;
  double* getAvailableStorage() const;
  double* getTotalStorage() const;
  double* gsPMSA() const;
  double gSI() const;
  double* gSIs() const;
  double gC() const;
  double gP() const;
  double gPi() const;
  double gCacc() const;
  double gPacc() const;
  double gPiacc() const;
};

#endif
