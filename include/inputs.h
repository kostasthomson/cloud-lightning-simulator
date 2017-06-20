#ifndef INPUTS_H
#define INPUTS_H
#include <string>
#include <fstream>
using namespace std;

struct brinputs
{
	int alloc;
	int numOfFuncs;
	double *Ws;
	int initResPervRM;
	int initvRMPerpSwitch;
	int initpSwitchPerpRouter;
	double pollIntervalCellM;
	double pollIntervalpRouter;
	double pollIntervalpSwitch;
	double pollIntervalvRM;
	int vRMdeploystrategy;
	brinputs();
	brinputs(const brinputs & t);
	brinputs & operator=(const brinputs & t);
	~brinputs();
	void parse(const string &outname, int cell_id);
	void print();
	void printfile(const string &outname, const ios::openmode &mode);
};

struct appinputs
{
		int alloc;
		double *minmaxJobsPerSec;
		int numOfApps;
		int *numOfAvailImplPerApp;
		int **availImplPerApp;
		int **minmaxVMPerApp;
		double **minmaxInsPerApp;
		double **minmaxProcPerVM;
		double **minmaxMemPerVM;
		double **minmaxStoPerVM;
		double **minmaxNetPerApp;
		int *typeOfActP;
		int *typeOfActM;
		int *typeOfActN;
		double **minmaxActP;
		double **minmaxActM;
		double **minmaxActN;
		int **accelerator;
		double **rhoAcc;
		void parse(const string &fname);
		void print();
		void printfile(const string &outname, const ios::openmode &mode);
		appinputs();
		~appinputs();
		appinputs & operator=(const appinputs & t);
		appinputs(const appinputs & t);
};

struct netinputs
{
    int alloc;
    double netBW;
    double overCommitNet;
    void print();
    netinputs();
    ~netinputs();
    netinputs & operator=(const netinputs & t);
    netinputs(const netinputs & t);
};

struct powinputs
{
    int alloc;
    int typeCpu;
    int typeAcc;
    double cpuPmin,cpuPmax,cpuC;
    int numOfPoints;
    double *cpubins,*cpuP;
    int accelerator;
    double accPmin,accPmax,accC;
    powinputs();
    powinputs(const powinputs & t);
    ~powinputs();
    powinputs & operator=(const powinputs & t);   
    
};

struct resinputs
{
    int alloc;
    double numOfProcUnits;
    double totMem;
    double totSto;
    double overCommitProc;
    double overCommitMem;
    double compCap;
    int accelerator;
    double accCompCap;
    int totAcc;
    int type;
    resinputs();
    resinputs(const resinputs & t);
    ~resinputs();
    resinputs & operator=(const resinputs & t);
};

struct cellinputs
{
    int alloc;
    int ID;
    struct resinputs *rinp;
    struct powinputs *pinp;
    struct netinputs *ninp;
    struct brinputs *binp;
    int numOfTypes;
    int *types;
    int *numOfResourcesPerType;
    cellinputs();
    cellinputs(const cellinputs & t);
    ~cellinputs();
    cellinputs & operator=(const cellinputs & t);
};

struct siminputs
{
    struct cellinputs *cinp;
    int alloc;
    int numOfCells;
    double maxTime;
    double upInterval;
    siminputs();
    siminputs(const siminputs & t);
    void parse(const string &fname, const string &bname);
    ~siminputs();
    siminputs & operator=(const siminputs & t);
    void print();
    void printfile(const string &outname, const ios::openmode &mode);
};

#endif
