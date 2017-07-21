#ifndef STAT_H
#define STAT_H
#include <fstream>
#include <string>
using namespace std;

struct stat {

  stat();

  stat(const stat& t);

  stat& operator=(const stat& t);

  ~stat();

  void print() const;

  void printfile(const string& outfile, const ios::openmode& mode);

  void printfileJson(const string& outfile, const string& inputfile, const ios::openmode& mode, int i, int j,
                     int overallRecords, int numOfCells, int numOfTypes, int ct);

  int procActServs;
  double memActServs;
  double stoActServs;
  int accActServs;
  int alloc;
  double currTstep;
  double phyMem;
  double phyProc;
  double phySto;
  double phyNetw;
  double totMem;
  double totProc;
  double availProc;
  double availMem;
  double utilProc;
  double utilMem;
  double totSto;
  double availSto;
  double utilSto;
  double totNetw;
  double availNetw;
  double utilNetw;
  double totPcons;

  int totAcc;
  int availAcc;
  int utilAcc;
  int activeSrvs;
  int numOfTasks;
  int rejTasks;
  int accTasks;

  double autilProc;
  double autilMem;
  double autilNetw;
};

#endif
