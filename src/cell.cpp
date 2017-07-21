#include <brokers.h>
#include <cell.h>
#include <inputs.h>
#include <netw.h>
#include <power.h>
#include <stat.h>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <vector>

cell::cell()
  : ID(0),
    alloc(0),
    types(NULL),
    numberOfTypes(0),
    sosmIntegration(0),
    numberOfResourcesPerType(NULL),
    resources(NULL),
    powerComp(NULL),
    network(NULL),
    brok(NULL),
    traditionalBrok(NULL),
    stats(NULL)
{
}

cell::cell(const cellinputs& setup, int L_sosmIntegration) : alloc(1)
{
  ID = setup.ID;
  numberOfTypes = setup.numberOfTypes;
  sosmIntegration=L_sosmIntegration;
  types = new int[numberOfTypes];
  numberOfResourcesPerType = new int[numberOfTypes];

  for (int i = 0; i < numberOfTypes; i++) {
    types[i] = setup.types[i];
    numberOfResourcesPerType[i] = setup.numberOfResourcesPerType[i];
  }

  resources = new resource*[numberOfTypes];
  for (int i = 0; i < numberOfTypes; i++) {
    resources[i] = new resource[numberOfResourcesPerType[i]];
  }
  for (int i = 0; i < numberOfTypes; i++) {
    for (int j = 0; j < numberOfResourcesPerType[i]; j++) {
      // Copy information about computer resources in the resources[hardware type][resource] array
      resources[i][j] = resource(setup.rinp[i], j);
    }
  }

  powerComp = new power[numberOfTypes];
  for (int i = 0; i < numberOfTypes; i++) {
    powerComp[i] = power(setup.pinp[i]);
  }
  network = new netw[1];
  network[0] = netw(setup.ninp[0]);

  brok = new broker[1];
  traditionalBrok=new traditionalBroker[1];

  if (! sosmIntegration) {
    traditionalBrok[0]=traditionalBroker(numberOfTypes,types,numberOfResourcesPerType,setup.binp[0].pollIntervalCellM);
  }
    //	brok[0]=broker(numberOfTypes,types,numberOfResourcesPerType,resources,powerComp,network,setup.binp[0]);

  stats = new stat[numberOfTypes];
  for (int i = 0; i < numberOfTypes; i++) {
    stats[i] = stat();
  }

  if (! sosmIntegration) {
    traditionalBrok[0].updateStateInfo(resources,network,0.0);
  }

  // brok->updateStateInfo(network,0.0);
  updateStats(0.0);
}

cell::cell(const cell& t)
{
  if (t.galloc()) {
    ID = t.gID();
    alloc = t.galloc();
    numberOfTypes = t.getNumberOfTypes();
    sosmIntegration=t.getSosmIntegration();
    types = new int[numberOfTypes];
    numberOfResourcesPerType = new int[numberOfTypes];

    for (int i = 0; i < numberOfTypes; i++) {
      types[i] = t.getTypes()[i];
      numberOfResourcesPerType[i] = t.getNumberOfResourcesPerType()[i];
    }

    resources = new resource*[numberOfTypes];
    for (int i = 0; i < numberOfTypes; i++) {
      resources[i] = new resource[numberOfResourcesPerType[i]];
    }
    for (int i = 0; i < numberOfTypes; i++) {
      for (int j = 0; j < numberOfResourcesPerType[i]; j++)
        resources[i][j] = t.getResources()[i][j];
    }

    powerComp = new power[numberOfTypes];
    for (int i = 0; i < numberOfTypes; i++) {
      powerComp[i] = t.getPowerConsumption()[i];
    }

    network = new netw[1];
    network[0] = t.getNetwork()[0];
    brok = new broker[1];
    brok[0] = t.getBroker()[0];
    traditionalBrok=new traditionalBroker[1];
    traditionalBrok[0]=t.getTraditionalBroker()[0];
    stats = new stat[numberOfTypes];

    for (int i = 0; i < numberOfTypes; i++) {
      stats[i] = t.getStats()[i];
    }
  }
}

