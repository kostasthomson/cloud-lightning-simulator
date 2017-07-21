#ifndef NETW_H
#define NETW_H
using namespace std;
#include <task.h>
#include <iterator>
#include <list>

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

  void deploy(const task* t);

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
