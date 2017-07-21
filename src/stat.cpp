#include <stat.h>
#include <fstream>
#include <iostream>
#include <jsoncons/json.hpp>
#include <string>

using std::cout;
using std::endl;

jsoncons::json js;
jsoncons::json cl;
jsoncons::json output_list = jsoncons::json::array();
jsoncons::json cl_list = jsoncons::json::array();

stat::stat()
  : processorsOverActiveServers(0),
    memoryOverActiveServers(0),
    storageOverActiveServers(0),
    acceleratorsOverActiveServers(0),
    alloc(0),
    currentTimestep(0.0),
    physicalMemory(0.0),
    physicalProcessors(0.0),
    physicalStorage(0.0),
    physicalNetwork(0.0),
    totalMemory(0.0),
    totalProcessors(0.0),
    availableProcessors(0.0),
    availableMemory(0.0),
    utilizedProcessors(0.0),
    utilizedMemory(0.0),
    totalStorage(0.0),
    availableStorage(0.0),
    utilizedStorage(0.0),
    totalNetwork(0.0),
    availableNetwork(0.0),
    utilizedNetwork(0.0),
    totalPowerConsumption(0.0),
    totalAccelerators(0),
    availableAccelerators(0),
    utilizedAccelerators(0),
    activeServers(0),
    numberOfTasks(0),
    rejectedTasks(0),
    acceptedTasks(0),
    actualUtilizedProcessors(0.0),
    actualUtilizedMemory(0.0),
    actualUtilizedNetwork(0.0)
{
}

stat::stat(const stat& t)
{
  if (t.alloc) {
    processorsOverActiveServers = t.processorsOverActiveServers;
    memoryOverActiveServers = t.memoryOverActiveServers;
    storageOverActiveServers = t.storageOverActiveServers;
    acceleratorsOverActiveServers = t.acceleratorsOverActiveServers;
    alloc = t.alloc;
    currentTimestep = t.currentTimestep;
    physicalMemory = t.physicalMemory;
    physicalProcessors = t.physicalProcessors;
    physicalStorage = t.physicalStorage;
    physicalNetwork = t.physicalNetwork;
    totalMemory = t.totalMemory;
    totalProcessors = t.totalProcessors;
    availableProcessors = t.availableProcessors;
    availableMemory = t.availableMemory;
    utilizedProcessors = t.utilizedProcessors;
    utilizedMemory = t.utilizedMemory;
    totalStorage = t.totalStorage;
    availableStorage = t.availableStorage;
    utilizedStorage = t.utilizedStorage;
    totalNetwork = t.totalNetwork;
    availableNetwork = t.availableNetwork;
    utilizedNetwork = t.utilizedNetwork;
    totalPowerConsumption = t.totalPowerConsumption;

    totalAccelerators = t.totalAccelerators;
    availableAccelerators = t.availableAccelerators;
    utilizedAccelerators = t.utilizedAccelerators;
    activeServers = t.activeServers;
    numberOfTasks = t.numberOfTasks;
    rejectedTasks = t.rejectedTasks;
    acceptedTasks = t.acceptedTasks;

    actualUtilizedProcessors = t.actualUtilizedProcessors;
    actualUtilizedMemory = t.actualUtilizedMemory;
    actualUtilizedNetwork = t.actualUtilizedNetwork;
  }
}

stat& stat::operator=(const stat& t)
{
  if (this != &t) {
    if (alloc) {
      processorsOverActiveServers = 0;
      memoryOverActiveServers = 0;
      storageOverActiveServers = 0;
      acceleratorsOverActiveServers = 0;
      alloc = 0;
      currentTimestep = 0.0;
      physicalMemory = 0.0;
      physicalProcessors = 0.0;
      physicalStorage = 0.0;
      physicalNetwork = 0.0;
      totalMemory = 0.0;
      totalProcessors = 0.0;
      availableProcessors = 0.0;
      availableMemory = 0.0;
      utilizedProcessors = 0.0;
      utilizedMemory = 0.0;
      totalStorage = 0.0;
      availableStorage = 0.0;
      utilizedStorage = 0.0;
      totalNetwork = 0.0;
      availableNetwork = 0.0;
      utilizedNetwork = 0.0;
      totalPowerConsumption = 0.0;

      totalAccelerators = 0;
      availableAccelerators = 0;
      utilizedAccelerators = 0;
      activeServers = 0;
      numberOfTasks = 0;
      rejectedTasks = 0;
      acceptedTasks = 0;

      actualUtilizedProcessors = 0.0;
      actualUtilizedMemory = 0.0;
      actualUtilizedNetwork = 0.0;
    }
    alloc = t.alloc;
    if (alloc) {
      processorsOverActiveServers = t.processorsOverActiveServers;
      memoryOverActiveServers = t.memoryOverActiveServers;
      storageOverActiveServers = t.storageOverActiveServers;
      acceleratorsOverActiveServers = t.acceleratorsOverActiveServers;
      currentTimestep = t.currentTimestep;
      physicalMemory = t.physicalMemory;
      physicalProcessors = t.physicalProcessors;
      physicalStorage = t.physicalStorage;
      physicalNetwork = t.physicalNetwork;
      totalMemory = t.totalMemory;
      totalProcessors = t.totalProcessors;
      availableProcessors = t.availableProcessors;
      availableMemory = t.availableMemory;
      utilizedProcessors = t.utilizedProcessors;
      utilizedMemory = t.utilizedMemory;
      totalStorage = t.totalStorage;
      availableStorage = t.availableStorage;
      utilizedStorage = t.utilizedStorage;
      totalNetwork = t.totalNetwork;
      availableNetwork = t.availableNetwork;
      utilizedNetwork = t.utilizedNetwork;
      totalPowerConsumption = t.totalPowerConsumption;

      totalAccelerators = t.totalAccelerators;
      availableAccelerators = t.availableAccelerators;
      utilizedAccelerators = t.utilizedAccelerators;
      activeServers = t.activeServers;
      numberOfTasks = t.numberOfTasks;
      rejectedTasks = t.rejectedTasks;
      acceptedTasks = t.acceptedTasks;

      actualUtilizedProcessors = t.actualUtilizedProcessors;
      actualUtilizedMemory = t.actualUtilizedMemory;
      actualUtilizedNetwork = t.utilizedNetwork;
    }
  }
  return *this;
}

