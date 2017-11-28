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

#ifndef NETW_H
#define NETW_H

#include <list>

class netinputs;
class task;

using std::list;

class netw
{
 private:
  int alloc;
  double physicalNetwork;
  double totalNetwork;
  double utilizedNetwork;
  double availableNetwork;
  double actualUtilizedNetwork;
  double overCommitmentNetwork;
  int numberOfTasks;

 public:
  netw();

  netw(const netinputs& setup);

  netw(const netw& t);

  ~netw();

  netw& operator=(const netw& t);

  void initializeRunningQuantities();

  void incrementRunningQuantities(const double& uNetw);

  void sutilizedNetwork(const double& L_utilizedNetwork);

  void print() const;

  void deploy(const task& task_);

  void unload(list<task>::iterator& t);

  void unload(const double& L_availableNetwork, const double& L_actualUtilizedNetwork, const int& L_numberOfTasks);

  int probe(const double& requestedNetwork) const;

  int galloc() const;
  double getPhysicalNetwork() const;
  double getTotalNetwork() const;
  double getUtilizedNetwork() const;
  double getAvailableNetwork() const;
  double getActualUtilizedNetwork() const;
  int getNumberOfTasks() const;
  double getOverCommitmentNetwork() const;
};

#endif
