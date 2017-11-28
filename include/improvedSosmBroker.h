/* Copyright 2017 The CloudLightning Simulation Framework Authors. All
Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#ifndef IMPROVEDSOSMBROKER_H
#define IMPROVEDSOSMBROKER_H

#include <baseBroker.h>
#include <list>

class brinputs;
class cell;
class improvedpRouter;
class improvedpSwitch;
class siminputs;
class improvedvRM;

using std::list;

class improvedSosmBroker : public baseBroker
{
 private:
  int numberOfvRMs;
  int numberOfpSwitches;
  int numberOfpRouters;

  double pollIntervalCellM;
  double pollIntervalpRouter;
  double pollIntervalpSwitch;
  double pollIntervalvRM;

  double** sPMSA;
  double* SIs;
  double *Cs, *Ps, *Pis, *Caccs,*Paccs,*Piaccs;
  double* Ws;

  int numberOfFunctions;

  list<improvedvRM>** vRMs;
  list<improvedpSwitch>** pSwitches;
  list<improvedpRouter>** pRouters;

 public:
  improvedSosmBroker();

  /*sosmBroker(const int& L_numberOfTypes, const int* L_types, const int* L_numberOfResourcesPerType, resource**
     resources,
         power* powerComp, netw* network, const brinputs& binp);*/

  improvedSosmBroker(const improvedSosmBroker& t);

  improvedSosmBroker& operator=(const improvedSosmBroker& t);

  ~improvedSosmBroker();

  void init(const cell* clCell, const siminputs* si);

  void print() const;

  /// Calculates the de-assessment functions
  double deassessmentFunctions(const double& dNu, const double& dAcc, const int& choice, const int& type);

  /// Updates the state information of the cell, by calling recursively the pRouter::updateStateInfo method
  void updateStateInfo(const cell* clCell, const double& tstep);

  /// Deploys the tasks to the appropriate vRMs, by calling recursively the pRouter::deploy method
  void deploy(resource** resources, netw* network, stat* stats, task& _task);

  /// Performs the simulation phase
  void timestep(const cell* clCell);

  int getNumberOfvRMs() const;
  int getNumberOfpSwitches() const;
  int getNumberOfpRouters() const;

  double gpollIntervalCellM() const;
  double gpollIntervalpRouter() const;
  double gpollIntervalpSwitch() const;
  double gpollIntervalvRM() const;

  double** gsPMSA() const;
  double* gSIs() const;
  double* gCs() const;
  double* gPs() const;
  double* gPis() const;
  double* gWs() const;
  double *gCaccs() const;
  double *gPaccs() const;
  double *gPiaccs() const;

  int getNumberOfFunctions() const;

  list<improvedvRM>** getvRMs() const;
  list<improvedpSwitch>** gpSwitches() const;
  list<improvedpRouter>** gpRouters() const;
};

#endif