stat::~stat()
{
  if (alloc) {
    processorsOverActiveServers = 0;
    memoryOverActiveServers = 0;
    storageOverActiveServers = 0;
    acceleratorsOverActiveServers = 0;
    alloc = 0;
    currentTimestep = 0.0;
    physicalMemory = 0.0;
    physicalProcessors = 0.0;
    physicalStorage = 0.0;
    physicalNetwork = 0.0;
    totalMemory = 0.0;
    totalProcessors = 0.0;
    availableProcessors = 0.0;
    availableMemory = 0.0;
    utilizedProcessors = 0.0;
    utilizedMemory = 0.0;
    totalStorage = 0.0;
    availableStorage = 0.0;
    utilizedStorage = 0.0;
    totalNetwork = 0.0;
    availableNetwork = 0.0;
    utilizedNetwork = 0.0;
    totalPowerConsumption = 0.0;

    totalAccelerators = 0;
    availableAccelerators = 0;
    utilizedAccelerators = 0;
    activeServers = 0;
    numberOfTasks = 0;
    rejectedTasks = 0;
    acceptedTasks = 0;

    actualUtilizedProcessors = 0.0;
    actualUtilizedMemory = 0.0;
    actualUtilizedNetwork = 0.0;
  }
}

void stat::print() const
{
  if (alloc) {
    cout << "         Active Servers: " << activeServers << endl;
    cout << "         Time Step: " << currentTimestep << endl;
    cout << "         Total Processors over Active Servers: " << processorsOverActiveServers << endl;
    cout << "         Total Memory over Active Servers: " << memoryOverActiveServers << endl;
    cout << "         Total Storage over Active Servers: " << storageOverActiveServers << endl;
    cout << "         Total Accelerators over Active Servers: " << acceleratorsOverActiveServers << endl;
    cout << "         Total Number of currently running VMs: " << numberOfTasks << endl;
    cout << "         Total Number of accepted Tasks: " << acceptedTasks << endl;
    cout << "         Total Number of rejected Tasks: " << rejectedTasks << endl;
    cout << "         Total Physical Processors: " << physicalProcessors << " Proc. Units" << endl;
    cout << "         Total Processors: " << totalProcessors << " Proc. Units" << endl;
    cout << "         Utilized Processors: " << utilizedProcessors << " Proc. Units" << endl;
    cout << "         Actual Utilized Processors: " << actualUtilizedProcessors << " Proc. Units" << endl;
    cout << "         Available Processors: " << availableProcessors << " Proc. Units" << endl;
    cout << "         Total Physical Memory: " << physicalMemory << " GBytes" << endl;
    cout << "         Total Memory: " << totalMemory << " GBytes" << endl;
    cout << "         Utilized Memory: " << utilizedMemory << " GBytes" << endl;
    cout << "         Actual Utilized Memory: " << actualUtilizedMemory << " Proc. Units" << endl;
    cout << "         Available Memory: " << availableMemory << " GBytes" << endl;
    cout << "         Total Physical Storage: " << physicalStorage << " TBytes" << endl;
    cout << "         Total Storage: " << totalStorage << " TBytes" << endl;
    cout << "         Utilized Storage: " << utilizedStorage << " TBytes" << endl;
    cout << "         Available Storage: " << availableStorage << " TBytes" << endl;
    cout << "         Total Physical Network: " << physicalNetwork << " Gbps" << endl;
    cout << "         Total Network: " << totalNetwork << " Gbps" << endl;
    cout << "         Utilized Network: " << utilizedNetwork << " Gbps" << endl;
    cout << "         Actual Utilized Network: " << actualUtilizedNetwork << " Gbps" << endl;
    cout << "         Available Network: " << availableNetwork << " Gbps" << endl;
    cout << "         Total Energy Consumption: " << totalPowerConsumption << " GWh" << endl;
    cout << "         Total Accelerators: " << totalAccelerators << endl;
    cout << "         Utilized Accelerators: " << utilizedAccelerators << endl;
    cout << "         Available Accelerators: " << availableAccelerators << endl;
  }
}

