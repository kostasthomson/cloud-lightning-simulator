#ifndef BROKERS_H
#define BROKERS_H
#include <resource.h>
#include <netw.h>
#include <power.h>
#include <stat.h>
#include <task.h>
#include <list>

class vRM
{
 private:
  int alloc;
  int numOfRes;
  int numOfFuncs;
  int optNumOfRes;
  double pollIntervalvRM;
  list<task>* queue;
  list<resource*>* res;
  double *Fs, *Ws;
  double* availProc;
  double* totProc;
  double* availMem;
  double* totMem;
  double* availAcc;
  double* totAcc;
  double* availSto;
  double* totSto;
  double* sPMSA;
  double C, P, Pi;
  double SI;
  int dep_strategy;

 public:
  vRM();

  ~vRM();

  vRM(const int& start, const int& end, const int& type, resource** resources, const double& L_pollIntervalvRM,
      const double& L_C, const double& L_P, const double& L_Pi, const int& L_optNumOfRes, const int& L_numOfFuncs,
      const double* L_Ws, const int& L_dep_strategy);

  vRM(const vRM& t);

  vRM& operator=(const vRM& t);

  /// Computes the assesment functions
  void computeFs();

  /// Computes the suitability index
  void computeSI();

  /// Updates the state information of the cell
  void updateStateInfo(const double& tstep);

  /// Removes the list of resources that satisfy the requested units from a vRM and adds them to the ores list
  /// \param ores The resources list to store the resources removed from the vRM
  /// \param remProc The number of requested processor cores
  /// \param remMem The size of requested memory
  /// \param remSto The size of requested storage
  /// \param remAcc The number of requested accelerators
  void obtainresources(list<resource*>& ores, double& remProc, double& remMem, double& remSto, double& remAcc);

  /// Adds the resources contained in the ores list to the vRM
  /// \param The resources list to be attached to the vRM
  void attachresources(list<resource*>& ores);

  double assessfuncs(const int& choice);

  double dassessfuncs(const double& dNu, const double& dNmem, const int& choice);

  /// Returns success if the task's processes, memory, storage and accelerator are less or equal than the vRM's
  int probe(const double& Proc, const double& Mem, const double& Sto, const int& Acc);

  /// Defines the strategies used to position VMs on resources
  int deploy_strategy(list<resource*>::iterator* it, int* IDs, const int& nVMs, const double& Proc, const double& Mem,
                      const double& Sto, const int& Acc);

  /// Deploys the tasks
  void deploy(resource** resources, netw* network, stat* stats, task* t);

  void enque(const task* t);

  void print();

  int galloc() const;
  int gnumOfRes() const;
  int goptNumOfRes() const;
  double gpollIntervalvRM() const;
  list<task>* gqueue() const;
  list<resource*>* gres() const;
  double* gavailProc() const;
  double* gtotProc() const;
  double* gavailMem() const;
  double* gtotMem() const;
  double* gavailAcc() const;
  double* gtotAcc() const;
  double* gavailSto() const;
  double* gtotSto() const;
  double* gsPMSA() const;
  double* gFs() const;
  double* gWs() const;
  double gC() const;
  double gP() const;
  double gPi() const;
  double gSI() const;
  int gdep_strategy() const;
  int gnumOfFuncs() const;
};

class pSwitch
{
 private:
  int alloc;
  int numOfvRMs;
  int numOfFuncs;
  double pollIntervalpSwitch;
  list<vRM*>* vRMs;
  double* Fs;
  double* Ws;
  double* availProc;
  double* totProc;
  double* availMem;
  double* totMem;
  double* availAcc;
  double* totAcc;
  double* availSto;
  double* totSto;
  double* sPMSA;
  double SI, *SIs;
  double C, P, Pi;

 public:
  pSwitch();

  ~pSwitch();

  pSwitch(const int& start, const int& end, const int& type, list<vRM>** LvRMs, const double& pollIntervalpSwitch,
          const double& L_C, const double& L_P, const double& L_Pi, const int& L_numOfFuncs, const double* L_Ws);

