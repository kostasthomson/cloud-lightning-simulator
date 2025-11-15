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

#ifndef CELL_H
#define CELL_H
#include <sosmBroker.h>        // for sosmBroker
#include <traditionalBroker.h> // for traditionalBroker
#include <list>

class cellinputs;
class netw;
class power;
class resource;
class stat;
class task;

using std::list;

class cell
{
private:
  int ID;
  int alloc;
  int numberOfTypes;             //! Number of hardware types
  int decisionMaking;            //! Select resource allocation mechanism
  int *types;                    //! Hardware type
  int *numberOfResourcesPerType; //! Number of resources that correspond to each hardware type
  power *powerComp;
  netw *network;
  baseBroker *broker;
  resource **resources; //! Two-dimensional array of computer resources (servers)
  stat *stats;          //! Array to keep cell statistics

public:
  cell();

  /// Creates a cell based on user-supplied configuration
  /// \param setup Stores cell-related configuration from the CellData file
  /// \param decisionMaking Indicates whether to create a traditional or SOSM broker
  cell(const cellinputs &setup, int _sosmIntegration);

  cell(const cell &t);

  cell &operator=(const cell &t);

  ~cell();

  /// Deploys the tasks to the appropriate vRMs by calling recursively the sosmBroker::deploy method
  void deploy(list<task> &jobs);

  /// Updates cell-related statistics. The statistics are gathered per resource and summed.
  /// \param tstep The current time-step
  void updateStats(const double &tstep);

  void print();

  int gID() const;
  int galloc() const;
  int getNumberOfTypes() const;
  int getSosmIntegration() const;
  int *getTypes() const;
  int *getNumberOfResourcesPerType() const;
  resource **getResources() const;
  baseBroker *getBroker() const;
  power *getPowerConsumption() const;
  netw *getNetwork() const;
  stat *getStats() const;
};

#endif
