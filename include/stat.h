#ifndef STAT_H
#define STAT_H
#include<string>
#include<fstream>
using namespace std;

struct stat 
{
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
	stat();
	stat(const stat &t);
	stat & operator=(const stat &t);
	~stat();
	void print() const;
	void printfile(const string &outfile, const ios::openmode &mode);
};

#endif
