#ifndef CELL_H
#define CELL_H
#include <brokers.h>
#include <inputs.h>
#include <netw.h>
#include <power.h>
#include <resource.h>
#include <stat.h>
#include <task.h>
using namespace std;

class cell
{
 private:
  int ID;
  int alloc;
  int numOfTypes;             //! Number of hardware types
  int* types;                 //! Hardware type
  int* numOfResourcesPerType; //! Number of resources that correspond to each hardware type
  power* powerComp;
  netw* network;
  broker* brok;
  resource** resources; //! Two-dimensional array of computer resources (servers)
  stat* stats;          //! Array to keep cell statistics

 public:
  cell();

  /// Creates cell based on user-supplied configuration
  /// \param setup Stores cell-related configuration from the CellData file
  cell(const cellinputs& setup);

  cell(const cell& t);

  cell& operator=(const cell& t);

  ~cell();

  /// Calls broker::timestep to perform simulation phase, update state information and update cell statistics
  void timestep(const double& tstep);

  /// Deploys the tasks to the appropriate vRMs, by calling recursively the broker::deploy method
  void deploy(list<task>* jobs);

  /// Updates cell-related statistics. The statistics are gathered per resource and summed.
  /// \param tstep The current time-step
  void updateStats(const double& tstep);

  void print();

  int gID() const;
  int galloc() const;
  int gnumOfTypes() const;
  int* gtypes() const;
  int* gnumOfResourcesPerType() const;
  resource** gresources() const;
  broker* gbrok() const;
  power* gpowerComp() const;
  netw* gnetwork() const;
  stat* gstats() const;
};

#endif
