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

#ifndef PSWITCH_H
#define PSWITCH_H

#include <list>

class netw;
class resource;
class stat;
class task;
class vRM;

using std::list;

class pSwitch
{
 private:
  int alloc;
  int numberOfvRMs;
  int numberOfFunctions;
  double pollIntervalpSwitch;
  list<vRM*>* vRMs;
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
  double C, P, Pi;

 public:
  pSwitch();

  ~pSwitch();

  pSwitch(const int& start, const int& end, const int& type, list<vRM>** LvRMs, const double& pollIntervalpSwitch,
          const double& L_C, const double& L_P, const double& L_Pi, const int& L_numberOfFunctions, const double* L_Ws);

  pSwitch(const pSwitch& t);

  pSwitch& operator=(const pSwitch& t);

  /// Computes the assesment functions, by calling recursively the vRM::computeFs method
  void computeFs();

  /// Computes the suitability index
  void computeSI();

  /// Updates the state information of the cell, by calling recursively the vRM::updateStateInfo method
  void updateStateInfo(const double& tstep);

  double deassessmentFunctions(const double& dNu, const double& totNu, const double& dNmem, const double& totalMemory,
                               const int& choice);

  /// Returns success if the task's processes, memory, storage and accelerator are less or equal than the pSwitch's
  int probe(const double& Proc, const double& Mem, const double& Sto, const int& Acc);

  /// Deploys the tasks to the appropriate vRMs, by calling recursively the vRM::deploy method
  void deploy(resource** resources, netw* network, stat* stats, task& task_);

  int galloc() const;
  int getNumberOfvRMs() const;
  int getNumberOfFunctions() const;
  double* gFs() const;
  double* gWs() const;
  double gpollIntervalpSwitch() const;
  list<vRM*>* getvRMs() const;
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
};

#endif
