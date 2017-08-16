#ifndef RESOURCE_H
#define RESOURCE_H

#include <list>

class resinputs;
class task;

using std::list;

class resource
{
 private:
  int alloc;
  int active;
  int movable;
  int type;
  int ID;

  double totalProcessors;
  double availableProcessors;
  double utilizedProcessors;

  double totalMemory;
  double availableMemory;
  double utilizedMemory;

  double totalStorage;
  double availableStorage;
  double utilizedStorage;

  double physicalProcessors;
  double physicalMemory;
  double physicalStorage;

  double computeCapability;

  int accelerator;
  int totalAccelerators;
  int availableAccelerators;
  int utilizedAccelerators;
  double acceleratorComputeCapability;

  double overcommitmentProcessors;
  double overcommitmentMemory;

  double actualUtilizedProcessors;
  double actualUtilizedMemory;
  double actualRhoAccelerators;
  double currentCompCapPerProc;
  double currentCompCapPerAcc;

  int numberOfTasks;

 public:
  resource();
  resource(const resinputs& setup, const int& iD);
  resource(const resource& t);

  resource& operator=(const resource& t);

  ~resource();

  /// Initializes the running quantities of a resource
  void initializeRunningQuantities();

  /// Calculates the compute capability of each processor
  void compcurrentCompCapPerProc();

  /// Calculates the compute capability of each accelerator
  void compcurrentCompCapPerAcc();

  /// Increments the running quantities of a resource by the provided values for processors, memory and accelerators
  void incrementRunningQuantities(const double& uProc, const double& uMem, const double& rAcc);

  /// Assigns a task to a resource: Increments the number of tasks variable, reduces the resources' available
  /// processors, memory, storage and accelerators and calculates the utilized units
  /// \param t The task to be assigned to the resource
  void deploy(const task& task_);

  int probe(const double& reqProc, const double& reqMem, const double& reqSto, const int& reqAcc);

  /// Dissociates a task from a resource by reversing the actions of the deploy method
  /// \param t The iterator of a list of tasks that points to the task to be dissociated
  void unload(const list<task>::iterator& t);

  int galloc() const;
  int getActive() const;
  int getMovable() const;
  int getType() const;
  int gID() const;

  double getTotalProcessors() const;
  double getAvailableProcessors() const;
  double getUtilizedProcessors() const;

  double getTotalMemory() const;
  double getAvailableMemory() const;
  double getUtilizedMemory() const;

  double getTotalStorage() const;
  double getAvailableStorage() const;
  double getUtilizedStorage() const;

  double getPhysicalProcessors() const;
  double getPhysicalMemory() const;
  double getPhysicalStorage() const;

  double getComputeCapability() const;

  int getAccelerator() const;
  int getTotalAccelerators() const;
  int getAvailableAccelerators() const;
  int getUtilizedAccelerators() const;
  double getAcceleratorComputeCapability() const;

  double getOvercommitmentProcessors() const;
  double getOvercommitmentMemory() const;

  double getActualUtilizedProcessors() const;
  double getActualUtilizedMemory() const;
  double getActualRhoAccelerators() const;

  double getCurrentCompCapPerProc() const;
  double getCurrentCompCapPerAcc() const;

  int getNumberOfTasks() const;
  void print() const;
};

#endif
