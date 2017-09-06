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
                     int overallRecords, int numOfCells, int numberOfTypes, int ct, int l_sosmIntegration);

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
  int numberOfTasks;         //! Total Number of currently running VMs
  int rejectedTasks;         //! Total Number of rejected Tasks
  int acceptedTasks;         //! Total Number of accepted Tasks

  double actualUtilizedProcessors; //! Actual Utilized Processors
  double actualUtilizedMemory;     //! Actual Utilized Memory
  double actualUtilizedNetwork;    //! Actual Utilized Network
};

#endif
