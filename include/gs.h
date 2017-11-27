/* Copyright 2015 The CloudLightning Simulation Framework Authors. All
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

#ifndef GS_H
#define GS_H

#include <inputs.h>
#include <string> // for string

class stat;

template <typename T>
std::string num2str(T num);

class gs
{
 private:
  int alloc;
  appinputs* ai;
  siminputs* si;
  stat** stats;

 public:
  gs();

  /// Parse the simulator configuration files
  /// \param cellData The path to the CellData configuration file
  /// \param appData The path to the AppData configuration file
  /// \param brokerData The path to the BrokerData configuration file
  gs(const std::string& cellData, const std::string& appData, const std::string& brokerData);

  gs(const gs& t);

  gs& operator=(const gs& t);

  ~gs();

  /// Retrieve the list of candidate cells for each task and select the most appropriate
  /// \param numImpl Number of implementations (hardware types)
  /// \param rVM Required number of VMs
  /// \param rvProc Required number of processing units
  /// \param rMem Required memory size
  /// \param rNet Required netowrk bandwidth
  /// \param rSto Required storage size
  /// \param rAcc Required accelerators
  int findCell(const int* rImpl, const int& numImpl, const int& rVM, const double& rvProc, const double& rMem,
               const double& rNet, const double& rSto, const int* rAcc) const;

  std::string num2str(int num);

  // Gateways
  int galloc() const;

  appinputs* gai() const;

  siminputs* gsi() const;

  stat** getStats() const;

  void print();

  void printStats();

  void printStats(const std::string& outfile, const ios::openmode& mode);

  void printfile(const std::string& outfile, const ios::openmode& mode);

  void printStatsJson(const std::string& outfile, const ios::openmode& mode, int endTime, int updateInterval, int sosmIntegration, int allTasks);
};

#endif
