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

#ifndef IMPROVEDVRM_H
#define IMPROVEDVRM_H
#include <list>

class netw;
class resource;
class stat;
class task;

using std::list;

class improvedvRM
{
 private:
  int alloc;
  int numberOfResources;
  int numberOfFunctions;
  int optNumOfRes;
  double pollIntervalvRM;
  list<task>* queue;
  list<resource*>* res;
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
  double C, P, Pi, Cacc, Pacc, Piacc;
  double SI;
  int dep_strategy;

 public:
  improvedvRM();

  ~improvedvRM();

  improvedvRM(const int& start, const int& end, const int& type, resource** resources, const double& L_pollIntervalvRM,
      const double& L_C, const double& L_Cacc, const double& L_P, const double& L_Pacc, const double& L_Pi, const double& L_Piacc, const int& L_optNumOfRes,
      const int& L_numberOfFunctions, const double* L_Ws, const int& L_dep_strategy);

  improvedvRM(const improvedvRM& t);

  improvedvRM& operator=(const improvedvRM& t);

  /// Computes the assesment functions
  void computeFs();

  /// Computes the suitability index
  void computeSI();

  /// Updates the state information of the cell
  void updateStateInfo(const double& tstep);

  /// Removes the list of resources that satisfy the requested units from a improvedvRM and adds them to the ores list
  /// \param ores The resources list to store the resources removed from the improvedvRM
  /// \param remProc The number of requested processor cores
  /// \param remMem The size of requested memory
  /// \param remSto The size of requested storage
  /// \param remAcc The number of requested accelerators
  void obtainresources(list<resource*>& ores, double& remProc, double& remMem, double& remSto, double& remAcc);

  /// Adds the resources contained in the ores list to the improvedvRM
  /// \param The resources list to be attached to the improvedvRM
  void attachresources(list<resource*>& ores);

  double assessfuncs(const int& choice);

  double deassessmentFunctions(const double& dNu, const double& dAcc, const int& choice);

  /// Returns success if the task's processes, memory, storage and accelerator are less or equal than the improvedvRM's
  int probe(const double& Proc, const double& Mem, const double& Sto, const int& Acc);

  /// Defines the strategies used to position VMs on resources
  int deploy_strategy(list<resource*>::iterator* it, int* IDs, const int& nVMs, const double& Proc, const double& Mem,
                      const double& Sto, const int& Acc);

  /// Deploys the tasks
  void deploy(resource** resources, netw* network, stat* stats, task& task_);

  void enque(const task& task_);

  void print();

  int galloc() const;
  int getNumberOfResources() const;
  int goptNumOfRes() const;
  double gpollIntervalvRM() const;
  list<task>* gqueue() const;
  list<resource*>* gres() const;
  double* getAvailableProcessors() const;
  double* getTotalProcessors() const;
  double* getAvailableMemory() const;
  double* getTotalMemory() const;
  double* getAvailableAccelerators() const;
  double* getTotalAccelerators() const;
  double* getAvailableStorage() const;
  double* getTotalStorage() const;
  double* gsPMSA() const;
  double* gFs() const;
  double* gWs() const;
  double gC() const;
  double gP() const;
  double gPi() const;
  double gSI() const;
  double gCacc() const;
  double gPacc() const;
  double gPiacc() const;
  int gdep_strategy() const;
  int getNumberOfFunctions() const;
};

#endif
