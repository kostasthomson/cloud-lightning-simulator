#ifndef GS_H
#define GS_H

#include <inputs.h>
#include <string> // for string

class stat;

template <typename T>
std::string num2str(T num);

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
  /// \param sfile The path to the CellData configuration file
  /// \param afile The path to the AppData configuration file
  /// \param bfile The path to the BrokerData configuration file
  gs(const std::string& sfile, const std::string& afile, const std::string& bfile);

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

  std::string num2str(int num);

  // Gateways
  int galloc() const;

  appinputs* gai() const;

  siminputs* gsi() const;

  stat** getStats() const;

  void print();

  void printStats();

  void printStats(const std::string& outfile, const ios::openmode& mode);

  void printfile(const std::string& outfile, const ios::openmode& mode);

  void printStatsJson(const std::string& outfile, const ios::openmode& mode, int endTime, int updateInterval);
};

#endif
