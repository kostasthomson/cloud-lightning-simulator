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

#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H
#include <mpi.h>
#include <list>

class cell;
class gs;
class task;

using std::list;

class communicator
{
 public:
  /// Sends configuration parameters from the gateway to the cells
  void simulationParameters(class siminputs& si, const int& rank, const int& clusterSize, const MPI_Comm& Comm);

  /// Retrieves statistics from the cells
  void cellStatistics(const gs* gates, const cell* clCell, const int& rank, const int& clusterSize,
                      const MPI_Comm& Comm);

  /// Sends tasks to the appropriate cells for further processing
  void taskParameters(list<task>& jobs, const int& rank, const int& clusterSize, const int* commCell,
                      const MPI_Comm& Comm);
};
#endif
