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

#ifndef TASK_H
#define TASK_H

class task
{
 private:
  int type;
  int numberOfAvailableImplementations;
  int* availableImplementations;
  double requestedInstructions;
  int numberOfVMs;
  double* reqPMNS;
  int* typeactPMN;
  double** minmaxactPMN;
  int* avAcc;
  double* rhoAcc;
  int alloc;
  int* resourceIDs;

  double* cUtilPMNr;

 public:
  task();

  task(const task& t);

  task(const int& L_type, const int& L_numberOfAvailableImplementations, const int* L_availableImplementations,
       const double& L_requestedInstructions, const int& L_numberOfVMs, const double& L_reqP, const double& L_reqM,
       const double& L_reqN, const double& L_reqS, const int& L_typeactP, const int& L_typeactM, const int& L_typeactN,
       const double* L_minmaxactP, const double* L_minmaxactM, const double* L_minmaxactN, const int* L_avAcc,
       const double* L_rhoAcc);

  task& operator=(const task& t);

  ~task();

  void attachResources(const int* IDs);

  void detachResources();

  void compcUtilPMNr();

  void remapType(const int* type, const int& num);

  void reduceImpl(const int* type);

  void print() const;

  void reduceIns(const double& amount);
  int getType() const;
  int getNumberOfAvailableImplementations() const;
  int* getAvailableImplementations() const;
  double grequestedInstructions() const;
  int getNumberOfVMs() const;
  double* greqPMNS() const;
  int* getTypeactPMN() const;
  double** gminmaxactPMN() const;
  int* gavAcc() const;
  double* grhoAcc() const;
  int* gresourceIDs() const;
  double* gcUtilPMNr() const;
  int galloc() const;
  double getactP();
  double getactM();
  double getactN();
};

#endif
