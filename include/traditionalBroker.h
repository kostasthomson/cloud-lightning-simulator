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

#ifndef TRADITIONALBROKER_H
#define TRADITIONALBROKER_H
#include <baseBroker.h>
#include <list>

class cell;
class siminputs;

using std::list;

class traditionalBroker : public baseBroker
{
private:
  double pollInterval;

  double **availableProcesses;
  double **totalProcesses;
  double **availableMemory;
  double **totalMemory;
  double **availableAccelerators;
  double **totalAccelerators;
  double **availableStorage;
  double **totalStorage;

  list<task> *queue;

public:
  traditionalBroker();

  // traditionalBroker(const int &L_numOfTypes, const int *L_types, const int *L_numOfResourcesPerType, const double &
  // L_pollInterval);

  traditionalBroker(const traditionalBroker &t);

  traditionalBroker &operator=(const traditionalBroker &t);

  ~traditionalBroker();

  void init(const cell *clCell, const siminputs *si);

  void print() const;

  void updateStateInfo(const cell *clCell, const double &tstep);

  void deploy(resource **resources, netw *network, stat *stats, task &_task);

  void enque(const task &task_);

  /// Performs the simulation phase
  void timestep(const cell *clCell);

  double gpollInterval() const;
  double **getAvailableProcesses() const;
  double **getTotalProcesses() const;
  double **getAvailableMemory() const;
  double **getTotalMemory() const;
  double **getAvailableAccelerators() const;
  double **getTotalAccelerators() const;
  double **getAvailableStorage() const;
  double **getTotalStorage() const;

  list<task> *gqueue() const;
};

#endif
