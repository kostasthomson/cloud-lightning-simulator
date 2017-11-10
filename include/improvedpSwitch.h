#ifndef IMPROVEDPSWITCH_H
#define IMPROVEDPSWITCH_H

#include <list>

class netw;
class resource;
class stat;
class task;
class improvedvRM;

using std::list;

class improvedpSwitch
{
 private:
  int alloc;
  int numberOfvRMs;
  int numberOfFunctions;
  double pollIntervalpSwitch;
  list<improvedvRM*>* vRMs;
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
  double C, Cacc, P, Pi, Pacc, Piacc;

 public:
  improvedpSwitch();

  ~improvedpSwitch();

  improvedpSwitch(const int& start, const int& end, const int& type, list<improvedvRM>** LvRMs, const double& pollIntervalpSwitch,
                  const double& L_C, const double& L_Cacc, const double& L_P, const double& L_Pacc, const double& L_Pi, const double& L_Piacc,
                  const int& L_numberOfFunctions, const double* L_Ws);

  improvedpSwitch(const improvedpSwitch& t);

  improvedpSwitch& operator=(const improvedpSwitch& t);

  /// Computes the assesment functions, by calling recursively the vRM::computeFs method
  void computeFs();

  /// Computes the suitability index
  void computeSI();

  /// Updates the state information of the cell, by calling recursively the vRM::updateStateInfo method
  void updateStateInfo(const double& tstep);

  double deassessmentFunctions(const double& dNu,const double& availNu,const double& totNu,const double& dAcc,
                               const double& availableAccelerators, const double& totalAccelerators, const int& choice);

  /// Returns success if the task's processes, memory, storage and accelerator are less or equal than the improvedpSwitch's
  int probe(const double& Proc, const double& Mem, const double& Sto, const int& Acc);

  /// Deploys the tasks to the appropriate vRMs, by calling recursively the vRM::deploy method
  void deploy(resource** resources, netw* network, stat* stats, task& task_);

  int galloc() const;
  int getNumberOfvRMs() const;
  int getNumberOfFunctions() const;
  double* gFs() const;
  double* gWs() const;
  double gpollIntervalpSwitch() const;
  list<improvedvRM*>* getvRMs() const;
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
