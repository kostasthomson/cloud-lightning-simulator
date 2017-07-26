#include <cell.h>
#include <gs.h>
#include <inputs.h>
#include <netw.h>
#include <power.h>
#include <stat.h>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <json.hpp>
#include <sstream>
#include <vector>

gs::gs()
{
  alloc = 0;
  ai = NULL;
  si = NULL;
  stats = NULL;
}

gs::gs(const std::string& sfile, const std::string& afile, const std::string& bfile)
{
  int i;
  alloc = 1;
  ai = new appinputs[1];
  si = new siminputs[1];
  ai->parse(afile);
  si->parse(sfile, bfile);
  stats = new stat*[si->numOfCells];
  for (i = 0; i < si->numOfCells; i++) {
    stats[i] = new stat[si->cinp[i].numOfTypes];
  }
}

gs::gs(const gs& t)
{
  int i, j;
  if (t.galloc()) {
    alloc = 1;
    ai = new appinputs[1];
    si = new siminputs[1];
    ai[0] = t.gai()[0];
    si[0] = t.gsi()[0];
    stats = new stat*[si->numOfCells];
    for (i = 0; i < si->numOfCells; i++) {
      stats[i] = new stat[si->cinp[i].numOfTypes];
    }
    for (i = 0; i < si->numOfCells; i++) {
      for (j = 0; j < si->cinp[i].numOfTypes; j++) {
        stats[i][j] = t.gstats()[i][j];
      }
    }
  }
}

gs& gs::operator=(const gs& t)
{
  int i, j;
  if (this != &t) {
    if (alloc) {
      alloc = 0;
      for (i = 0; i < si->numOfCells; i++) {
        delete[] stats[i];
      }
      delete[] stats;
      delete[] ai;
      delete[] si;
      stats = NULL;
      ai = NULL;
      si = NULL;
    }
    alloc = t.galloc();
    if (alloc) {
      ai = new appinputs[1];
      si = new siminputs[1];
      ai[0] = t.gai()[0];
      si[0] = t.gsi()[0];
      stats = new stat*[si->numOfCells];
      for (i = 0; i < si->numOfCells; i++) {
        stats[i] = new stat[si->cinp[i].numOfTypes];
      }
      for (i = 0; i < si->numOfCells; i++) {
        for (j = 0; j < si->cinp[i].numOfTypes; j++) {
          stats[i][j] = t.gstats()[i][j];
        }
      }
    }
  }
  return *this;
}

gs::~gs()
{
  int i;
  if (alloc) {
    alloc = 0;
    for (i = 0; i < si->numOfCells; i++) {
      delete[] stats[i];
    }
    delete[] stats;
    delete[] ai;
    delete[] si;
    stats = NULL;
    ai = NULL;
    si = NULL;
  }
}

int gs::galloc() const { return alloc; }
appinputs* gs::gai() const { return ai; }
siminputs* gs::gsi() const { return si; }
stat** gs::gstats() const { return stats; }

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
      for (j = 0; j < si->cinp[i].numOfTypes; j++) {
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
      if (stats[i][rind].availProc >= ((double)rVM) * rvProc && stats[i][rind].availMem >= ((double)rVM) * rMem &&
          stats[i][rind].availNetw >= rNet && stats[i][rind].availSto >= ((double)rVM) * rSto &&
          stats[i][rind].availAcc >= ((double)rVM) * rAcc[k]) {
        // Calculate max weight from the formula: total processors - requested processors
        lweight = ((stats[i][rind].availProc - ((double)rVM) * rvProc) / (stats[i][rind].availProc + 1) +
                   (stats[i][rind].availMem - ((double)rVM) * rMem) / (stats[i][rind].availMem + 1) +
                   (stats[i][rind].availNetw - rNet) / (stats[i][rind].availNetw + 1) +
                   (stats[i][rind].availSto - ((double)rVM) * rSto) / (stats[i][rind].availSto + 1) +
                   (stats[i][rind].availAcc - ((double)rVM) * rAcc[k]) / (stats[i][rind].availAcc + 1));
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
    stats[choice - 1][frind].availProc -= ((double)rVM) * rvProc;
    stats[choice - 1][frind].availMem -= ((double)rVM) * rMem;
    stats[choice - 1][frind].availNetw -= rNet;
    stats[choice - 1][frind].availSto -= ((double)rVM) * rSto;
    stats[choice - 1][frind].availAcc -= ((double)rVM) * rAcc[choice2];
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
      for (int j = 0; j < si->cinp[i].numOfTypes; j++) {
        stats[i][j].print();
      }
    }
  }
}

void gs::printStats(const std::string& outfile, const ios::openmode& mode)
{
  if (alloc) {
    for (int i = 0; i < si->numOfCells; i++) {
      for (int j = 0; j < si->cinp[i].numOfTypes; j++) {
        std::string tmp = outfile + num2str(si->cinp[i].ID) + num2str(si->cinp[i].types[j]);
        stats[i][j].printfile(tmp, mode);
      }
    }
  }
}

void gs::printStatsJson(const std::string& outfile, const ios::openmode& mode, int endTime, int upInterval)
{
  int endT = endTime;
  int upIn = upInterval;
  int overallRecords = (endT / upIn) + 1;

  if (alloc) {
    for (int i = 0; i < si->numOfCells; i++) {
      for (int j = 0; j < si->cinp[i].numOfTypes; j++) {
        std::string tmp = outfile + num2str(si->cinp[i].ID) + num2str(si->cinp[i].types[j]);
        std::string temp = outfile + "CLsim.json";
        stats[i][j].printfileJson(temp, tmp, mode, si->cinp[i].ID, si->cinp[i].types[j], overallRecords, si->numOfCells,
                                  si->cinp[i].numOfTypes, j + 1);
      }
    }
  }
}