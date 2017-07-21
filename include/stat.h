#ifndef STAT_H
#define STAT_H
#include <fstream>
#include <iostream>
#include <string>

using std::fstream;
using std::ifstream;
using std::ios;
using std::string;


struct stat {
  stat();

  stat(const stat& t);

  stat& operator=(const stat& t);

  ~stat();

  void print() const;

  void printfile(const string& outfile, const ios::openmode& mode);

  void printfileJson(const string& outfile, const string& inputfile, const ios::openmode& mode, int i, int j,
                     int overallRecords, int numOfCells, int numberOfTypes, int ct);

  int processorsOverActiveServers;
  double memoryOverActiveServers;
  double storageOverActiveServers;
  int acceleratorsOverActiveServers;
  int alloc;
  double currentTimestep;
  double physicalMemory;
  double physicalProcessors;
  double physicalStorage;
  double physicalNetwork;
  double totalMemory;
  double totalProcessors;
  double availableProcessors;
  double availableMemory;
  double utilizedProcessors;
  double utilizedMemory;
  double totalStorage;
  double availableStorage;
  double utilizedStorage;
  double totalNetwork;
  double availableNetwork;
  double utilizedNetwork;
  double totalPowerConsumption;

  int totalAccelerators;
  int availableAccelerators;
  int utilizedAccelerators;
  int activeServers;
  int numberOfTasks;
  int rejectedTasks;
  int acceptedTasks;

  double actualUtilizedProcessors;
  double actualUtilizedMemory;
  double actualUtilizedNetwork;
};

#endif