  pSwitch(const pSwitch& t);

  pSwitch& operator=(const pSwitch& t);

  /// Computes the assesment functions, by calling recursively the vRM::computeFs method
  void computeFs();

  /// Computes the suitability index
  void computeSI();

  /// Updates the state information of the cell, by calling recursively the vRM::updateStateInfo method
  void updateStateInfo(const double& tstep);

  double dassessfuncs(const double& dNu, const double& totNu, const double& dNmem, const double& totMem,
                      const int& choice);

  /// Returns success if the task's processes, memory, storage and accelerator are less or equal than the pSwitch's
  int probe(const double& Proc, const double& Mem, const double& Sto, const int& Acc);

  /// Deploys the tasks to the appropriate vRMs, by calling recursively the vRM::deploy method
  void deploy(resource** resources, netw* network, stat* stats, task* t);

  int galloc() const;
  int gnumOfvRMs() const;
  int gnumOfFuncs() const;
  double* gFs() const;
  double* gWs() const;
  double gpollIntervalpSwitch() const;
  list<vRM*>* gvRMs() const;
  double* gavailProc() const;
  double* gtotProc() const;
  double* gavailMem() const;
  double* gtotMem() const;
  double* gavailAcc() const;
  double* gtotAcc() const;
  double* gavailSto() const;
  double* gtotSto() const;
  double* gsPMSA() const;
  double gSI() const;
  double* gSIs() const;
  double gC() const;
  double gP() const;
  double gPi() const;
};

class pRouter
{
 private:
  int alloc;
  int numOfpSwitches;
  int numOfFuncs;
  double pollIntervalpRouter;
  list<pSwitch*>* pSwitches;
  double* Fs;
  double* Ws;
  double* availProc;
  double* totProc;
  double* availMem;
  double* totMem;
  double* availAcc;
  double* totAcc;
  double* availSto;
  double* totSto;
  double* sPMSA;
  double SI, *SIs;
  double C, P, Pi;

 public:
  pRouter();

  ~pRouter();

  pRouter(const int& start, const int& end, const int& type, list<pSwitch>** LpSwitches,
          const double& pollIntervalpRouter, const double& L_C, const double& L_P, const double& L_Pi,
          const int& L_numOfFuncs, const double* L_Ws);

  pRouter(const pRouter& t);

  pRouter& operator=(const pRouter& t);

  /// Updates the state information of the cell, by calling recursively the pSwitch::updateStateInfo method
  void updateStateInfo(const double& tstep);

  /// Deploys the tasks to the appropriate vRMs, by calling recursively the pSwitch::deploy method
  void deploy(resource** resources, netw* network, stat* stats, task* t);

  double dassessfuncs(const double& dNu, const double& totNu, const double& dNmem, const double& totMem,
                      const int& choice);

  /// Returns success if the task's processes, memory, storage and accelerator are less or equal than the pRouter's
  int probe(const double& Proc, const double& Mem, const double& Sto, const int& Acc);

  /// Computes the assesment functions, by calling recursively the pSwitch::computeFs method
  void computeFs();

  /// Computes the suitability index
  void computeSI();

  void print() const;

  int galloc() const;
  int gnumOfpSwitches() const;
  int gnumOfFuncs() const;
  double gpollIntervalpRouter() const;
  list<pSwitch*>* gpSwitches() const;
  double* gFs() const;
  double* gWs() const;
  double* gavailProc() const;
  double* gtotProc() const;
  double* gavailMem() const;
  double* gtotMem() const;
  double* gavailAcc() const;
  double* gtotAcc() const;
  double* gavailSto() const;
  double* gtotSto() const;
  double* gsPMSA() const;
  double gSI() const;
  double* gSIs() const;
  double gC() const;
  double gP() const;
  double gPi() const;
};

