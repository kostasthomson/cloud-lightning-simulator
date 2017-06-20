#ifndef GS_H
#define GS_H
#include <inputs.h>
#include <netw.h>
#include <power.h>
#include <resource.h>
#include <stat.h>
#include <task.h>

using namespace std;

template <typename T>
string num2str(T num);

class gs
{
 private:
  int alloc;
  appinputs* ai;
  siminputs* si;
  stat** stats;

 public:
  gs();

  /// Parse the simulator configuration files
  gs(const string& sfile, const string& afile, const string& bfile);

  gs(const gs& t);

  gs& operator=(const gs& t);

  ~gs();

  /// Retrieve the list of candidate cells for each task and select the most appropriate
  /// \param numImpl Number of implementations (hardware types)
  /// \param rVM Required number of VMs
  /// \param rvProc Required number of processing units
  /// \param rMem Required memory size
  /// \param rNet Required netowrk bandwidth
  /// \param rSto Required storage size
  /// \param rAcc Required accelerators
  int findCell(const int* rImpl, const int& numImpl, const int& rVM, const double& rvProc, const double& rMem,
               const double& rNet, const double& rSto, const int* rAcc) const;

  // Gateways
  int galloc() const;

  appinputs* gai() const;

  siminputs* gsi() const;

  stat** gstats() const;

  void print();

  void printStats();

  void printStats(const string& outfile, const ios::openmode& mode);

  void printfile(const string& outfile, const ios::openmode& mode);

  void printStatsJson(const string& outfile, const ios::openmode& mode, int endTime, int upInterval);
};

#endif