cell& cell::operator=(const cell& t)
{
  if (this != &t) {
    if (alloc) {
      ID = 0;
      alloc = 0;
      delete[] types;
      types = NULL;
      delete[] numberOfResourcesPerType;
      numberOfResourcesPerType = NULL;

      for (int i = 0; i < numberOfTypes; i++) {
        delete[] resources[i];
      }

      delete[] resources;
      delete[] powerComp;
      delete[] network;
      delete[] brok;
      delete[] traditionalBrok;
      delete[] stats;
      resources = NULL;
      powerComp = NULL;
      network = NULL;
      brok = NULL;
      traditionalBrok=NULL;
      stats = NULL;
      numberOfTypes = 0;
      sosmIntegration=0;
    }
    alloc = t.galloc();
    if (alloc) {
      ID = t.gID();
      numberOfTypes = t.getNumberOfTypes();
      sosmIntegration=t.getSosmIntegration();
      types = new int[numberOfTypes];

      numberOfResourcesPerType = new int[numberOfTypes];
      for (int i = 0; i < numberOfTypes; i++) {
        types[i] = t.getTypes()[i];
        numberOfResourcesPerType[i] = t.getNumberOfResourcesPerType()[i];
      }

      resources = new resource*[numberOfTypes];
      for (int i = 0; i < numberOfTypes; i++) {
        resources[i] = new resource[numberOfResourcesPerType[i]];
      }
      for (int i = 0; i < numberOfTypes; i++) {
        for (int j = 0; j < numberOfResourcesPerType[i]; j++)
          resources[i][j] = t.getResources()[i][j];
      }

      powerComp = new power[numberOfTypes];
      for (int i = 0; i < numberOfTypes; i++) {
        powerComp[i] = t.getPowerConsumption()[i];
      }

      network = new netw[1];
      network[0] = t.getNetwork()[0];
      brok = new broker[1];
      brok[0] = t.getBroker()[0];
      traditionalBrok=new traditionalBroker[1];
      traditionalBrok[0]=t.getTraditionalBroker()[0];

      stats = new stat[numberOfTypes];
      for (int i = 0; i < numberOfTypes; i++) {
        stats[i] = t.getStats()[i];
      }
    }
  }
  return *this;
}

cell::~cell()
{
  if (alloc) {
    ID = 0;
    alloc = 0;

    delete[] types;
    delete[] numberOfResourcesPerType;
    types = NULL;
    numberOfResourcesPerType = NULL;
    for (int i = 0; i < numberOfTypes; i++) {
      delete[] resources[i];
    }
    delete[] resources;
    delete[] powerComp;
    delete[] network;
    delete[] brok;
    delete[] traditionalBrok;
    delete[] stats;
    resources = NULL;
    powerComp = NULL;
    network = NULL;
    brok = NULL;
    traditionalBrok=NULL;
    stats = NULL;
    numberOfTypes = 0;
    sosmIntegration=0;
  }
}

void cell::timestep(const double& tstep)
{
  if (alloc) {
    if(getSosmIntegration()){
      brok[0].timestep(resources, network, stats, powerComp);
      brok->updateStateInfo(network, tstep);
    }
    else{
      traditionalBrok[0].timestep(resources,network,stats,powerComp);
      traditionalBrok[0].updateStateInfo(resources,network,tstep);
    }
    updateStats(tstep);
  }
}

int cell::gID() const { return ID; }
int cell::galloc() const { return alloc; }
int cell::getNumberOfTypes() const { return numberOfTypes; }
int cell::getSosmIntegration() const { return sosmIntegration; }
int* cell::getTypes() const { return types; }
int* cell::getNumberOfResourcesPerType() const { return numberOfResourcesPerType; }
resource** cell::getResources() const { return resources; }
power* cell::getPowerConsumption() const { return powerComp; }
broker* cell::getBroker() const { return brok; }
traditionalBroker *cell::getTraditionalBroker() const { return traditionalBrok; }
netw* cell::getNetwork() const { return network; }
stat* cell::getStats() const { return stats; }

void cell::deploy(list<task>* jobs)
{
  list<task>::iterator it;
  if (alloc) {
    if(getSosmIntegration()){
      for (it = jobs->begin(); it != jobs->end(); it++) {
        brok[0].deploy(resources, network, stats, &(*it));
      }
    }
    else{
      for (it = jobs->begin(); it != jobs->end(); it++){
        traditionalBrok[0].deploy(resources,network,stats,&(*it));
      }
    }
  }
}

