#include <mlBroker.h>
#include <httpClient.h>
#include <cell.h>
#include <inputs.h>
#include <netw.h>
#include <resource.h>
#include <stat.h>
#include <task.h>
#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <cstring>

using std::cout;
using std::endl;
using std::string;
using std::stringstream;

mlBroker::mlBroker()
    : baseBroker(),
      fallbackBroker(nullptr),
      availableProcesses(nullptr),
      totalProcesses(nullptr),
      availableMemory(nullptr),
      totalMemory(nullptr),
      availableAccelerators(nullptr),
      totalAccelerators(nullptr),
      availableStorage(nullptr),
      totalStorage(nullptr)
{
}

mlBroker::mlBroker(const mlBroker &m)
    : baseBroker(m)
{
  if (m.galloc())
  {
    fallbackBroker = new traditionalBroker(*m.fallbackBroker);

    availableProcesses = new double *[numberOfTypes];
    totalProcesses = new double *[numberOfTypes];
    availableMemory = new double *[numberOfTypes];
    totalMemory = new double *[numberOfTypes];
    availableAccelerators = new double *[numberOfTypes];
    totalAccelerators = new double *[numberOfTypes];
    availableStorage = new double *[numberOfTypes];
    totalStorage = new double *[numberOfTypes];

    for (int i = 0; i < numberOfTypes; i++)
    {
      availableProcesses[i] = new double[numberOfResourcesPerType[i]];
      totalProcesses[i] = new double[numberOfResourcesPerType[i]];
      availableMemory[i] = new double[numberOfResourcesPerType[i]];
      totalMemory[i] = new double[numberOfResourcesPerType[i]];
      availableAccelerators[i] = new double[numberOfResourcesPerType[i]];
      totalAccelerators[i] = new double[numberOfResourcesPerType[i]];
      availableStorage[i] = new double[numberOfResourcesPerType[i]];
      totalStorage[i] = new double[numberOfResourcesPerType[i]];
    }
  }
}

mlBroker &mlBroker::operator=(const mlBroker &m)
{
  if (this != &m)
  {
    baseBroker::operator=(m);

    if (fallbackBroker)
      delete fallbackBroker;
    if (m.fallbackBroker)
    {
      fallbackBroker = new traditionalBroker(*m.fallbackBroker);
    }
  }
  return *this;
}

mlBroker::~mlBroker()
{
  if (alloc)
  {
    if (fallbackBroker)
      delete fallbackBroker;

    for (int i = 0; i < numberOfTypes; i++)
    {
      delete[] availableProcesses[i];
      delete[] totalProcesses[i];
      delete[] availableMemory[i];
      delete[] totalMemory[i];
      delete[] availableAccelerators[i];
      delete[] totalAccelerators[i];
      delete[] availableStorage[i];
      delete[] totalStorage[i];
    }
    delete[] availableProcesses;
    delete[] totalProcesses;
    delete[] availableMemory;
    delete[] totalMemory;
    delete[] availableAccelerators;
    delete[] totalAccelerators;
    delete[] availableStorage;
    delete[] totalStorage;
  }
}

void mlBroker::init(const cell *clCell, const siminputs *si)
{
  baseBroker::init(clCell);

  fallbackBroker = new traditionalBroker();
  fallbackBroker->init(clCell, si);

  availableProcesses = new double *[numberOfTypes];
  totalProcesses = new double *[numberOfTypes];
  availableMemory = new double *[numberOfTypes];
  totalMemory = new double *[numberOfTypes];
  availableAccelerators = new double *[numberOfTypes];
  totalAccelerators = new double *[numberOfTypes];
  availableStorage = new double *[numberOfTypes];
  totalStorage = new double *[numberOfTypes];

  for (int i = 0; i < numberOfTypes; i++)
  {
    availableProcesses[i] = new double[numberOfResourcesPerType[i]];
    totalProcesses[i] = new double[numberOfResourcesPerType[i]];
    availableMemory[i] = new double[numberOfResourcesPerType[i]];
    totalMemory[i] = new double[numberOfResourcesPerType[i]];
    availableAccelerators[i] = new double[numberOfResourcesPerType[i]];
    totalAccelerators[i] = new double[numberOfResourcesPerType[i]];
    availableStorage[i] = new double[numberOfResourcesPerType[i]];
    totalStorage[i] = new double[numberOfResourcesPerType[i]];
  }
}

void mlBroker::print() const
{
  if (alloc)
  {
    cout << "     ML-Based Broker with Traditional Fallback" << endl;
  }
}