class broker
{
 private:
  int alloc;
  int numOfTypes;
  int* types;
  int* numOfResourcesPerType;
  int numOfvRMs;
  int numOfpSwitches;
  int numOfpRouters;
  double pollIntervalCellM;
  double pollIntervalpRouter;
  double pollIntervalpSwitch;
  double pollIntervalvRM;
  double** sPMSA;
  double* SIs;
  double *Cs, *Ps, *Pis;
  double* Ws;
  int numOfFuncs;
  double availNetw, totNetw;
  list<vRM>** vRMs;
  list<pSwitch>** pSwitches;
  list<pRouter>** pRouters;

 public:
  broker();

  void initbroker(const int& L_numOfTypes, const int* L_types, const int* L_numOfResourcesPerType, resource** resources,
                  power* powerComp, netw* network, const brinputs& binp);

  /*broker(const int& L_numOfTypes, const int* L_types, const int* L_numOfResourcesPerType, resource** resources,
         power* powerComp, netw* network, const brinputs& binp);*/

  broker(const broker& t);

  broker& operator=(const broker& t);

  ~broker();

  void print() const;

  /// Calculates the de-assessment functions
  double dassessfuncs(const double& dNu, const double& dNmem, const int& choice, const int& type);

  /// Updates the state information of the cell, by calling recursively the pRouter::updateStateInfo method
  void updateStateInfo(netw* network, const double& tstep);

  /// Deploys the tasks to the appropriate vRMs, by calling recursively the pRouter::deploy method
  void deploy(resource** resources, netw* network, stat* stats, task* t);

  /// Performs the simulation phase
  void timestep(resource** resources, netw* network, stat* stats, power* powerComp);

  int galloc() const;
  int gnumOfTypes() const;
  int* gtypes() const;
  int* gnumOfResourcesPerType() const;
  int gnumOfvRMs() const;
  int gnumOfpSwitches() const;
  int gnumOfpRouters() const;
  double gpollIntervalCellM() const;
  double gpollIntervalpRouter() const;
  double gpollIntervalpSwitch() const;
  double gpollIntervalvRM() const;
  double** gsPMSA() const;
  double* gSIs() const;
  double gavailNetw() const;
  double gtotNetw() const;
  double* gCs() const;
  double* gPs() const;
  double* gPis() const;
  double* gWs() const;
  int gnumOfFuncs() const;

  list<vRM>** gvRMs() const;
  list<pSwitch>** gpSwitches() const;
  list<pRouter>** gpRouters() const;
};

class brokerNOsosm
{
  private:
    int alloc;
    double pollInterval;
    int numOfTypes;
    int *types;
    int *numOfResourcesPerType;
    double **availProc;
    double **totProc;
    double **availMem;
    double **totMem;
    double **availAcc;
    double **totAcc;
    double **availSto;
    double **totSto;
    double availNetw;
    double totNetw;
    list<task> *queue;
  public:
    brokerNOsosm();
    brokerNOsosm(const int &L_numOfTypes, const int *L_types, const int *L_numOfResourcesPerType, const double & L_pollInterval);
    brokerNOsosm(const brokerNOsosm &t);
    brokerNOsosm & operator=(const brokerNOsosm & t);
    ~brokerNOsosm();

    int galloc() const;
    int gnumOfTypes() const;
    double gpollInterval() const;
    int *gtypes() const;
    int *gnumOfResourcesPerType() const;
    double **gavailProc() const;
    double **gtotProc() const;
    double **gavailMem() const;
    double **gtotMem() const;
    double **gavailAcc() const;
    double **gtotAcc() const;
    double **gavailSto() const;
    double **gtotSto() const;
    double gavailNetw() const;
    double gtotNetw() const;
    list<task>* gqueue() const;
    void print() const;
    void updateStateInfo(resource **resources, netw *network, const double &tstep);
    void deploy(resource **resources, netw *network, stat* stats, task * t);
    void enque(const task *t);
    void timestep(resource **resources, netw *network, stat *stats, power *powerComp);
};

#endif
