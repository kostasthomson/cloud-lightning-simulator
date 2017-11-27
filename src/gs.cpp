/* Copyright 2015 The CloudLightning Simulation Framework Authors. All
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

#include <gs.h>
#include <inputs.h> // for siminputs, cellinputs, appinputs, string
#include <stat.h>   // for stat
#include <sstream>  // for stringstream
#include <iostream>

using std::stringstream;

gs::gs() : alloc(0), ai(nullptr), si(nullptr), stats(nullptr) {}
gs::gs(const std::string& cellData, const std::string& appData, const std::string& brokerData)
{
  alloc = 1;
  ai = new appinputs[1];
  si = new siminputs[1];
  ai->parse(appData);
  si->parse(cellData, brokerData);
  stats = new stat*[si->numOfCells];
  for (int i = 0; i < si->numOfCells; i++) {
    stats[i] = new stat[si->cinp[i].numberOfTypes];
  }
}

gs::gs(const gs& t)
{
  if (t.galloc()) {
    alloc = 1;
    ai = new appinputs[1];
    si = new siminputs[1];
    ai[0] = t.gai()[0];
    si[0] = t.gsi()[0];
    stats = new stat*[si->numOfCells];
    for (int i = 0; i < si->numOfCells; i++) {
      stats[i] = new stat[si->cinp[i].numberOfTypes];
    }
    for (int i = 0; i < si->numOfCells; i++) {
      for (int j = 0; j < si->cinp[i].numberOfTypes; j++) {
        stats[i][j] = t.getStats()[i][j];
      }
    }
  }
}

gs& gs::operator=(const gs& t)
{
  if (this != &t) {
    if (alloc) {
      alloc = 0;
      for (int i = 0; i < si->numOfCells; i++) {
        delete[] stats[i];
      }
      delete[] stats;
      delete[] ai;
      delete[] si;
      stats = nullptr;
      ai = nullptr;
      si = nullptr;
    }
    alloc = t.galloc();
    if (alloc) {
      ai = new appinputs[1];
      si = new siminputs[1];
      ai[0] = t.gai()[0];
      si[0] = t.gsi()[0];
      stats = new stat*[si->numOfCells];
      for (int i = 0; i < si->numOfCells; i++) {
        stats[i] = new stat[si->cinp[i].numberOfTypes];
      }
      for (int i = 0; i < si->numOfCells; i++) {
        for (int j = 0; j < si->cinp[i].numberOfTypes; j++) {
          stats[i][j] = t.getStats()[i][j];
        }
      }
    }
  }
  return *this;
}

gs::~gs()
{
  if (alloc) {
    alloc = 0;
    for (int i = 0; i < si->numOfCells; i++) {
      delete[] stats[i];
    }
    delete[] stats;
    delete[] ai;
    delete[] si;
    stats = nullptr;
    ai = nullptr;
    si = nullptr;
  }
}

int gs::galloc() const { return alloc; }
appinputs* gs::gai() const { return ai; }
siminputs* gs::gsi() const { return si; }
stat** gs::getStats() const { return stats; }
int gs::findCell(const int* rImpl, const int& numImpl, const int& rVM, const double& rvProc, const double& rMem,
                 const double& rNet, const double& rSto, const int* rAcc) const
{
  int choice = -1, i, j, rind = -1, k, choice2 = -1;
  int frind = -1;
  double weight = 0.0, lweight = 0.0;

  // For each available cell
  for (i = 0; i < si->numOfCells; i++) {
    // For each hardware type appropriate for the task
    for (k = 0; k < numImpl; k++) {
      rind = -1;
      // For each hardware type that is offered by the cell
      for (j = 0; j < si->cinp[i].numberOfTypes; j++) {
        // If the cell offers a hardware type that is required by the task, select the first offer (usually CPU)
        if (rImpl[k] == si->cinp[i].types[j]) {
          rind = j;
        }
      }
      // If no cell offer matches a task requirement, discard task
      if (rind == -1) {
        continue;
      }
      // If stats[cell][hardware type] has the required number of processors, memory, storage and accelerators
      if (stats[i][rind].availableProcessors >= ((double)rVM) * rvProc &&
          stats[i][rind].availableMemory >= ((double)rVM) * rMem && stats[i][rind].availableNetwork >= rNet &&
          stats[i][rind].availableStorage >= ((double)rVM) * rSto &&
          stats[i][rind].availableAccelerators >= ((double)rVM) * rAcc[k]) {
        // Calculate max weight from the formula: total processors - requested processors
        lweight =
          ((stats[i][rind].availableProcessors - ((double)rVM) * rvProc) / (stats[i][rind].availableProcessors + 1) +
           (stats[i][rind].availableMemory - ((double)rVM) * rMem) / (stats[i][rind].availableMemory + 1) +
           (stats[i][rind].availableNetwork - rNet) / (stats[i][rind].availableNetwork + 1) +
           (stats[i][rind].availableStorage - ((double)rVM) * rSto) / (stats[i][rind].availableStorage + 1) +
           (stats[i][rind].availableAccelerators - ((double)rVM) * rAcc[k]) /
             (stats[i][rind].availableAccelerators + 1));
        if (lweight > weight) {
          weight = lweight;
          choice = i + 1;
          choice2 = k;
          frind = rind;
        }
      }
    }
  }
  if (frind != -1 && choice != -1) {
    stats[choice - 1][frind].availableProcessors -= ((double)rVM) * rvProc;
    stats[choice - 1][frind].availableMemory -= ((double)rVM) * rMem;
    stats[choice - 1][frind].availableNetwork -= rNet;
    stats[choice - 1][frind].availableStorage -= ((double)rVM) * rSto;
    stats[choice - 1][frind].availableAccelerators -= ((double)rVM) * rAcc[choice2];
  }

  return choice;
}

void gs::print()
{
  if (alloc) {
    si->print();
    ai->print();
  }
}

void gs::printfile(const std::string& outfile, const ios::openmode& mode)
{
  fstream file;
  if (alloc) {
    file.open(outfile.c_str(), mode);
    file.close();
    si->printfile(outfile, ios::out | ios::app);
    ai->printfile(outfile, ios::out | ios::app);
  }
}

std::string gs::num2str(int num)
{
  stringstream s;
  s << num;
  return s.str();
}

void gs::printStats()
{
  if (alloc) {
    for (int i = 0; i < si->numOfCells; i++) {
      for (int j = 0; j < si->cinp[i].numberOfTypes; j++) {
        stats[i][j].print();
      }
    }
  }
}

void gs::printStats(const std::string& outfile, const ios::openmode& mode)
{
  if (alloc) {
    for (int i = 0; i < si->numOfCells; i++) {
      for (int j = 0; j < si->cinp[i].numberOfTypes; j++) {
        std::string tmp = outfile + num2str(si->cinp[i].ID) + num2str(si->cinp[i].types[j]);
        stats[i][j].printfile(tmp, mode);
      }
    }
  }
}

// Print statistics to json, first reading data from txt output files and then creating the json
void gs::printStatsJson(const std::string& outfile, const ios::openmode& mode, int endTime, int updateInterval, int sosmIntegration, int allTasks)
{
  int overallRecords = (endTime / updateInterval) + 1;
  if (alloc) {
    for (int i = 0; i < si->numOfCells; i++) {
      for (int j = 0; j < si->cinp[i].numberOfTypes; j++) {
        std::string tmp = outfile + num2str(si->cinp[i].ID) + num2str(si->cinp[i].types[j]);
        std::string temp = outfile + "CLSim.json";
        stats[i][j].printfileJson(temp, tmp, mode, si->cinp[i].ID, si->cinp[i].types[j], overallRecords, si->numOfCells,
                                  si->cinp[i].numberOfTypes, j + 1, sosmIntegration, allTasks);
      }
    }
  }
}

// Print statistics to json directly, without reading first the txt output files
void gs::printStatsToJsonDirect(const std::string& outfile, const ios::openmode& mode, int endTime, int updateInterval, int sosmIntegration,
                                int allTasks, int currentTime)
{
  int timeStep = (currentTime+1)/updateInterval;
  int k=0;  //variable suitable in printfileJsonDirect for calculating the prooper location of the needed element in the array of json objects
  if (alloc) {
    for (int i = 0; i < si->numOfCells; i++) {
      for (int j = 0; j < si->cinp[i].numberOfTypes; j++) {
        //std::string tmp = outfile + num2str(si->cinp[i].ID) + num2str(si->cinp[i].types[j]);
        std::string temp = outfile + "CLSim.json";
        stats[i][j].printfileJsonDirect(temp, mode, si->cinp[i].ID, si->cinp[i].types[j], si->numOfCells,
                                  si->cinp[i].numberOfTypes, j + 1, sosmIntegration, allTasks, k, timeStep);
        k++;
      }
    }
  }
}