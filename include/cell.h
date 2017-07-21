#ifndef CELL_H
#define CELL_H
#include <brokers.h>
#include <inputs.h>
#include <netw.h>
#include <power.h>
#include <resource.h>
#include <stat.h>
#include <task.h>

class cell
{
 private:
  int ID;
  int alloc;
  int numberOfTypes;             //! Number of hardware types
  int sosmIntegration;          //! Select resource allocation mechanism
  int* types;                    //! Hardware type
  int* numberOfResourcesPerType; //! Number of resources that correspond to each hardware type
  power* powerComp;
  netw* network;
  broker* brok;
  traditionalBroker* traditionalBrok;
  resource** resources; //! Two-dimensional array of computer resources (servers)
  stat* stats;          //! Array to keep cell statistics

 public:
  cell();

  /// Creates a cell based on user-supplied configuration
  /// \param setup Stores cell-related configuration from the CellData file
  cell(const cellinputs& setup, int L_sosmIntegration);

  cell(const cell& t);

  cell& operator=(const cell& t);

  ~cell();

  /// Calls broker::timestep to perform the simulation phase, update the state information and update cell statistics
  void timestep(const double& tstep);

  /// Deploys the tasks to the appropriate vRMs by calling recursively the broker::deploy method
  void deploy(list<task>* jobs);

  /// Updates cell-related statistics. The statistics are gathered per resource and summed.
  /// \param tstep The current time-step
  void updateStats(const double& tstep);

  void print();

  int gID() const;
  int galloc() const;
  int getNumberOfTypes() const;
  int getSosmIntegration() const;
  int* getTypes() const;
  int* getNumberOfResourcesPerType() const;
  traditionalBroker* getTraditionalBroker() const;
  resource** getResources() const;
  broker* getBroker() const;
  power* getPowerConsumption() const;
  netw* getNetwork() const;
  stat* getStats() const;
};

#endif
