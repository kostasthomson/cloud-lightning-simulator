#ifndef MLBROKER_H
#define MLBROKER_H

#include <baseBroker.h>
#include <traditionalBroker.h>
#include <string>

class cell;
class siminputs;

class mlBroker : public baseBroker
{
 private:
  traditionalBroker* fallbackBroker;

  double** availableProcesses;
  double** totalProcesses;
  double** availableMemory;
  double** totalMemory;
  double** availableAccelerators;
  double** totalAccelerators;
  double** availableStorage;
  double** totalStorage;

  std::string buildAllocationRequest(const cell* clCell, const task& _task, double timestamp);
  bool parseAllocationResponse(const std::string& response, int& cellId, int& hwTypeId,
                                int** serverAssignments, int& numVMs);
  bool deployFromMLDecision(resource** resources, netw* network, stat* stats, task& _task,
                           int cellId, int hwTypeId, int* serverAssignments, int numVMs);

 public:
  mlBroker();
  mlBroker(const mlBroker& m);
  mlBroker& operator=(const mlBroker& m);
  ~mlBroker();

  void init(const cell* clCell, const siminputs* si);
  void print() const;
  void updateStateInfo(const cell* clCell, const double& tstep);
  void deploy(resource** resources, netw* network, stat* stats, task& _task);
  void timestep(const cell* clCell);
};

#endif
