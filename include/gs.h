#ifndef GS_H
#define GS_H
#include <inputs.h>
#include <resource.h>
#include <task.h>
#include <power.h>
#include <netw.h>
#include <stat.h>

using namespace std;

template <typename T> string num2str(T num);

class gs 
{
    private:
	int alloc;
	appinputs *ai;
	siminputs *si;
	stat **stats;
    public:

        //---------------------------------
        //        CONSTRUCTORS
        //---------------------------------
	gs();
	gs(const string &sfile, const string &afile, const string &bfile);
 	gs(const gs & t);

        //---------------------------------
        //        ASSIGNMENT OPERATOR
        //---------------------------------
	gs & operator=(const gs & t);
  

        //---------------------------------
        //        DECONSTRUCTOR
        //---------------------------------        
    	~gs();

        //---------------------------------
        //        TIMESTEPPING
        //---------------------------------

	//---------------------------------
	//        FIND CELL
	//---------------------------------
	int findCell(const int *rImpl, const int &numImpl, const int &rVM, const double &rvProc,const double &rMem, const double &rNet, const double &rSto, const int *rAcc)  const;
 

        //---------------------------------
        //        GATEWAYS
        //---------------------------------
	int galloc() const;
	appinputs *gai() const;
	siminputs *gsi() const;
	stat **gstats() const;

        //---------------------------------
        //        UPDATE STATS
        //---------------------------------

        //---------------------------------
        //        PRINT
        //---------------------------------
	void print();
	void printStats();
	void printStats(const string &outfile, const ios::openmode &mode);
	void printfile(const string &outfile, const ios::openmode &mode);
	void printStatsJson(const string &outfile, const ios::openmode &mode, int endTime, int upInterval);
};

#endif