void cell::updateStats(const double& tstep)
{
  int i, j;
  double physicalMemory, physicalProcessors, physicalStorage, totalMemory, totalProcessors, availableProcessors,
    availableMemory, totalStorage, availableStorage, actualUtilizedProcessors, actualUtilizedMemory;
  int totalAccelerators, availableAccelerators, activeServers, numberOfTasks, processorsOverActiveServers,
    processorsPerServer, memoryOverActiveServers, memoryPerServer, storageOverActiveServers, storagePerServer,
    acceleratorsOverActiveServers, acceleratorsPerServer;
  int omp_thr = atoi(getenv("OMP_NUM_THREADS"));
  for (i = 0; i < numberOfTypes; i++) {
    stats[i].alloc = 1;
    stats[i].currentTimestep = tstep;

    stats[i].totalNetwork = network[0].getTotalNetwork();
    stats[i].availableNetwork = network[0].getAvailableNetwork();
    stats[i].utilizedNetwork = stats[i].totalNetwork - stats[i].availableNetwork;
    stats[i].actualUtilizedNetwork = network[0].getActualUtilizedNetwork();

    processorsPerServer = resources[i][0].getTotalProcessors();
    memoryPerServer = resources[i][0].getTotalMemory();
    storagePerServer = resources[i][0].getTotalStorage();
    acceleratorsPerServer = resources[i][0].getTotalAccelerators();

    processorsOverActiveServers = 0;
    memoryOverActiveServers = 0;
    storageOverActiveServers = 0;
    acceleratorsOverActiveServers = 0;

    availableMemory = 0.0;
    physicalMemory = 0.0;
    totalMemory = 0.0;

    availableProcessors = 0.0;
    physicalProcessors = 0.0;
    totalProcessors = 0.0;

    availableStorage = 0.0;
    physicalStorage = 0.0;
    totalStorage = 0.0;

    availableAccelerators = 0;
    totalAccelerators = 0;

    activeServers = 0;
    numberOfTasks = 0;

    actualUtilizedProcessors = 0.0;
    actualUtilizedMemory = 0.0;

#pragma omp parallel for default(shared) private(j) num_threads(omp_thr) schedule(static)              \
  reduction(+ : physicalProcessors, totalProcessors, availableProcessors, physicalMemory, totalMemory, \
            availableMemory, physicalStorage, totalStorage, availableStorage, totalAccelerators,       \
            availableAccelerators, activeServers, numberOfTasks, actualUtilizedProcessors, actualUtilizedMemory)
    for (j = 0; j < numberOfResourcesPerType[i]; j++) {
      physicalProcessors += resources[i][j].getPhysicalProcessors();
      totalProcessors += resources[i][j].getTotalProcessors();
      availableProcessors += resources[i][j].getAvailableProcessors();
      physicalMemory += resources[i][j].getPhysicalMemory();
      totalMemory += resources[i][j].getTotalMemory();
      availableMemory += resources[i][j].getAvailableMemory();
      physicalStorage += resources[i][j].getPhysicalStorage();
      totalStorage += resources[i][j].getTotalStorage();
      availableStorage += resources[i][j].getAvailableStorage();
      totalAccelerators += resources[i][j].getTotalAccelerators();
      availableAccelerators += resources[i][j].getAvailableAccelerators();
      activeServers += resources[i][j].getActive();
      numberOfTasks += resources[i][j].getNumberOfTasks();

      actualUtilizedProcessors += resources[i][j].getActualUtilizedProcessors();
      actualUtilizedMemory += resources[i][j].getActualUtilizedMemory();
    }
    stats[i].physicalProcessors = physicalProcessors;
    stats[i].totalProcessors = totalProcessors;
    stats[i].availableProcessors = availableProcessors;
    stats[i].utilizedProcessors = totalProcessors - availableProcessors;
    stats[i].physicalMemory = physicalMemory;
    stats[i].totalMemory = totalMemory;
    stats[i].availableMemory = availableMemory;
    stats[i].utilizedMemory = totalMemory - availableMemory;
    stats[i].physicalStorage = physicalStorage;
    stats[i].totalStorage = totalStorage;
    stats[i].availableStorage = availableStorage;
    stats[i].utilizedStorage = totalStorage - availableStorage;
    stats[i].totalAccelerators = totalAccelerators;
    stats[i].availableAccelerators = availableAccelerators;
    stats[i].utilizedAccelerators = totalAccelerators - availableAccelerators;
    stats[i].activeServers = activeServers;
    stats[i].numberOfTasks = numberOfTasks;
    stats[i].actualUtilizedProcessors = actualUtilizedProcessors;
    stats[i].actualUtilizedMemory = actualUtilizedMemory;

    processorsOverActiveServers = activeServers * processorsPerServer;
    memoryOverActiveServers = activeServers * memoryPerServer;
    storageOverActiveServers = activeServers * storagePerServer;
    acceleratorsOverActiveServers = activeServers * acceleratorsPerServer;

    stats[i].processorsOverActiveServers = processorsOverActiveServers;
    stats[i].memoryOverActiveServers = memoryOverActiveServers;
    stats[i].storageOverActiveServers = storageOverActiveServers;
    stats[i].acceleratorsOverActiveServers = acceleratorsOverActiveServers;
  }
}

void cell::print()
{
  if (alloc) {
    cout << endl;
    cout << "Cell ID: " << ID << endl;
    cout << "Number of HW types: " << numberOfTypes << endl;
    cout << "HW types: ";
    for (int i = 0; i < numberOfTypes; i++) {
      cout << types[i] << " ";
    }
    cout << endl;
    cout << "Number of Resources Per Type: ";
    for (int i = 0; i < numberOfTypes; i++) {
      cout << numberOfResourcesPerType[i] << " ";
    }
    cout << endl;
    cout << "---------------------------------------------" << endl;

    network[0].print();
    brok[0].print();
		traditionalBrok[0].print();
    for (int i = 0; i < numberOfTypes; i++) {
      cout << "     Resource Type: " << types[i] << endl;
      powerComp[i].print();
      stats[i].print();
    }
  }
}