void stat::printfile(const string& outfile, const ios::openmode& mode)
{
  fstream file;
  if (alloc) {
    file.open(outfile.c_str(), mode);
    file << currentTimestep << " " << activeServers << " " << processorsOverActiveServers << " "
         << memoryOverActiveServers << " " << storageOverActiveServers << " " << acceleratorsOverActiveServers << " "
         << numberOfTasks << " " << acceptedTasks << " " << rejectedTasks << " " << availableProcessors << " "
         << utilizedProcessors << " " << actualUtilizedProcessors << " " << totalProcessors << " " << physicalProcessors
         << " " << availableMemory << " " << utilizedMemory << " " << actualUtilizedMemory << " " << totalMemory << " "
         << physicalMemory << " " << availableStorage << " " << utilizedStorage << " " << availableStorage << " "
         << physicalStorage << " " << availableNetwork << " " << utilizedNetwork << " " << actualUtilizedNetwork << " "
         << totalNetwork << " " << physicalNetwork << " " << availableAccelerators << " " << utilizedAccelerators << " "
         << totalAccelerators << " " << totalPowerConsumption << endl;
    file.close();
  }
}

void stat::printfileJson(const string& outfile, const string& inputfile, const ios::openmode& mode, int a, int b,
                         int overallRecords, int numOfCells, int numberOfTypes, int j)
{
  ifstream file;
  file.open(inputfile.c_str());
  std::ofstream ff(outfile, mode);
  output_list.clear();

  int k;
  for (k = 0; k < overallRecords; k++) {
    file >> currentTimestep;
    file >> activeServers;
    file >> processorsOverActiveServers;
    file >> memoryOverActiveServers;
    file >> storageOverActiveServers;
    file >> acceleratorsOverActiveServers;
    file >> numberOfTasks;
    file >> acceptedTasks;
    file >> rejectedTasks;
    file >> availableProcessors;
    file >> utilizedProcessors;
    file >> actualUtilizedProcessors;
    file >> totalProcessors;
    file >> physicalProcessors;
    file >> availableMemory;
    file >> utilizedMemory;
    file >> actualUtilizedMemory;
    file >> totalMemory;
    file >> physicalMemory;
    file >> availableStorage;
    file >> utilizedStorage;
    file >> totalStorage;
    file >> physicalStorage;
    file >> availableNetwork;
    file >> utilizedNetwork;
    file >> actualUtilizedNetwork;
    file >> totalNetwork;
    file >> physicalNetwork;
    file >> availableAccelerators;
    file >> utilizedAccelerators;
    file >> totalAccelerators;
    file >> totalPowerConsumption;

    js = jsoncons::json::object{
        {"Time Step", currentTimestep},
        {"Total Energy Consumption",totalPowerConsumption},
        {"Active Servers", activeServers},
        {"Total Number of currently running VMs",numberOfTasks},
        {"Total Processors over Active Servers",processorsOverActiveServers},
        {"Total Memory over Active Servers",memoryOverActiveServers},
        {"Total Storage over Active Servers",storageOverActiveServers},
        {"Total Accelerators over Active Servers",acceleratorsOverActiveServers},
        {"Total Number of accepted Tasks",acceptedTasks},
        {"Total Number of rejected Tasks",rejectedTasks},
        {"Total Physical Processors",physicalProcessors},
        {"Total Processors",totalProcessors},
        {"Utilized Processors",utilizedProcessors},
        {"Actual Utilized Processors",actualUtilizedProcessors},
        {"Available Processors",availableProcessors},
        {"Total Physical Memory",physicalMemory},
        {"Total Memory",totalMemory},
        {"Utilized Memory",utilizedMemory},
        {"Actual Utilized Memory",actualUtilizedMemory},
        {"Available Memory",availableMemory},
        {"Total Physical Storage",physicalStorage},
        {"Total Storage",totalStorage},
        {"Utilized Storage",utilizedStorage},
        {"Available Storage",availableStorage},
        {"Total Physical Network",physicalNetwork},
        {"Total Network",totalNetwork},
        {"Utilized Network",utilizedNetwork},
        {"Actual Utilized Network",actualUtilizedNetwork},
        {"Available Network",availableNetwork},
        {"Total Accelerators",totalAccelerators},
        {"Utilized Accelerators",utilizedAccelerators},
        {"Available Accelerators",availableAccelerators} 
        
    };
    output_list.add(js);
  }

  cl = jsoncons::json::object{
      {"Cell",a},
      {"HW Type", b},
      {"Outputs", output_list}
    };

  cl_list.add(cl);

  if (a == numOfCells && j == numberOfTypes){
    ff << std::setw(4) << pretty_print(cl_list) << std::endl;
  }
  file.close();
}