void mlBroker::updateStateInfo(const cell *clCell, const double &tstep)
{
  if (alloc)
  {
    for (int i = 0; i < numberOfTypes; i++)
    {
      for (int j = 0; j < numberOfResourcesPerType[i]; j++)
      {
        availableProcesses[i][j] = clCell[0].getResources()[i][j].getAvailableProcessors();
        totalProcesses[i][j] = clCell[0].getResources()[i][j].getTotalProcessors();
        availableMemory[i][j] = clCell[0].getResources()[i][j].getAvailableMemory();
        totalMemory[i][j] = clCell[0].getResources()[i][j].getTotalMemory();
        availableAccelerators[i][j] = clCell[0].getResources()[i][j].getAvailableAccelerators();
        totalAccelerators[i][j] = clCell[0].getResources()[i][j].getTotalAccelerators();
        availableStorage[i][j] = clCell[0].getResources()[i][j].getAvailableStorage();
        totalStorage[i][j] = clCell[0].getResources()[i][j].getTotalStorage();
      }
    }
    availableNetwork = clCell[0].getNetwork()[0].getAvailableNetwork();
    totalNetwork = clCell[0].getNetwork()[0].getTotalNetwork();
  }
  if (fallbackBroker)
  {
    fallbackBroker->updateStateInfo(clCell, tstep);
  }
}

void mlBroker::deploy(resource **resources, netw *network, stat *stats, task &_task)
{
  double *reqPMNS = _task.greqPMNS();
  int L_ID = network[0].probe(reqPMNS[2]);

  if (L_ID == -1)
  {
    int type = 0;
    for (int i = 0; i < numberOfTypes; i++)
    {
      if (types[i] == _task.getAvailableImplementations()[0])
      {
        type = i;
        break;
      }
    }
    stats[type].rejectedTasks++;
    return;
  }

  static double timestamp = 0.0;
  timestamp += 1.0;

  string jsonRequest = buildAllocationRequest(nullptr, _task, timestamp);

  if (jsonRequest.empty())
  {
    // cout << "[ML] Failed to build request, using fallback" << endl;
    fallbackBroker->deploy(resources, network, stats, _task);
    return;
  }

  string response = postJSON("/allocate_task", jsonRequest);

  if (response.empty())
  {
    // cout << "[ML] No response from ML service, using fallback" << endl;
    fallbackBroker->deploy(resources, network, stats, _task);
    return;
  }

  int cellId, hwTypeId;
  int *serverAssignments = new int[_task.getNumberOfVMs()];
  int numVMs = 0;

  bool parseSuccess = parseAllocationResponse(response, cellId, hwTypeId, &serverAssignments, numVMs);

  if (!parseSuccess || numVMs != _task.getNumberOfVMs())
  {
    // cout << "[ML] Failed to parse ML response, using fallback" << endl;
    delete[] serverAssignments;
    fallbackBroker->deploy(resources, network, stats, _task);
    return;
  }

  bool deploySuccess = deployFromMLDecision(resources, network, stats, _task,
                                            cellId, hwTypeId, serverAssignments, numVMs);

  delete[] serverAssignments;

  if (!deploySuccess)
  {
    // cout << "[ML] ML allocation failed, using fallback" << endl;
    fallbackBroker->deploy(resources, network, stats, _task);
  }
}

void mlBroker::timestep(const cell *clCell)
{
  if (fallbackBroker)
  {
    fallbackBroker->timestep(clCell);
  }
}

string mlBroker::buildAllocationRequest(const cell *clCell, const task &_task, double timestamp)
{
  stringstream json;
  json << "{\"timestamp\":" << timestamp << ",\"cells\":[{\"cell_id\":0,\"hw_types\":[";

  for (int i = 0; i < numberOfTypes; i++)
  {
    if (i > 0)
      json << ",";

    json << "{\"hw_type_id\":" << types[i]
         << ",\"hw_type_name\":\"Type" << types[i] << "\""
         << ",\"num_servers\":" << numberOfResourcesPerType[i];

    if (numberOfResourcesPerType[i] > 0)
    {
      json << ",\"num_cpus_per_server\":" << (int)totalProcesses[i][0]
           << ",\"memory_per_server\":" << totalMemory[i][0]
           << ",\"storage_per_server\":" << totalStorage[i][0]
           << ",\"compute_capability\":88000.8"
           << ",\"accelerators\":" << ((int)totalAccelerators[i][0] > 0 ? 1 : 0)
           << ",\"num_accelerators_per_server\":" << (int)totalAccelerators[i][0]
           << ",\"accelerator_compute_capability\":587505.34";
    }
    else
    {
      json << ",\"num_cpus_per_server\":20,\"memory_per_server\":128.0"
           << ",\"storage_per_server\":1.0,\"compute_capability\":88000.8"
           << ",\"accelerators\":0,\"num_accelerators_per_server\":0"
           << ",\"accelerator_compute_capability\":0.0";
    }

    json << ",\"cpu_power_consumption\":[163,170.1,172.6,175.4,179.8,183.6,190.0,196.8,206.3,215.9,220.2]"
         << ",\"cpu_utilization_bins\":[0.0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0]"
         << ",\"cpu_idle_power\":163.0,\"accelerator_idle_power\":32.0,\"accelerator_max_power\":250.0}";
  }

  json << "],\"available_resources\":{";
  for (int i = 0; i < numberOfTypes; i++)
  {
    if (i > 0)
      json << ",";
    double totalCPU = 0, totalMem = 0, totalStor = 0, totalAcc = 0;
    for (int j = 0; j < numberOfResourcesPerType[i]; j++)
    {
      totalCPU += availableProcesses[i][j];
      totalMem += availableMemory[i][j];
      totalStor += availableStorage[i][j];
      totalAcc += availableAccelerators[i][j];
    }
    json << "\"" << types[i] << "\":{\"cpu\":" << totalCPU
         << ",\"memory\":" << totalMem
         << ",\"storage\":" << totalStor
         << ",\"network\":" << availableNetwork
         << ",\"accelerators\":" << (int)totalAcc << "}";
  }

  json << "},\"current_utilization\":{";
  for (int i = 0; i < numberOfTypes; i++)
  {
    if (i > 0)
      json << ",";
    json << "\"" << types[i] << "\":{\"cpu\":0.5,\"memory\":0.5,\"network\":0.5}";
  }
  json << "}}],\"task\":{";

  json << "\"task_id\":\"task_" << timestamp << "\""
       << ",\"application_id\":" << _task.getType()
       << ",\"implementation_id\":" << _task.getAvailableImplementations()[0]
       << ",\"num_vms\":" << _task.getNumberOfVMs()
       << ",\"vcpus_per_vm\":" << (int)_task.greqPMNS()[0]
       << ",\"memory_per_vm\":" << _task.greqPMNS()[1]
       << ",\"storage_per_vm\":" << _task.greqPMNS()[3]
       << ",\"network_per_vm\":" << _task.greqPMNS()[2]
       << ",\"requires_accelerator\":" << (_task.gavAcc()[0] > 0 ? "true" : "false")
       << ",\"accelerator_utilization\":" << (_task.grhoAcc() ? _task.grhoAcc()[0] : 0.0)
       << ",\"estimated_duration\":3600.0}}";

  return json.str();
}

