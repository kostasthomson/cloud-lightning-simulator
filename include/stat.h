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

#ifndef STAT_H
#define STAT_H
#include <fstream>
#include <string>

using std::fstream;
using std::ifstream;
using std::ios;
using std::string;

class stat
{
 public:
  stat();

  stat(const stat& t);

  stat& operator=(const stat& t);

  ~stat();

  void print() const;

  void printfile(const string& outfile, const ios::openmode& mode);

  void printfileJson(const string& outfile, const string& inputfile, const ios::openmode& mode, int i, int j,
                     int overallRecords, int numOfCells, int numberOfTypes, int ct, int l_sosmIntegration, int allTasks);

  void printfileJsonDirect(const string& outfile, const ios::openmode& mode, int i, int j,
                           int numOfCells, int numberOfTypes, int ct, int l_sosmIntegration, int allTasks, int k, int timeStep);

  int processorsOverActiveServers;
  double memoryOverActiveServers;
  double storageOverActiveServers;
  int acceleratorsOverActiveServers;
  int alloc;
  double currentTimestep;       //! Time Step
  double physicalMemory;        //! Total Physical Memory
  double physicalProcessors;    //! Total Physical Processors
  double physicalStorage;       //! Total Physical Storage
  double physicalNetwork;       //! Total Physical Network
  double totalMemory;           //! Total Memory
  double totalProcessors;       //! Total Processors
  double availableProcessors;   //! Available Processors
  double availableMemory;       //! Available Memory
  double utilizedProcessors;    //! Utilized Processors
  double utilizedMemory;        //! Utilized Memory
  double totalStorage;          //! Total Storage
  double availableStorage;      //! Available Storage
  double utilizedStorage;       //! Utilized Storage
  double totalNetwork;          //! Total Network
  double availableNetwork;      //! Available Network
  double utilizedNetwork;       //! Utilized Network
  double totalPowerConsumption; //! Total Energy Consumption

  int totalAccelerators;     //! Total Accelerators
  int availableAccelerators; //! Available Accelerators
  int utilizedAccelerators;  //! Utilized Accelerators
  int activeServers;         //! Active Servers
  int runningVMs;         //! Total Number of currently running VMs
  int rejectedTasks;         //! Total Number of rejected Tasks
  int acceptedTasks;         //! Total Number of accepted Tasks

  double actualUtilizedProcessors; //! Actual Utilized Processors
  double actualUtilizedMemory;     //! Actual Utilized Memory
  double actualUtilizedNetwork;    //! Actual Utilized Network
};

#endif
