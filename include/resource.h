#ifndef RESOURCE_H
#define RESOURCE_H
#include <inputs.h>
#include <task.h>
#include <iterator>
#include <list>
using namespace std;

class resource
{
 private:
  int alloc;
  int active;
  int movable;
  int type;
  int ID;

  double totalProc;
  double availProc;
  double utilProc;

  double totalMem;
  double availMem;
  double utilMem;

  double totalSto;
  double availSto;
  double utilSto;

  double physProc;
  double physMem;
  double physSto;

  double compCap;

  int accelerator;
  int totAcc;
  int availAcc;
  int utilAcc;
  double accCompCap;

  double overCommitProc;
  double overCommitMem;

  double autilProc;
  double autilMem;
  double arhoAcc;
  double cCompCapPerProc;
  double cCompCapPerAcc;

  int numOfTasks;

 public:

  resource();
  resource(const resinputs& setup, const int& iD);
  resource(const resource& t);

  resource& operator=(const resource& t);

  ~resource();

  /// Initializes the running quantities of a resource
  void initRunQuan();

  /// Calculates the compute capability of each processor
  void compcCompCapPerProc();

  /// Calculates the compute capability of each accelerator
  void compcCompCapPerAcc();

  /// Increments the running quantities of a resource by the provided values for processors, memory and accelerators
  void incrRunQuan(const double& uProc, const double& uMem, const double& rAcc);

  /// Assigns a task to a resource: Increments the number of tasks variable, reduces the resources' available
  /// processors, memory, storage and accelerators and calculates the utilized units
  /// \param t The task to be assigned to the resource
  void deploy(const task* t);

  int probe(const double& reqProc, const double& reqMem, const double& reqSto, const int& reqAcc);

  /// Dissociates a task from a resource by reversing the actions of the deploy method
  /// \param t The iterator for a list of tasks that points to the task to be dissociated
  void unload(const list<task>::iterator& t);

  int galloc() const;
  int gactive() const;
  int gmovable() const;
  int gtype() const;
  int gID() const;

  double gtotalProc() const;
  double gavailProc() const;
  double gutilProc() const;

  double gtotalMem() const;
  double gavailMem() const;
  double gutilMem() const;

  double gtotalSto() const;
  double gavailSto() const;
  double gutilSto() const;

  double gphysProc() const;
  double gphysMem() const;
  double gphysSto() const;

  double gcompCap() const;

  int gaccelerator() const;
  int gtotAcc() const;
  int gavailAcc() const;
  int gutilAcc() const;
  double gaccCompCap() const;

  double goverCommitProc() const;
  double goverCommitMem() const;

  double gautilProc() const;
  double gautilMem() const;
  double garhoAcc() const;

  double gcCompCapPerProc() const;
  double gcCompCapPerAcc() const;

  int gnumOfTasks() const;
  void print() const;
};

#endif