bool mlBroker::parseAllocationResponse(const string &response, int &cellId, int &hwTypeId,
                                       int **serverAssignments, int &numVMs)
{
  size_t pos;

  pos = response.find("\"success\"");
  if (pos == string::npos)
    return false;
  pos = response.find("true", pos);
  if (pos == string::npos)
    return false;

  pos = response.find("\"num_vms_allocated\"");
  if (pos == string::npos)
    return false;
  pos = response.find(":", pos) + 1;
  numVMs = atoi(response.c_str() + pos);

  pos = response.find("\"vm_allocations\"");
  if (pos == string::npos)
    return false;

  size_t vmStart = pos;
  for (int i = 0; i < numVMs; i++)
  {
    vmStart = response.find("{", vmStart + 1);
    if (vmStart == string::npos)
      return false;

    pos = response.find("\"cell_id\"", vmStart);
    if (pos == string::npos)
      return false;
    pos = response.find(":", pos) + 1;
    cellId = atoi(response.c_str() + pos);

    pos = response.find("\"hw_type_id\"", vmStart);
    if (pos == string::npos)
      return false;
    pos = response.find(":", pos) + 1;
    hwTypeId = atoi(response.c_str() + pos);

    pos = response.find("\"server_index\"", vmStart);
    if (pos == string::npos)
      return false;
    pos = response.find(":", pos) + 1;
    (*serverAssignments)[i] = atoi(response.c_str() + pos);
  }

  return true;
}

bool mlBroker::deployFromMLDecision(resource **resources, netw *network, stat *stats, task &_task,
                                    int cellId, int hwTypeId, int *serverAssignments, int numVMs)
{
  int type = -1;
  for (int i = 0; i < numberOfTypes; i++)
  {
    if (types[i] == hwTypeId)
    {
      type = i;
      _task.remapType(&i, 1);
      break;
    }
  }

  if (type == -1)
  {
    return false;
  }

  double *reqPMNS = _task.greqPMNS();
  int avAcc = _task.gavAcc()[0];

  for (int j = 0; j < numVMs; j++)
  {
    int serverIdx = serverAssignments[j];
    if (serverIdx < 0 || serverIdx >= numberOfResourcesPerType[type])
    {
      return false;
    }

    if (availableProcesses[type][serverIdx] < reqPMNS[0] ||
        availableMemory[type][serverIdx] < reqPMNS[1] ||
        availableStorage[type][serverIdx] < reqPMNS[3] ||
        availableAccelerators[type][serverIdx] < avAcc)
    {
      return false;
    }
  }

  availableNetwork -= reqPMNS[2];

  for (int j = 0; j < numVMs; j++)
  {
    int serverIdx = serverAssignments[j];
    resources[type][serverIdx].deploy(_task);
    availableProcesses[type][serverIdx] -= reqPMNS[0];
    availableMemory[type][serverIdx] -= reqPMNS[1];
    availableStorage[type][serverIdx] -= reqPMNS[3];
    availableAccelerators[type][serverIdx] -= avAcc;
  }

  network[0].deploy(_task);
  _task.attachResources(serverAssignments);
  stats[type].acceptedTasks++;

  // cout << "[ML] Successfully deployed task with " << numVMs
  //      << " VMs to HW Type " << hwTypeId << endl;

  return true;
}
