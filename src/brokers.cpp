#include <brokers.h>
#include <resource.h>
#include <netw.h>
#include <iostream>
#include <task.h>
#include <stat.h>
#include <power.h>
#include <cmath>
#include <list>
#include <cstdlib>
#include <mpi.h>
#include <omp.h>
#define MY_MIN(a,b) ((a) <= (b) ? (a) : (b))
#define MY_MAX(a,b) ((a) >= (b) ? (a) : (b))
using namespace std;

pRouter::pRouter()
{
	alloc=0;
	numOfpSwitches=0;
	numOfFuncs=0;
	pollIntervalpRouter=0.0;
	pSwitches=NULL;
	Fs=NULL;
	Ws=NULL;
	availProc=NULL;
	totProc=NULL;
	availMem=NULL;
	totMem=NULL;
	availAcc=NULL;
	totAcc=NULL;
	availSto=NULL;
	totSto=NULL;
	sPMSA=NULL;
	SIs=NULL;
	SI=0.0;
	C=0.0;
	P=0.0;
	Pi=0.0;
}

pRouter::pRouter(const int &start, const int &end, const int &type, list<pSwitch> **LpSwitches,const double &L_pollIntervalpRouter, const double &L_C, const double &L_P, const double &L_Pi, const int &L_numOfFuncs,const double *L_Ws)
{
	int i;
	alloc=1;
	numOfpSwitches=end-start;
	numOfFuncs=L_numOfFuncs;
	pollIntervalpRouter=L_pollIntervalpRouter;
	list<pSwitch>::iterator it = LpSwitches[type]->begin();
	pSwitches=new list<pSwitch*>[1];
	for(i=0;i<start;i++) {
		it++;
	}
	for(i=start;i<end;i++)
	{
		pSwitches->push_back(&(*it));
		it++;
	}
	Ws=new double[numOfFuncs];
	Fs=new double[numOfFuncs];
	for(i=0;i<numOfFuncs;i++)
	{
		Ws[i]=L_Ws[i];
		Fs[i]=0.0;
	}
	availProc=new double[numOfpSwitches];
	totProc=new double[numOfpSwitches];
	availMem=new double[numOfpSwitches];
	totMem=new double[numOfpSwitches];
	availAcc=new double[numOfpSwitches];
	totAcc=new double[numOfpSwitches];
	availSto=new double[numOfpSwitches];
	totSto=new double[numOfpSwitches];
	SIs=new double[numOfpSwitches];
	SI=0.0;
	sPMSA=new double[8];
	C=L_C;
	P=L_P;
	Pi=L_Pi;
	updateStateInfo(0.0);
}

pRouter::pRouter(const pRouter &t)
{
	int i;
	if (t.galloc())
	{
		alloc=1;
		numOfpSwitches=t.gnumOfpSwitches();
		numOfFuncs=t.gnumOfFuncs();
		pollIntervalpRouter=t.gpollIntervalpRouter();
		pSwitches=new list<pSwitch*>[1];
		for(list<pSwitch*>::iterator it = t.gpSwitches()->begin(); it != t.gpSwitches()->end(); it++) {
			pSwitches->push_back(*it);
		}
		Ws=new double[numOfFuncs];
		Fs=new double[numOfFuncs];
		for(i=0;i<numOfFuncs;i++)
		{
			Ws[i]=t.gWs()[i];
			Fs[i]=t.gFs()[i];
		}
		availProc=new double[numOfpSwitches];
		totProc=new double[numOfpSwitches];
		availMem=new double[numOfpSwitches];
		totMem=new double[numOfpSwitches];
		availAcc=new double[numOfpSwitches];
		totAcc=new double[numOfpSwitches];
		availSto=new double[numOfpSwitches];
		totSto=new double[numOfpSwitches];
		SIs=new double[numOfpSwitches];
		for(i=0;i<numOfpSwitches;i++)
		{
			availProc[i]=t.gavailProc()[i];
			totProc[i]=t.gtotProc()[i];
			availMem[i]=t.gavailMem()[i];
			totMem[i]=t.gtotMem()[i];
			availSto[i]=t.gavailSto()[i];
			totSto[i]=t.gtotSto()[i];
			availAcc[i]=t.gavailAcc()[i];
			totAcc[i]=t.gtotAcc()[i];
			SIs[i]=t.gSIs()[i];
		}
		SI=t.gSI();
		sPMSA=new double[8];
		for(i=0;i<8;i++) {
			sPMSA[i]=t.gsPMSA()[i];
		}
		C=t.gC();
		P=t.gP();
		Pi=t.gPi();
	}
}

pRouter & pRouter::operator=(const pRouter & t)
{
	int i;
    if (this!=&t)
    {
		if (alloc)
		{
			alloc=0;
			numOfpSwitches=0;
			numOfFuncs=0;
			pollIntervalpRouter=0.0;
			pSwitches->clear();
			delete[] pSwitches;
			pSwitches=NULL;
			delete[] Ws;
			delete[] Fs;
			Ws=NULL;
			Fs=NULL;
			delete[] availProc;
			delete[] totProc;
			delete[] availMem;
			delete[] totMem;
			delete[] availSto;
			delete[] totSto;
			delete[] availAcc;
			delete[] totAcc;
			delete[] sPMSA;
			delete[] SIs;
			availProc=NULL;
			totProc=NULL;
			availMem=NULL;
			totMem=NULL;
			availAcc=NULL;
			totAcc=NULL;
			availSto=NULL;
			totSto=NULL;
			sPMSA=NULL;
			SIs=NULL;
			SI=0.0;
			C=0.0;
			P=0.0;
			Pi=0.0;
		}
		alloc=t.galloc();
		if (alloc)
		{
			numOfpSwitches=t.gnumOfpSwitches();
			numOfFuncs=t.gnumOfFuncs();
			pollIntervalpRouter=t.gpollIntervalpRouter();
			pSwitches=new list<pSwitch*>[1];
			for(list<pSwitch*>::iterator it = t.gpSwitches()->begin(); it != t.gpSwitches()->end(); it++)
				pSwitches->push_back(*it);
			Ws=new double[numOfFuncs];
			Fs=new double[numOfFuncs];
			for(i=0;i<numOfFuncs;i++)
			{
				Ws[i]=t.gWs()[i];
				Fs[i]=t.gFs()[i];
			}
			availProc=new double[numOfpSwitches];
			totProc=new double[numOfpSwitches];
			availMem=new double[numOfpSwitches];
			totMem=new double[numOfpSwitches];
			availAcc=new double[numOfpSwitches];
			totAcc=new double[numOfpSwitches];
			availSto=new double[numOfpSwitches];
			totSto=new double[numOfpSwitches];
			SIs=new double[numOfpSwitches];
			for(i=0;i<numOfpSwitches;i++)
			{
				availProc[i]=t.gavailProc()[i];
				totProc[i]=t.gtotProc()[i];
				availMem[i]=t.gavailMem()[i];
				totMem[i]=t.gtotMem()[i];
				availSto[i]=t.gavailSto()[i];
				totSto[i]=t.gtotSto()[i];
				availAcc[i]=t.gavailAcc()[i];
				totAcc[i]=t.gtotAcc()[i];
				SIs[i]=t.gSIs()[i];
			}
			SI=t.gSI();
			sPMSA=new double[8];
			for(i=0;i<8;i++)
				sPMSA[i]=t.gsPMSA()[i];
			C=t.gC();
			P=t.gP();
			Pi=t.gPi();
		}
	}
	return *this;
}

pRouter::~pRouter()
{
	if(alloc)
	{
		alloc=0;
		numOfpSwitches=0;
		numOfFuncs=0;
		pollIntervalpRouter=0.0;
		pSwitches->clear();
		delete[] pSwitches;
		pSwitches=NULL;
		delete[] Ws;
		delete[] Fs;
		Ws=NULL;
		Fs=NULL;
		delete[] availProc;
		delete[] totProc;
		delete[] availMem;
		delete[] totMem;
		delete[] availSto;
		delete[] totSto;
		delete[] availAcc;
		delete[] totAcc;
		delete[] sPMSA;
		delete[] SIs;
		availProc=NULL;
		totProc=NULL;
		availMem=NULL;
		totMem=NULL;
		availAcc=NULL;
		totAcc=NULL;
		availSto=NULL;
		totSto=NULL;
		sPMSA=NULL;
		SIs=NULL;
		SI=0.0;
		C=0.0;
		P=0.0;
		Pi=0.0;
	}
}

void pRouter::computeFs()
{
	int i,j;
	if(alloc)
	{
		for(i=0;i<numOfFuncs;i++) {
			Fs[i]=0.0;
		}
		list<pSwitch*>::iterator it=pSwitches->begin();
		for(i=0;i<numOfpSwitches;i++)
		{
			for(j=0;j<numOfFuncs;j++)
			{
				Fs[j]+=(*it)->gFs()[j];
			}
			it++;
		}
		for(j=0;j<numOfFuncs;j++) {
			Fs[j]/=numOfpSwitches;
		}
	}
}

void pRouter::computeSI()
{
	int i;
	if (alloc)
	{
		SI=(1e-4)*(((double)rand())/RAND_MAX);
		for(i=0;i<numOfFuncs;i++) {
			SI+=Ws[i]*Fs[i];
		}
	}
}

int pRouter::probe(const double &Proc, const double &Mem, const double &Sto, const int &Acc)
{
	if (Proc<=sPMSA[0] && Mem<=sPMSA[2] && Sto<=sPMSA[4] && Acc<=(int)sPMSA[6]) {
		return 1;
	}
	else {
		return -1;
	}
}

void pRouter::deploy(resource **resources, netw *network, stat* stats, task * t)
{
	if (alloc)
	{
		int L_numOfVMs=t->gnumOfVMs();
		double *L_reqPMNS=t->greqPMNS();
		double maxSI=0.0,ssum=0.0,*maxSIs;
		int L_avAcc=t->gavAcc()[0];
		int omp_thr=atoi(getenv("OMP_NUM_THREADS"));
		int i,tid;
		int choice=-1,*choices;

		list<pSwitch*>::iterator itf;
		choices=new int[omp_thr];
		maxSIs=new double[omp_thr];

		// Initialize the choices and maxSIs arrays
		for(i=0;i<omp_thr;i++)
		{
			choices[i]=-1;
			maxSIs[i]=0.0;
		}

		// The total requested units = number of task VMs * task units
		double reqProc=L_numOfVMs*L_reqPMNS[0];
		double reqMem=L_numOfVMs*L_reqPMNS[1];
		double reqSto=L_numOfVMs*L_reqPMNS[3];
		int reqAcc=L_numOfVMs*L_avAcc;

		// Partition the list of pSwithces per thread. Then, for every pSwitch, determine their maximum SI. Set the
		// index number of the switch with the maximum SI in the array choices[thread_number]
		#pragma omp parallel default(shared) private(i,tid) num_threads(omp_thr)
		{
			tid=omp_get_thread_num();
			#pragma omp for
			for(i=0;i<numOfpSwitches;i++)
			{
				if(maxSIs[tid]<SIs[i] && reqProc<=availProc[i] && reqMem<=availMem[i] && reqSto<=availSto[i] && reqAcc<=availAcc[i])
				{
					maxSIs[tid]=SIs[i];
					choices[tid]=i;
				}
			}
		}

		choice=choices[0];
		maxSI=maxSIs[0];

		// From the list of pSwitches with the maximum SI per thread, selected the highest for the task deployment
		for(i=1;i<omp_thr;i++)
		{
			if(maxSI<maxSIs[i])
			{
				maxSI=maxSIs[i];
				choice=choices[i];
			}
		}
		delete[] choices;
		delete[] maxSIs;

		// If no suitable pSwitch was found, reject the task
		if (choice==-1)
		{
			stats[t->gavailImpl()[0]].rejTasks++;
			return;
		}

		// Set the pSwitch iterator to the position of the selected pSwitch
		itf=pSwitches->begin();
		for(i=0;i<choice;i++) {
			itf++;
		}

		// Reduce the task's units from the list of available units
		availProc[choice]-=reqProc;
		availMem[choice]-=reqMem;
		availSto[choice]-=reqSto;
		availAcc[choice]-=reqAcc;
		sPMSA[0]-=reqProc;
		sPMSA[2]-=reqMem;
		sPMSA[4]-=reqSto;
		sPMSA[6]-=(double)reqAcc;

		// Re-compute the assessment functions
		if (reqAcc>0) {
			for(i=0;i<4;i++)
			{
				ssum+=Ws[i]*dassessfuncs(-(double)reqAcc,sPMSA[7],-reqMem,sPMSA[3],i);
			}
		}
		else {
			for(i=0;i<4;i++)
			{
				ssum+=Ws[i]*dassessfuncs(-reqProc,sPMSA[1],-reqMem,sPMSA[3],i);
			}
		}
		SI+=ssum;
		ssum=0.0;
		if (sPMSA[7]>0) {
			for(i=0;i<4;i++)
			{
				ssum+=Ws[i]*dassessfuncs(-(double)reqAcc,(double)totAcc[choice],-reqMem,totMem[choice],i);
			}
		}
		else {
			for(i=0;i<4;i++)
			{
				ssum+=Ws[i]*dassessfuncs(-reqProc,totProc[choice],-reqMem,totMem[choice],i);
			}
		}
		// Re-compute the suitability index
		SIs[choice]+=ssum;

		// Call the pSwitch::deploy method recursively
		(*itf)->deploy(resources,network,stats,t);
	}
}

void pRouter::updateStateInfo(const double &tstep)
{
	int i;
	if(alloc)
	{
		// On every time interval
		if(((int)tstep%(int)pollIntervalpRouter)==0)
		{
			int len=(int)pSwitches->size();
			for(i=0;i<8;i++) {
				sPMSA[i]=0.0;
			}
			i=0;
			for(list<pSwitch*>::iterator it=pSwitches->begin(); it!=pSwitches->end();it++)
			{
				availProc[i]=((*it)->gsPMSA())[0];
				totProc[i]=((*it)->gsPMSA())[1];
				availMem[i]=((*it)->gsPMSA())[2];
				totMem[i]=((*it)->gsPMSA())[3];
				availSto[i]=((*it)->gsPMSA())[4];
				totSto[i]=((*it)->gsPMSA())[5];
				availAcc[i]=((*it)->gsPMSA())[6];
				totAcc[i]=((*it)->gsPMSA())[7];
				SIs[i]=((*it)->gSI());
				i++;
			}

			for(i=0;i<len;i++)
			{
				sPMSA[0]+=availProc[i];
				sPMSA[1]+=totProc[i];
				sPMSA[2]+=availMem[i];
				sPMSA[3]+=totMem[i];
				sPMSA[4]+=availSto[i];
				sPMSA[5]+=totSto[i];
				sPMSA[6]+=availAcc[i];
				sPMSA[7]+=totAcc[i];
			}
			computeFs();
			computeSI();
		}
	}
}


// Derivatives used for Taylor expansions (principal linear part)
double pRouter::dassessfuncs(const double &dNu,const double &totNu, const double &dNmem, const double &totMem,const int &choice)
{
	switch (choice)
	{
		case 0:
			return (dNu*C/totNu);
			break;
		case 1:
			return (dNmem/totMem);
			break;
		case 2:
			return (dNu*Pi*P*totNu)/((P*(totNu-dNu)+Pi*dNu)*(P*(totNu-dNu)+Pi*dNu));
			break;
		case 3:
			return (0.2*dNu/(totNu));
			break;
		default:
			return 0.0;
	}
}

int pRouter::galloc() const
{
	return alloc;
}

int pRouter::gnumOfpSwitches() const
{
	return numOfpSwitches;
}

int pRouter::gnumOfFuncs() const
{
	return numOfFuncs;
}

double pRouter::gpollIntervalpRouter() const
{
	return pollIntervalpRouter;
}

list<pSwitch*> *pRouter::gpSwitches() const
{
	return pSwitches;
}

double *pRouter::gavailProc() const
{
	return availProc;
}

double *pRouter::gtotProc() const
{
	return totProc;
}

double *pRouter::gavailMem() const
{
	return availMem;
}

double *pRouter::gtotMem() const
{
	return totMem;
}

double *pRouter::gavailAcc() const
{
	return availAcc;
}

double *pRouter::gtotAcc() const
{
	return totAcc;
}

double *pRouter::gavailSto() const
{
	return availSto;
}

double *pRouter::gtotSto() const
{
	return totSto;
}

double *pRouter::gsPMSA() const
{
	return sPMSA;
}

double *pRouter::gWs() const
{
	return Ws;
}

double *pRouter::gFs() const
{
	return Fs;
}

double pRouter::gSI() const
{
	return SI;
}

double *pRouter::gSIs() const
{
	return SIs;
}

double pRouter::gC() const
{
	return C;
}

double pRouter::gP() const
{
	return P;
}

double pRouter::gPi() const
{
	return Pi;
}


void pRouter::print() const
{
	if (alloc)
	{
		cout<<"pRouter Stats"<<endl;
		cout<<"Available Processing Units: "<<sPMSA[0]<<endl;
		cout<<"Total Processing Units: "<<sPMSA[1]<<endl;
		cout<<"Available Memory: "<<sPMSA[2]<<endl;
		cout<<"Total Memory: "<<sPMSA[3]<<endl;
		cout<<"Available Storage: "<<sPMSA[4]<<endl;
		cout<<"Total Storage: "<<sPMSA[5]<<endl;
		cout<<"Available Accelerators: "<<sPMSA[6]<<endl;
		cout<<"Total Accelerators: "<<sPMSA[7]<<endl;
	}
}

pSwitch::pSwitch()
{
	alloc=0;
	numOfvRMs=0;
	numOfFuncs=0;
	pollIntervalpSwitch=0.0;
	vRMs=NULL;
	Fs=NULL;
	Ws=NULL;
	availProc=NULL;
	totProc=NULL;
	availMem=NULL;
	totMem=NULL;
	availAcc=NULL;
	totAcc=NULL;
	availSto=NULL;
	totSto=NULL;
	sPMSA=NULL;
	SIs=NULL;
	SI=0.0;
	C=0.0;
	P=0.0;
	Pi=0.0;
}

pSwitch::pSwitch(const int &start, const int &end, const int &type, list<vRM> **LvRMs,const double &L_pollIntervalpSwitch, const double &L_C, const double &L_P, const double &L_Pi, const int &L_numOfFuncs,const double *L_Ws)
{
	int i;
	alloc=1;
	numOfvRMs=end-start;
	numOfFuncs=L_numOfFuncs;
	pollIntervalpSwitch=L_pollIntervalpSwitch;
	list<vRM>::iterator it = LvRMs[type]->begin();
	vRMs=new list<vRM*>[1];
	for(i=0;i<start;i++) {
		it++;
	}
	for(i=start;i<end;i++)
	{
		vRMs->push_back(&(*it));
		it++;
	}
	Ws=new double[numOfFuncs];
	Fs=new double[numOfFuncs];
	for(i=0;i<numOfFuncs;i++)
	{
		Ws[i]=L_Ws[i];
		Fs[i]=0.0;
	}
	availProc=new double[numOfvRMs];
	totProc=new double[numOfvRMs];
	availMem=new double[numOfvRMs];
	totMem=new double[numOfvRMs];
	availAcc=new double[numOfvRMs];
	totAcc=new double[numOfvRMs];
	availSto=new double[numOfvRMs];
	totSto=new double[numOfvRMs];
	SIs=new double[numOfvRMs];
	for(i=0;i<numOfvRMs;i++)
	{
		availProc[i]=0.0;
		totProc[i]=0.0;
		availMem[i]=0.0;
		totMem[i]=0.0;
		availAcc[i]=0.0;
		totAcc[i]=0.0;
		availSto[i]=0.0;
		totSto[i]=0.0;
		SIs[i]=0.0;

	}
	SI=0.0;
	sPMSA=new double[8];
	for(i=0;i<8;i++) {
		sPMSA[i]=0.0;
	}
	C=L_C;
	P=L_P;
	Pi=L_Pi;
	updateStateInfo(0.0);
}

pSwitch::pSwitch(const pSwitch &t)
{
	int i;
	if (t.galloc())
	{
		alloc=1;
		numOfvRMs=t.gnumOfvRMs();
		numOfFuncs=t.gnumOfFuncs();
		pollIntervalpSwitch=t.gpollIntervalpSwitch();
		vRMs=new list<vRM*>[1];
		for(list<vRM*>::iterator it = t.gvRMs()->begin(); it != t.gvRMs()->end(); it++)
		{
			vRMs->push_back(*it);
		}
		Ws=new double[numOfFuncs];
		Fs=new double[numOfFuncs];
		for(i=0;i<numOfFuncs;i++)
		{
			Ws[i]=t.gWs()[i];
			Fs[i]=t.gFs()[i];
		}
		availProc=new double[numOfvRMs];
		totProc=new double[numOfvRMs];
		availMem=new double[numOfvRMs];
		totMem=new double[numOfvRMs];
		availAcc=new double[numOfvRMs];
		totAcc=new double[numOfvRMs];
		availSto=new double[numOfvRMs];
		totSto=new double[numOfvRMs];
		SIs=new double[numOfvRMs];
		for(i=0;i<numOfvRMs;i++)
		{
			availProc[i]=t.gavailProc()[i];
			totProc[i]=t.gtotProc()[i];
			availMem[i]=t.gavailMem()[i];
			totMem[i]=t.gtotMem()[i];
			availSto[i]=t.gavailSto()[i];
			totSto[i]=t.gtotSto()[i];
			availAcc[i]=t.gavailAcc()[i];
			totAcc[i]=t.gtotAcc()[i];
			SIs[i]=t.gSIs()[i];
		}
		SI=t.gSI();
		sPMSA=new double[8];
		for(i=0;i<8;i++) {
			sPMSA[i]=t.gsPMSA()[i];
		}
		C=t.gC();
		P=t.gP();
		Pi=t.gPi();
	}
}

pSwitch & pSwitch::operator=(const pSwitch & t)
{
	int i;
    if (this!=&t)
    {
		if (alloc)
		{
			alloc=0;
			numOfvRMs=0;
			numOfFuncs=0;
			pollIntervalpSwitch=0.0;
			vRMs->clear();
			delete[] vRMs;
			vRMs=NULL;
			delete[] Ws;
			delete[] Fs;
			Ws=NULL;
			Fs=NULL;
			delete[] availProc;
			delete[] totProc;
			delete[] availMem;
			delete[] totMem;
			delete[] availSto;
			delete[] totSto;
			delete[] availAcc;
			delete[] totAcc;
			delete[] sPMSA;
			delete[] SIs;
			availProc=NULL;
			totProc=NULL;
			availMem=NULL;
			totMem=NULL;
			availAcc=NULL;
			totAcc=NULL;
			availSto=NULL;
			totSto=NULL;
			sPMSA=NULL;
			SIs=NULL;
			SI=0.0;
			C=0.0;
			P=0.0;
			Pi=0.0;
		}
		alloc=t.galloc();
		if (alloc)
		{
			numOfvRMs=t.gnumOfvRMs();
			numOfFuncs=t.gnumOfFuncs();
			pollIntervalpSwitch=t.gpollIntervalpSwitch();
			vRMs=new list<vRM*>[1];
			for(list<vRM*>::iterator it = t.gvRMs()->begin(); it != t.gvRMs()->end(); it++) {
				vRMs->push_back(*it);
			}
			Ws=new double[numOfFuncs];
			Fs=new double[numOfFuncs];
			for(i=0;i<numOfFuncs;i++)
			{
				Ws[i]=t.gWs()[i];
				Fs[i]=t.gFs()[i];
			}
			availProc=new double[numOfvRMs];
			totProc=new double[numOfvRMs];
			availMem=new double[numOfvRMs];
			totMem=new double[numOfvRMs];
			availAcc=new double[numOfvRMs];
			totAcc=new double[numOfvRMs];
			availSto=new double[numOfvRMs];
			totSto=new double[numOfvRMs];
			SIs=new double[numOfvRMs];
			for(i=0;i<numOfvRMs;i++)
			{
				availProc[i]=t.gavailProc()[i];
				totProc[i]=t.gtotProc()[i];
				availMem[i]=t.gavailMem()[i];
				totMem[i]=t.gtotMem()[i];
				availSto[i]=t.gavailSto()[i];
				totSto[i]=t.gtotSto()[i];
				availAcc[i]=t.gavailAcc()[i];
				totAcc[i]=t.gtotAcc()[i];
				SIs[i]=t.gSIs()[i];

			}
			SI=t.gSI();
			sPMSA=new double[8];
			for(i=0;i<8;i++) {
				sPMSA[i]=t.gsPMSA()[i];
			}
			C=t.gC();
			P=t.gP();
			Pi=t.gPi();

		}
	}
	return *this;
}

pSwitch::~pSwitch()
{
	if(alloc)
	{
		alloc=0;
		numOfvRMs=0;
		numOfFuncs=0;
		pollIntervalpSwitch=0.0;
		vRMs->clear();
		delete[] vRMs;
		delete[] Fs;
		delete[] Ws;
		vRMs=NULL;
		Fs=NULL;
		Ws=NULL;
		delete[] availProc;
		delete[] totProc;
		delete[] availMem;
		delete[] totMem;
		delete[] availSto;
		delete[] totSto;
		delete[] availAcc;
		delete[] totAcc;
		delete[] sPMSA;
		delete[] SIs;
		availProc=NULL;
		totProc=NULL;
		availMem=NULL;
		totMem=NULL;
		availAcc=NULL;
		totAcc=NULL;
		availSto=NULL;
		totSto=NULL;
		sPMSA=NULL;
		SIs=NULL;
		SI=0.0;
		P=0.0;
		C=0.0;
		Pi=0.0;
	}
}

void pSwitch::computeFs()
{
	int i,j;
	if(alloc)
	{
		for(i=0;i<numOfFuncs;i++)
			Fs[i]=0.0;
		list<vRM*>::iterator it=vRMs->begin();
		for(i=0;i<numOfvRMs;i++)
		{
			for(j=0;j<numOfFuncs;j++)
			{
				Fs[j]+=(*it)->gFs()[j];
			}
			it++;
		}
		for(j=0;j<numOfFuncs;j++)
			Fs[j]/=numOfvRMs;
	}
}

void pSwitch::computeSI()
{
	int i;
	if(alloc)
	{
		SI=(1e-4)*(((double)rand())/RAND_MAX);
		for(i=0;i<numOfFuncs;i++)
			SI+=Ws[i]*Fs[i];
	}
}

void pSwitch::updateStateInfo(const double &tstep)
{
	int i;
	if(alloc)
	{
		// On every time interval
		if(((int)tstep%(int)pollIntervalpSwitch)==0)
		{
			for(i=0;i<8;i++) {
				sPMSA[i]=0.0;
			}
			i=0;
			for(list<vRM*>::iterator it=vRMs->begin(); it!=vRMs->end();it++)
			{
				availProc[i]=((*it)->gsPMSA())[0];
				totProc[i]=((*it)->gsPMSA())[1];
				availMem[i]=((*it)->gsPMSA())[2];
				totMem[i]=((*it)->gsPMSA())[3];
				availSto[i]=((*it)->gsPMSA())[4];
				totSto[i]=((*it)->gsPMSA())[5];
				availAcc[i]=((*it)->gsPMSA())[6];
				totAcc[i]=((*it)->gsPMSA())[7];
				SIs[i]=((*it)->gSI());
				i++;
			}

			for(i=0;i<(int)vRMs->size();i++)
			{
				sPMSA[0]+=availProc[i];
				sPMSA[1]+=totProc[i];
				sPMSA[2]+=availMem[i];
				sPMSA[3]+=totMem[i];
				sPMSA[4]+=availSto[i];
				sPMSA[5]+=totSto[i];
				sPMSA[6]+=availAcc[i];
				sPMSA[7]+=totAcc[i];
			}
			computeFs();
			computeSI();
		}
	}
}

double pSwitch::dassessfuncs(const double &dNu,const double &totNu, const double &dNmem, const double &totMem,const int &choice)
{
	switch (choice)
	{
		case 0:
			return (dNu*C/totNu);
			break;
		case 1:
			return (dNmem/totMem);
			break;
		case 2:
			return (dNu*Pi*P*totNu)/((P*(totNu-dNu)+Pi*dNu)*(P*(totNu-dNu)+Pi*dNu));
			break;
		case 3:
			return (0.2*dNu/(totNu));
			break;
		default:
			return 0.0;
	}
}

int pSwitch::probe(const double &Proc, const double &Mem, const double &Sto, const int &Acc)
{
	if (Proc<=sPMSA[0] && Mem<=sPMSA[2] && Sto<=sPMSA[4] && Acc<=(int)sPMSA[6]) {
		return 1;
	}
	else {
		return -1;
	}
}

void pSwitch::deploy(resource **resources, netw *network, stat* stats, task * t)
{
	if (alloc)
	{
		int L_numOfVMs=t->gnumOfVMs();
		double *L_reqPMNS=t->greqPMNS();
		double maxSI=0.0,ssum=0.0,*maxSIs;
		int L_avAcc=t->gavAcc()[0];
		int omp_thr=atoi(getenv("OMP_NUM_THREADS"));
		int i,tid,j;
		int choice=-1,*choices;
		list<vRM*>::iterator itt=vRMs->begin(),itf,itff;
		choices=new int[omp_thr];
		maxSIs=new double[omp_thr];

		// Initialize the choices and maxSIs arrays
		for(i=0;i<omp_thr;i++)
		{
			choices[i]=-1;
			maxSIs[i]=0.0;
		}

		// The total requested units = number of task VMs * task units
		double reqProc=L_numOfVMs*L_reqPMNS[0];
		double reqMem=L_numOfVMs*L_reqPMNS[1];
		double reqSto=L_numOfVMs*L_reqPMNS[3];
		int reqAcc=L_numOfVMs*L_avAcc;

		// Partition the list of vRMs per thread. Then, for every vRM, determine their maximum SI. Set the
        // index number of the vRM with the maximum SI in the array choices[thread_number]
		#pragma omp parallel default(shared) private(i,tid) num_threads(omp_thr)
		{
			tid=omp_get_thread_num();
			for(i=(tid*numOfvRMs)/omp_thr;i<((tid+1)*numOfvRMs)/omp_thr;i++)
			{
				if(maxSIs[tid]<SIs[i] && reqProc<=availProc[i] && reqMem<=availMem[i] && reqSto<=availSto[i] && reqAcc<=availAcc[i])
				{
					maxSIs[tid]=SIs[i];
					choices[tid]=i;
				}
			}
		}

		choice=choices[0];
		maxSI=maxSIs[0];

		// From the list of vRMs with the maximum SI per thread, selected the highest for the task deployment
		for(i=1;i<omp_thr;i++)
		{
			if(maxSI<maxSIs[i])
			{
				maxSI=maxSIs[i];
				choice=choices[i];
			}
		}

		// If no suitable vRM was found, begin Self-Organization and reorganize resources
		// First step, locate the vRM with the highest suitability index (lets call this chosenvRM)
		// Then, move resources from vRMs 0 - (chosenvRM - 1) to chosenvRM
		// If there are still additional resources needed to satisfy the task, move them from vRMs
		// (chosenvRM + 1) - numOfvRMs to chosenvRM
		// If there are still additional resources needed, reject the task
		if (choice==-1)
		{
			for(i=0;i<omp_thr;i++)
			{
				choices[i]=-1;
				maxSIs[i]=0.0;
			}
			//First, find a suitable vRM even if it does not satisfy all the task's requested units
			#pragma omp parallel default(shared) private(i,tid) num_threads(omp_thr)
			{
				tid=omp_get_thread_num();
				for(i=(tid*numOfvRMs)/omp_thr;i<((tid+1)*numOfvRMs)/omp_thr;i++)
				{
					if(maxSIs[tid]<SIs[i])
					{
						maxSIs[tid]=SIs[i];
						choices[tid]=i;
					}
				}
			}
			choice=choices[0];
			maxSI=maxSIs[0];
			for(i=1;i<omp_thr;i++)
			{
				if(maxSI<maxSIs[i])
				{
					maxSI=maxSIs[i];
					choice=choices[i];
				}
			}

			// Reduce the task's units from the list of available units
			double remProc=reqProc-availProc[choice];
			double remMem=reqMem-availMem[choice];
			double remSto=reqSto-availSto[choice];
			double remAcc=(double)reqAcc-availAcc[choice];

			// The pores list contains the resources of all vRMs needed to satisfy the task's requirments
			list<resource*> ores;

			// The ores list contains the resources of the current vRM
			list<resource*> pores;

			itt=vRMs->begin();
			for(i=0;i<choice;i++)
			{
				// Obtain the list of resources belonging to every vRM, until all the task's requested resources are satisfied
				(*itt)->obtainresources(pores,remProc,remMem,remSto,remAcc);

				// If the vRM has at least one resource, recalculate its suitability index
				if(pores.size()>0)
				{
					ssum=0.0;
					if (sPMSA[7]>0) {
						for(j=0;j<4;j++)
						{
							ssum+=Ws[j]*dassessfuncs(-((double)pores.size())*((double)(*pores.begin())->gtotAcc()),(double)totAcc[i],-((double)pores.size())*((*pores.begin())->gtotalMem()),totMem[i],j);
						}
					}
					else {
						for(j=0;j<4;j++)
						{
							ssum+=Ws[j]*dassessfuncs(-((double)pores.size())*((*pores.begin())->gtotalProc()),totProc[i],-((double)pores.size())*((*pores.begin())->gtotalMem()),totMem[i],j);
						}
					}
					totProc[i]-=((double)pores.size())*((*pores.begin())->gtotalProc());
					totMem[i]-=((double)pores.size())*((*pores.begin())->gtotalMem());
					totSto[i]-=((double)pores.size())*((*pores.begin())->gtotalSto());
					totAcc[i]-=((double)pores.size())*((double)(*pores.begin())->gtotAcc());
					availProc[i]-=((double)pores.size())*((*pores.begin())->gtotalProc());
					availMem[i]-=((double)pores.size())*((*pores.begin())->gtotalMem());
					availSto[i]-=((double)pores.size())*((*pores.begin())->gtotalSto());
					availAcc[i]-=((double)pores.size())*((double)(*pores.begin())->gtotAcc());
					SIs[i]+=ssum;

					// At the end of the ores list, add the pores list
					ores.splice(ores.end(),pores);
				}
				// If all remaining units are below 0, break the vRM loop
				if(remProc<=0.0 && remMem<=0.0 && remSto<=0.0 && remAcc<=0.0) {
					break;
				}
				// Move to the next vRM
				itt++;
			}
			// Save the last vRM needed to satisfy the task's unit requirements to itff
			itff=itt;

			// Go to the next vRM
			itt++;

			// If there are still some of the task's requirements unsatisfied but we are already at the vRM with the
			// highest suitability index
			if (!(remProc<=0.0 && remMem<=0.0 && remSto<=0.0 && remAcc<=0.0))
			{
				// Iterate the list of the remaining vRMs until the last
				for(i=choice+1;i<numOfvRMs;i++)
				{
					// Obtain the list of resources belonging to every vRM, until all the task's requested resources are satisfied
					(*itt)->obtainresources(pores,remProc,remMem,remSto,remAcc);
					if(pores.size()>0)
					{

						ssum=0.0;
						if (sPMSA[7]>0)
							for(j=0;j<4;j++)
							{
								ssum+=Ws[j]*dassessfuncs(-((double)pores.size())*((double)(*pores.begin())->gtotAcc()),(double)totAcc[i],-((double)pores.size())*((*pores.begin())->gtotalMem()),totMem[i],j);
							}
						else
							for(j=0;j<4;j++)
							{
								ssum+=Ws[j]*dassessfuncs(-((double)pores.size())*((*pores.begin())->gtotalProc()),totProc[i],-((double)pores.size())*((*pores.begin())->gtotalMem()),totMem[i],j);
							}
						totProc[i]-=((double)pores.size())*((*pores.begin())->gtotalProc());
						totMem[i]-=((double)pores.size())*((*pores.begin())->gtotalMem());
						totSto[i]-=((double)pores.size())*((*pores.begin())->gtotalSto());
						totAcc[i]-=((double)pores.size())*((double)(*pores.begin())->gtotAcc());
						availProc[i]-=((double)pores.size())*((*pores.begin())->gtotalProc());
						availMem[i]-=((double)pores.size())*((*pores.begin())->gtotalMem());
						availSto[i]-=((double)pores.size())*((*pores.begin())->gtotalSto());
						availAcc[i]-=((double)pores.size())*((double)(*pores.begin())->gtotAcc());
						SIs[i]+=ssum;
						// At the end of the ores list, add the pores list
						ores.splice(ores.end(),pores);
					}
					if(remProc<=0.0 && remMem<=0.0 && remSto<=0.0 && remAcc<=0.0) {
						break;
					}

					itt++;
				}
			}
			// If there was a number of resources obtained that satisfy the task's unit requirements
			if (ores.size()>0)
			{
				// Add these resources to the last vRM needed to satisfy the task's unit requirements
				(*itff)->attachresources(ores);

				// And then recalculate that vRMs assessment functions and suitability index
				totProc[choice]-=((double)ores.size())*((*ores.begin())->gtotalProc());
				totMem[choice]-=((double)ores.size())*((*ores.begin())->gtotalMem());
				totSto[choice]-=((double)ores.size())*((*ores.begin())->gtotalSto());
				totAcc[choice]-=((double)ores.size())*((double)(*ores.begin())->gtotAcc());
				availProc[choice]-=((double)ores.size())*((*ores.begin())->gtotalProc());
				availMem[choice]-=((double)ores.size())*((*ores.begin())->gtotalMem());
				availSto[choice]-=((double)ores.size())*((*ores.begin())->gtotalSto());
				availAcc[choice]-=((double)ores.size())*((double)(*ores.begin())->gtotAcc());
				ssum=0.0;
				if (sPMSA[7]>0)
					for(j=0;j<4;j++)
					{
						ssum+=Ws[j]*dassessfuncs(((double)ores.size())*((double)(*ores.begin())->gtotAcc()),(double)totAcc[choice],((double)ores.size())*((*ores.begin())->gtotalMem()),totMem[choice],j);
					}
				else
					for(j=0;j<4;j++)
					{
						ssum+=Ws[j]*dassessfuncs(((double)ores.size())*((*pores.begin())->gtotalProc()),totProc[choice],((double)ores.size())*((*ores.begin())->gtotalMem()),totMem[choice],j);
					}

				SIs[choice]+=ssum;
				ores.clear();
			}
			else {
				choice=-1;
			}

		}
		delete[] choices;
		delete[] maxSIs;

		// If after SOSM there is still no suitable vRM found, reject the task
		if (choice==-1)
		{
			stats[t->gavailImpl()[0]].rejTasks++;
			return;
		}

		// Compute again the assessment functions and suitability index of the modified vRM
		itf=vRMs->begin();
		for(i=0;i<choice;i++) {
			itf++;
		}

		availProc[choice]-=reqProc;
		availMem[choice]-=reqMem;
		availSto[choice]-=reqSto;
		availAcc[choice]-=reqAcc;
		sPMSA[0]-=reqProc;
		sPMSA[2]-=reqMem;
		sPMSA[4]-=reqSto;
		sPMSA[6]-=(double)reqAcc;

		if (sPMSA[7]>0) {
			for(i=0;i<4;i++)
			{
				ssum+=Ws[i]*dassessfuncs(-reqAcc,sPMSA[7],-reqMem,sPMSA[3],i);
			}
		}
		else {
			for(i=0;i<4;i++)
			{
				ssum+=Ws[i]*dassessfuncs(-reqProc,sPMSA[1],-reqMem,sPMSA[3],i);
			}
		}
		SI+=ssum;
		ssum=0.0;
		if (sPMSA[7]>0)
			for(i=0;i<4;i++)
			{
				ssum+=Ws[i]*dassessfuncs(-(double)reqAcc,(double)totAcc[choice],-reqMem,totMem[choice],i);
			}
		else {
			for(i=0;i<4;i++)
			{
				ssum+=Ws[i]*dassessfuncs(-reqProc,totProc[choice],-reqMem,totMem[choice],i);
			}
		}
		SIs[choice]+=ssum;

		// Recursively call the vRM::deploy method of the selected vRM
		(*itf)->deploy(resources,network,stats,t);

	}
}

int pSwitch::galloc() const
{
	return alloc;
}

int pSwitch::gnumOfvRMs() const
{
	return numOfvRMs;
}

int pSwitch::gnumOfFuncs() const
{
	return numOfFuncs;
}

double pSwitch::gpollIntervalpSwitch() const
{
	return pollIntervalpSwitch;
}

double *pSwitch::gFs() const
{
	return Fs;
}

double *pSwitch::gWs() const
{
	return Ws;
}

list<vRM*> *pSwitch::gvRMs() const
{
	return vRMs;
}

double *pSwitch::gavailProc() const
{
	return availProc;
}

double *pSwitch::gtotProc() const
{
	return totProc;
}

double *pSwitch::gavailMem() const
{
	return availMem;
}

double *pSwitch::gtotMem() const
{
	return totMem;
}

double *pSwitch::gavailAcc() const
{
	return availAcc;
}

double *pSwitch::gtotAcc() const
{
	return totAcc;
}

double *pSwitch::gavailSto() const
{
	return availSto;
}

double *pSwitch::gtotSto() const
{
	return totSto;
}

double *pSwitch::gsPMSA() const
{
	return sPMSA;
}

double *pSwitch::gSIs() const
{
	return SIs;
}

double pSwitch::gSI() const
{
	return SI;
}

double pSwitch::gC() const
{
	return C;
}

double pSwitch::gP() const
{
	return P;
}

double pSwitch::gPi() const
{
	return Pi;
}

void vRM::print()
{
	if(alloc)
	{
		for(list<resource*>::iterator it = res->begin(); it != res->end(); it++) {
			(*it)->print();
		}

	}
}

vRM::vRM()
{
	alloc=0;
	numOfRes=0;
	numOfFuncs=0;
	optNumOfRes=0;
	pollIntervalvRM=0.0;
	queue=NULL;
	res=NULL;
	availProc=NULL;
	totProc=NULL;
	availMem=NULL;
	totMem=NULL;
	availAcc=NULL;
	totAcc=NULL;
	availSto=NULL;
	totSto=NULL;
	sPMSA=NULL;
	Fs=NULL;
	Ws=NULL;
	C=0.0;
	P=0.0;
	Pi=0.0;
	SI=0.0;
	dep_strategy=0;
}

vRM::vRM(const int &start,const int &end,const int &type,resource** resources, const double &L_pollIntervalvRM, const double &L_C, const double &L_P, const double &L_Pi, const int &L_optNumOfRes,const int &L_numOfFuncs,const double *L_Ws, const int &L_dep_strategy)
{
	alloc=1;
	int i;
	numOfRes=end-start;
	optNumOfRes=L_optNumOfRes;
	numOfFuncs=L_numOfFuncs;
	pollIntervalvRM=L_pollIntervalvRM;
	C=L_C;
	P=L_P;
	Pi=L_Pi;
	queue=new list<task>[1];
	res=new list<resource*>[1];
	for(i=start;i<end;i++)
	{
		res->push_back(&resources[type][i]);
	}
	availProc=new double[numOfRes];
	totProc=new double[numOfRes];
	availMem=new double[numOfRes];
	totMem=new double[numOfRes];
	availAcc=new double[numOfRes];
	totAcc=new double[numOfRes];
	availSto=new double[numOfRes];
	totSto=new double[numOfRes];
	sPMSA=new double[8];
	for(i=0;i<8;i++) {
		sPMSA[i]=0.0;
	}
	Fs=new double[numOfFuncs];
	Ws=new double[numOfFuncs];
	for(i=0;i<numOfFuncs;i++)
	{
		Ws[i]=L_Ws[i];
		Fs[i]=0.0;
	}
	SI=0.0;
	dep_strategy=L_dep_strategy;
	updateStateInfo(0.0);

}

vRM::vRM(const vRM &t)
{
	int i;
	if (t.galloc())
	{
		alloc=1;
		numOfRes=t.gnumOfRes();
		optNumOfRes=t.goptNumOfRes();
		numOfFuncs=t.gnumOfFuncs();
		C=t.gC();
		P=t.gP();
		Pi=t.gPi();
		pollIntervalvRM=t.gpollIntervalvRM();
		queue=new list<task>[1];
		res=new list<resource*>[1];
		for(list<task>::iterator it = t.gqueue()->begin(); it != t.gqueue()->end(); it++) {
			queue->push_back(*it);
		}
		for(list<resource*>::iterator it = t.gres()->begin(); it != t.gres()->end(); it++) {
			res->push_back(*it);
		}
		availProc=new double[numOfRes];
		totProc=new double[numOfRes];
		availMem=new double[numOfRes];
		totMem=new double[numOfRes];
		availAcc=new double[numOfRes];
		totAcc=new double[numOfRes];
		availSto=new double[numOfRes];
		totSto=new double[numOfRes];
		for(i=0;i<numOfRes;i++)
		{
			availProc[i]=t.gavailProc()[i];
			totProc[i]=t.gtotProc()[i];
			availMem[i]=t.gavailMem()[i];
			totMem[i]=t.gtotMem()[i];
			availSto[i]=t.gavailSto()[i];
			totSto[i]=t.gtotSto()[i];
			availAcc[i]=t.gavailAcc()[i];
			totAcc[i]=t.gtotAcc()[i];
		}
		sPMSA=new double[8];
		for(i=0;i<8;i++) {
			sPMSA[i]=t.gsPMSA()[i];
		}
		Fs=new double[numOfFuncs];
		Ws=new double[numOfFuncs];
		for(i=0;i<numOfFuncs;i++)
		{
			Fs[i]=t.gFs()[i];
			Ws[i]=t.gWs()[i];
		}
		SI=t.gSI();
		dep_strategy=t.gdep_strategy();
	}
}

vRM & vRM::operator=(const vRM & t)
{
	int i;
    	if (this!=&t)
    	{
		if (alloc)
		{
			alloc=0;
			numOfRes=0;
			optNumOfRes=0;
			numOfFuncs=0;
			pollIntervalvRM=0.0;
			C=0.0;
			P=0.0;
			Pi=0.0;
			queue->clear();
			delete[] queue;
			res->clear();
			delete[] res;
			queue=NULL;
			res=NULL;
			delete[] availProc;
			delete[] totProc;
			delete[] availMem;
			delete[] totMem;
			delete[] availSto;
			delete[] totSto;
			delete[] availAcc;
			delete[] totAcc;
			delete[] sPMSA;
			delete[] Fs;
			delete[] Ws;
			availProc=NULL;
			totProc=NULL;
			availMem=NULL;
			totMem=NULL;
			availAcc=NULL;
			totAcc=NULL;
			availSto=NULL;
			totSto=NULL;
			sPMSA=NULL;
			Fs=NULL;
			Ws=NULL;
			SI=0.0;
			dep_strategy=0;
		}
		alloc=t.galloc();
		if (alloc)
		{
			numOfRes=t.gnumOfRes();
			optNumOfRes=t.goptNumOfRes();
			numOfFuncs=t.gnumOfFuncs();
			C=t.gC();
			P=t.gP();
			Pi=t.gPi();
			pollIntervalvRM=t.gpollIntervalvRM();
			queue=new list<task>[1];
			res=new list<resource*>[1];
			for(list<task>::iterator it = t.gqueue()->begin(); it != t.gqueue()->end(); it++) {
				queue->push_back(*it);
			}
		//	queue[0]=t.gqueue()[0];
			for(list<resource*>::iterator it = t.gres()->begin(); it != t.gres()->end(); it++) {
				res->push_back(*it);
			}
		//	res[0]=t.gres()[0];
			availProc=new double[numOfRes];
			totProc=new double[numOfRes];
			availMem=new double[numOfRes];
			totMem=new double[numOfRes];
			availAcc=new double[numOfRes];
			totAcc=new double[numOfRes];
			availSto=new double[numOfRes];
			totSto=new double[numOfRes];
			for(i=0;i<numOfRes;i++)
			{
				availProc[i]=t.gavailProc()[i];
				totProc[i]=t.gtotProc()[i];
				availMem[i]=t.gavailMem()[i];
				totMem[i]=t.gtotMem()[i];
				availSto[i]=t.gavailSto()[i];
				totSto[i]=t.gtotSto()[i];
				availAcc[i]=t.gavailAcc()[i];
				totAcc[i]=t.gtotAcc()[i];
			}
			sPMSA=new double[8];
			for(i=0;i<8;i++) {
				sPMSA[i]=t.gsPMSA()[i];
			}
			Fs=new double[numOfFuncs];
			Ws=new double[numOfFuncs];
			for(i=0;i<numOfFuncs;i++)
			{
				Fs[i]=t.gFs()[i];
				Ws[i]=t.gWs()[i];
			}
			SI=t.gSI();
			dep_strategy=t.gdep_strategy();
		}
	}
	return *this;
}

vRM::~vRM()
{
	if (alloc)
	{
		alloc=0;
		numOfRes=0;
		optNumOfRes=0;
		numOfFuncs=0;
		pollIntervalvRM=0.0;
		C=0.0;
		P=0.0;
		Pi=0.0;
		queue->clear();
		delete[] queue;
		queue=NULL;
		res->clear();
		delete[] res;
		res=NULL;
		delete[] availProc;
		delete[] totProc;
		delete[] availMem;
		delete[] totMem;
		delete[] availSto;
		delete[] totSto;
		delete[] availAcc;
		delete[] totAcc;
		delete[] sPMSA;
		delete[] Fs;
		delete[] Ws;
		availProc=NULL;
		totProc=NULL;
		availMem=NULL;
		totMem=NULL;
		availAcc=NULL;
		totAcc=NULL;
		availSto=NULL;
		totSto=NULL;
		sPMSA=NULL;
		Fs=NULL;
		Ws=NULL;
		SI=0.0;
		dep_strategy=0;
	}
}

void vRM::obtainresources(list<resource*> &ores, double &remProc, double &remMem, double &remSto, double &remAcc)
{
	if (alloc)
	{
		if (remProc<=0.0 && remMem<=0.0 && remSto<=0.0 && remAcc<=0) {
			return;
		}
		list <resource*>::iterator it=res->begin();
		int i=0;
		while (it!=res->end())
		{
			// If the resource is allowed to be movable to a different vRM
			if((*it)->gmovable()==1)
			{
				ores.push_back(*it);
				numOfRes--;
				remProc-=totProc[i];
				remMem-=totMem[i];
				remSto-=totSto[i];
				remAcc-=totAcc[i];
				sPMSA[0]-=totProc[i];
				sPMSA[1]-=totProc[i];
				sPMSA[2]-=totMem[i];
				sPMSA[3]-=totMem[i];
				sPMSA[4]-=totSto[i];
				sPMSA[5]-=totSto[i];
				sPMSA[6]-=totAcc[i];
				sPMSA[7]-=totAcc[i];

				double *availProc2=new double[numOfRes];
				double *totProc2=new double[numOfRes];
				double *availMem2=new double[numOfRes];
				double *totMem2=new double[numOfRes];
				double *availAcc2=new double[numOfRes];
				double *totAcc2=new double[numOfRes];
				double *availSto2=new double[numOfRes];
				double *totSto2=new double[numOfRes];

				int k=0;
				for(int j=0;j<i;j++)
				{
					availProc2[k]=availProc[j];
					totProc2[k]=totProc[j];
					availMem2[k]=availMem[j];
					totMem2[k]=totMem[j];
					availAcc2[k]=availAcc[j];
					totAcc2[k]=totAcc[j];
					availSto2[k]=availSto[j];
					totSto2[k]=totSto[j];
					k++;
				}
				for(int j=i+1;j<numOfRes+1;j++)
				{
					availProc2[k]=availProc[j];
					totProc2[k]=totProc[j];
					availMem2[k]=availMem[j];
					totMem2[k]=totMem[j];
					availAcc2[k]=availAcc[j];
					totAcc2[k]=totAcc[j];
					availSto2[k]=availSto[j];
					totSto2[k]=totSto[j];
					k++;
				}
				delete[] availProc;
				delete[] totProc;
				delete[] availMem;
				delete[] totMem;
				delete[] availAcc;
				delete[] totAcc;
				delete[] availSto;
				delete[] totSto;
				availProc=availProc2;
				totProc=totProc2;
				availMem=availMem2;
				totMem=totMem2;
				availAcc=availAcc2;
				totAcc=totAcc2;
				availSto=availSto2;
				totSto=totSto2;
				availProc2=NULL;
				totProc2=NULL;
				availMem2=NULL;
				totMem2=NULL;
				availAcc2=NULL;
				totAcc2=NULL;
				availSto2=NULL;
				totSto2=NULL;
				computeFs();
				computeSI();
				it=res->erase(it);
				if (remProc<=0.0 && remMem<=0.0 && remSto<=0.0 && remAcc<=0) {
					break;
				}
			}
			else
			{
				++it;
				i++;
			}
		}
	}

}

void vRM::attachresources(list<resource*> &ores)
{
	if (alloc)
	{
		int nar=(int)ores.size();
		int i=numOfRes;
		if (nar<=0)
			return;
		numOfRes+=nar;

		double *availProc2=new double[numOfRes];
		double *totProc2=new double[numOfRes];
		double *availMem2=new double[numOfRes];
		double *totMem2=new double[numOfRes];
		double *availAcc2=new double[numOfRes];
		double *totAcc2=new double[numOfRes];
		double *availSto2=new double[numOfRes];
		double *totSto2=new double[numOfRes];

		list<resource*>::iterator it=ores.begin();
		while(it!=ores.end())
		{
			sPMSA[0]+=(*it)->gtotalProc();
			sPMSA[1]+=(*it)->gtotalProc();
			sPMSA[2]+=(*it)->gtotalMem();
			sPMSA[3]+=(*it)->gtotalMem();
			sPMSA[4]+=(*it)->gtotalSto();
			sPMSA[5]+=(*it)->gtotalSto();
			sPMSA[6]+=(double)(*it)->gtotAcc();
			sPMSA[7]+=(double)(*it)->gtotAcc();
			availProc2[i]=(*it)->gtotalProc();
			totProc2[i]=(*it)->gtotalProc();
			availMem2[i]=(*it)->gtotalMem();
			totMem2[i]=(*it)->gtotalMem();
			availAcc2[i]=(double)(*it)->gtotAcc();
			totAcc2[i]=(double)(*it)->gtotAcc();
			availSto2[i]=(*it)->gtotalSto();
			totSto2[i]=(*it)->gtotalSto();
			res->push_back(*it);
			i++;
			it++;
		}

		for(int j=0;j<numOfRes-nar;j++)
		{
			availProc2[j]=availProc[j];
			totProc2[j]=totProc[j];
			availMem2[j]=availMem[j];
			totMem2[j]=totMem[j];
			availAcc2[j]=availAcc[j];
			totAcc2[j]=totAcc[j];
			availSto2[j]=availSto[j];
			totSto2[j]=totSto[j];
		}
		delete[] availProc;
		delete[] totProc;
		delete[] availMem;
		delete[] totMem;
		delete[] availAcc;
		delete[] totAcc;
		delete[] availSto;
		delete[] totSto;
		availProc=availProc2;
		totProc=totProc2;
		availMem=availMem2;
		totMem=totMem2;
		availAcc=availAcc2;
		totAcc=totAcc2;
		availSto=availSto2;
		totSto=totSto2;
		availProc2=NULL;
		totProc2=NULL;
		availMem2=NULL;
		totMem2=NULL;
		availAcc2=NULL;
		totAcc2=NULL;
		availSto2=NULL;
		totSto2=NULL;

		// After the resources were attached to the vRM, re-compute the assessment functions and the suitability index
		computeFs();
		computeSI();

	}
}

void vRM::updateStateInfo(const double &tstep)
{
	int i;
	if(alloc)
	{
		if(((int)tstep%(int)pollIntervalvRM)==0)
		{
			for(i=0;i<8;i++) {
				sPMSA[i]=0.0;
			}
			i=0;
			for(list<resource*>::iterator it=res->begin();it!=res->end();it++)
			{

				availProc[i]=(*it)->gavailProc();
				totProc[i]=(*it)->gtotalProc();
				availMem[i]=(*it)->gavailMem();
				totMem[i]=(*it)->gtotalMem();
				availAcc[i]=(double)(*it)->gavailAcc();
				totAcc[i]=(double)(*it)->gtotAcc();
				availSto[i]=(*it)->gavailSto();
				totSto[i]=(*it)->gtotalSto();
				i++;
			}
			for(i=0;i<(int)res->size();i++)
			{
				sPMSA[0]+=availProc[i];
				sPMSA[1]+=totProc[i];
				sPMSA[2]+=availMem[i];
				sPMSA[3]+=totMem[i];
				sPMSA[4]+=availSto[i];
				sPMSA[5]+=totSto[i];
				sPMSA[6]+=availAcc[i];
				sPMSA[7]+=totAcc[i];
			}
			computeFs();
			computeSI();
		}
	}
}

int vRM::galloc() const
{
	return alloc;
}

int vRM::gnumOfRes() const
{
	return numOfRes;
}

int vRM::goptNumOfRes() const
{
	return optNumOfRes;
}

double vRM::gpollIntervalvRM() const
{
	return pollIntervalvRM;
}

list<task> *vRM::gqueue() const
{
	return queue;
}

list<resource*> *vRM::gres() const
{
	return res;
}

double *vRM::gavailProc() const
{
	return availProc;
}

double *vRM::gtotProc() const
{
	return totProc;
}

double *vRM::gavailMem() const
{
	return availMem;
}

double *vRM::gtotMem() const
{
	return totMem;
}

double *vRM::gavailAcc() const
{
	return availAcc;
}

double *vRM::gtotAcc() const
{
	return totAcc;
}

double *vRM::gavailSto() const
{
	return availSto;
}

double *vRM::gtotSto() const
{
	return totSto;
}


double *vRM::gsPMSA() const
{
	return sPMSA;
}

double vRM::gC() const
{
	return C;
}

double vRM::gP() const
{
	return P;
}

double vRM::gPi() const
{
	return Pi;
}

int vRM::gnumOfFuncs() const
{
	return numOfFuncs;
}

double *vRM::gFs() const
{
	return Fs;
}

double *vRM::gWs() const
{
	return Ws;
}

double vRM::gSI() const
{
	return SI;
}

int vRM::gdep_strategy() const
{
	return dep_strategy;
}

void vRM::computeFs()
{
	int i;
	if(alloc)
	{
		for(i=0;i<numOfFuncs;i++)
		{
			Fs[i]=assessfuncs(i);
		}
	}
}

void vRM::computeSI()
{
	int i;
	if(alloc)
	{
		SI=(1e-4)*(((double)rand())/RAND_MAX);
		for(i=0;i<numOfFuncs;i++)
			SI+=Ws[i]*Fs[i];
	}
}

double vRM::assessfuncs(const int &choice)
{
	if(sPMSA[7]>0)
	{
		switch (choice)
		{
			case 0:
				return (C*((double)sPMSA[6])/(double)sPMSA[7]);
				break;
			case 1:
				return ((double)sPMSA[2])/(double)sPMSA[3];
				break;
			case 2:
				return (Pi*((double)sPMSA[6]))/(Pi*((double)sPMSA[6])+P*((double)sPMSA[7]-(double)sPMSA[6]));
				break;
			case 3:
				return (1.0-0.2*((double)sPMSA[7]-(double)sPMSA[6])/((double)sPMSA[7]));
				break;
			case 4:
				return 2.0/(1.0+exp(-6.0+6.0*(double)numOfRes/(double)optNumOfRes));
				break;
			default:
				return 0.0;
		}
	}
	else
	{
		switch (choice)
		{
			case 0:
				return (C*((double)sPMSA[0])/(double)sPMSA[1]);
				break;
			case 1:
				return ((double)sPMSA[2])/(double)sPMSA[3];
				break;
			case 2:
				return (Pi*((double)sPMSA[0]))/(Pi*((double)sPMSA[0])+P*((double)sPMSA[1]-(double)sPMSA[0]));
				break;
			case 3:
				return (1.0-0.2*((double)sPMSA[1]-(double)sPMSA[0])/((double)sPMSA[1]));
				break;
			case 4:
				return 2.0/(1.0+exp(-6.0+6.0*(double)numOfRes/(double)optNumOfRes));
				break;
			default:
				return 0.0;
		}
	}
}

double vRM::dassessfuncs(const double &dNu, const double &dNmem,const int &choice)
{
	if(sPMSA[7]>0)
	{
		switch (choice)
		{
			case 0:
				return (dNu*C/sPMSA[7]);
				break;
			case 1:
				return (dNmem/sPMSA[3]);
				break;
			case 2:
				return (dNu*Pi*P*sPMSA[7])/((P*(sPMSA[7]-sPMSA[6])+Pi*sPMSA[6])*(P*(sPMSA[7]-sPMSA[6])+Pi*sPMSA[6]));
				break;
			case 3:
				return (0.2*dNu/(sPMSA[7]));
				break;
			default:
				return 0.0;
		}

	}
	else
	{
		switch (choice)
		{
			case 0:
				return (dNu*C/sPMSA[1]);
				break;
			case 1:
				return (dNmem/sPMSA[3]);
				break;
			case 2:
				return (dNu*Pi*P*sPMSA[1])/((P*(sPMSA[1]-sPMSA[0])+Pi*sPMSA[0])*(P*(sPMSA[1]-sPMSA[0])+Pi*sPMSA[0]));
				break;
			case 3:
				return (0.2*dNu/(sPMSA[1]));
				break;
			default:
				return 0.0;
		}
	}
}

int vRM::deploy_strategy(list<resource*>::iterator *it, int *IDs, const int &nVMs, const double &Proc, const double &Mem, const double &Sto, const int &Acc)
{
	if (alloc)
	{
		int L_ID=-1,i,j,rem;
		list<resource*>::iterator itt;
		switch (dep_strategy)
		{
			// Task compaction strategy
			case 1:
				for(i=0;i<nVMs;i++)
				{
					L_ID=-1;
					itt=res->begin();
					for(j=0;j<numOfRes;j++)
					{
						if(availProc[j]>=Proc && availMem[j]>=Mem && availSto[j]>=Sto && availAcc[j]>=Acc)
						{
							L_ID=(*itt)->probe(Proc,Mem,Sto,Acc);
							if (L_ID!=-1)
							{
								IDs[i]=j;
								it[i]=itt;
								break;
							}
						}
						itt++;
					}
					if (L_ID==-1)
					{
						break;
					}
					else
					{
						availProc[j]-=Proc;
						availMem[j]-=Mem;
						availSto[j]-=Sto;
						availAcc[j]-=Acc;
					}
				}
				if(L_ID==-1)
				{
					for(i=0;i<nVMs;i++)
					{
						if(IDs[i]==-1)
						{
							break;
						}
						availProc[IDs[i]]+=Proc;
						availMem[IDs[i]]+=Mem;
						availSto[IDs[i]]+=Sto;
						availAcc[IDs[i]]+=Acc;
					}
					return (-1);
				}
				return (1);

			// Isotropy preservation strategy
			case 2:
				rem=nVMs;
				j=0;i=0;
				while (rem!=0 && i<nVMs)
				{
					j=0;
					itt=res->begin();
					while (j<numOfRes)
					{
						if(availProc[j]>=Proc && availMem[j]>=Mem && availSto[j]>=Sto && availAcc[j]>=Acc)
						{
							L_ID=(*itt)->probe(Proc,Mem,Sto,Acc);
							if (L_ID!=-1)
							{
								IDs[nVMs-rem]=j;
								it[nVMs-rem]=itt;
								availProc[j]-=Proc;
								availMem[j]-=Mem;
								availSto[j]-=Sto;
								availAcc[j]-=Acc;
								rem--;
							}
						}
						if(rem==0)
							break;
						j++;
						itt++;
					}
					i++;
				}
				if(rem!=0)
				{
					for(i=0;i<nVMs;i++)
					{
						if(IDs[i]==-1)
						{
							break;
						}
						availProc[IDs[i]]+=Proc;
						availMem[IDs[i]]+=Mem;
						availSto[IDs[i]]+=Sto;
						availAcc[IDs[i]]+=Acc;
					}
					return (-1);
				}
				return (1);
		}
	}
	return (0);
}

void vRM::deploy(resource **resources, netw *network, stat* stats, task * t)
{
	if (alloc)
	{
		int L_numOfVMs=t->gnumOfVMs();
		int type=t->gavailImpl()[0];
		double *L_reqPMNS=t->greqPMNS();
		double ssum=0.0;
		int L_avAcc=t->gavAcc()[0];
		int i;
		int L_ID=-1;
		int *IDs;
		list<resource*>::iterator *it,itt;

		// Check if the task's processes, memory, storage and accelerator are less or equal than the vRM's
		L_ID=network[0].probe(L_reqPMNS[2]);

		// If not, reject the task
		if (L_ID==-1)
		{
			stats[type].rejTasks++;
			return;
		}
		IDs=new int[L_numOfVMs];
		it=new list<resource*>::iterator[L_numOfVMs];
		for(i=0;i<L_numOfVMs;i++)
		{
			IDs[i]=-1;
		}

		// Position L_numOfVMs VMs on the vRM resources
		L_ID=deploy_strategy(it, IDs, L_numOfVMs, L_reqPMNS[0], L_reqPMNS[1], L_reqPMNS[3], L_avAcc);

		// If the VMs could not be positioned, reject the task
		if(L_ID==-1)
		{
			stats[type].rejTasks++;
		}
		else
		{
			// Deploy task on a resource
			// Set the ID of each VM as equal to the ID of the resource (match VMs to the appropriate resources)
			for(i=0;i<L_numOfVMs;i++)
			{
				(*it[i])->deploy(t);
				IDs[i]=(*it[i])->gID();
			}

			// Deploy task on network
			network[0].deploy(t);

			// Add the resources-VMs couples to the task
			t->attachResources(IDs);

			// Add the task to the list of tasks of the vRM
			enque(t);

			// Increase the number of active tasks of the vRM
			stats[type].accTasks++;

			// Update the assessment functions and the suitability index of the vRM
			sPMSA[0]-=L_numOfVMs*L_reqPMNS[0];
			sPMSA[2]-=L_numOfVMs*L_reqPMNS[1];
			sPMSA[4]-=L_numOfVMs*L_reqPMNS[3];
			sPMSA[6]-=(double)L_numOfVMs*L_avAcc;
			ssum=0.0;
			for(i=0;i<4;i++)
			{
				ssum+=Ws[i]*dassessfuncs(-L_numOfVMs*L_reqPMNS[0],-L_numOfVMs*L_reqPMNS[1],i);
			}
			SI+=ssum;
		}

		delete[] IDs;
		delete[] it;
	}
}

int vRM::probe(const double &Proc, const double &Mem, const double &Sto, const int &Acc)
{
	int rs=-1;
	if (Proc<=sPMSA[0] && Mem<=sPMSA[2] && Sto<=sPMSA[4] && Acc<=(int)sPMSA[6]) {
		rs=1;
	}

	return rs;
}

void vRM::enque(const task *t)
{
	if(alloc)
	{
		queue->push_back(*t);
	}
}


broker::broker()
{
	alloc=0;
	numOfTypes=0;
	types=NULL;
	numOfResourcesPerType=NULL;
	numOfvRMs=0;
	numOfpSwitches=0;
	numOfpRouters=0;
	pollIntervalCellM=0.0;
	pollIntervalpRouter=0.0;
	pollIntervalpSwitch=0.0;
	pollIntervalvRM=0.0;
	sPMSA=NULL;
	vRMs=NULL;
	SIs=NULL;
	pSwitches=NULL;
	pRouters=NULL;
	Cs=NULL;
	Ps=NULL;
	Pis=NULL;
	Ws=NULL;
	numOfFuncs=0;
}

void broker::initbroker(const int &L_numOfTypes, const int *L_types, const int *L_numOfResourcesPerType, resource **resources, power *powerComp, netw *network, const brinputs &binp)
{
	alloc=1;
	numOfTypes=L_numOfTypes;
	types=new int[numOfTypes];
	numOfResourcesPerType=new int[numOfTypes];

	for(int i=0;i<numOfTypes;i++)
	{
		types[i]=L_types[i];
		numOfResourcesPerType[i]=L_numOfResourcesPerType[i];
	}

	// Copy the interval time for polling from the BrokerData configuration file
	pollIntervalCellM=binp.pollIntervalCellM;
	pollIntervalpRouter=binp.pollIntervalpRouter;
	pollIntervalpSwitch=binp.pollIntervalpSwitch;
	pollIntervalvRM=binp.pollIntervalvRM;

	double *tempC=new double[numOfTypes];
	double *tempP=new double[numOfTypes];
	double *tempPi=new double[numOfTypes];

	// Calculate the values for C, P and Pi to be used for the assessment functions
	for(int i=0;i<numOfTypes;i++) {
		tempC[i]=resources[i][0].gcompCap()+resources[i][0].gaccCompCap();
		double oz=1.0;
		int active=resources[i][0].gactive();
		int totAcc=resources[i][0].gtotAcc();
		tempP[i]=powerComp[i].cpCons(oz,oz,active,totAcc);
		oz=0.0;
		tempPi[i]=powerComp[i].cpCons(oz,oz,active,totAcc);
	}

	int tminC=0;
	int tminP=0;
	for(int i=1;i<numOfTypes;i++)
	{
		if(tempC[tminC]>tempC[i])
			tminC=i;
		if(tempP[tminP]>tempP[i])
			tminP=i;
	}
	double minC=tempC[tminC];
	double minP=tempP[tminP];

	for(int i=0;i<numOfTypes;i++)
	{
		tempPi[i]=tempPi[i]/tempP[i];
	}

	for(int i=0;i<numOfTypes;i++)
	{
		tempC[i]/=minC;
		tempP[i]/=minP;
	}

	// Set the number of assesment functions
	numOfFuncs=binp.numOfFuncs;

	// Allocate an array of weights for the assesment functions
	Ws=new double[numOfFuncs];

	// Set the individual weights
	for(int i=0;i<numOfFuncs;i++) {
		Ws[i]=binp.Ws[i];
	}

	// Set the number of pRouters as equal to the number of hardware types
	numOfpRouters=numOfTypes;
	numOfvRMs=0;
	numOfpSwitches=0;
	vRMs=new list<vRM>*[numOfTypes];
	pSwitches=new list<pSwitch>*[numOfTypes];
	pRouters=new list<pRouter>*[numOfTypes];
	for(int i=0;i<numOfTypes;i++)
	{
		vRMs[i]=new list<vRM>[1];
		pSwitches[i]=new list<pSwitch>[1];
		pRouters[i]=new list<pRouter>[1];
	}

	// Create hierarchical the SOSM vRMs
	for(int i=0;i<numOfTypes;i++)
	{
		int temp=numOfResourcesPerType[i]/binp.initResPervRM;
		for(int j=0;j<temp;j++)
		{
			vRMs[i]->push_back(vRM(j*binp.initResPervRM,(j+1)*binp.initResPervRM,i,resources,pollIntervalvRM,tempC[i],tempP[i],tempPi[i],binp.initResPervRM,binp.numOfFuncs,binp.Ws,binp.vRMdeploystrategy));
		}
		numOfvRMs+=temp;
		if (temp*binp.initResPervRM!=numOfResourcesPerType[i])
		{
			vRMs[i]->push_back(vRM(temp*binp.initResPervRM,numOfResourcesPerType[i],i,resources,pollIntervalvRM,tempC[i],tempP[i],tempPi[i],binp.initResPervRM,binp.numOfFuncs,binp.Ws,binp.vRMdeploystrategy));
			numOfvRMs++;
		}
	}

	// Create hierarchical the SOSM pSwitches
	for(int i=0;i<numOfTypes;i++)
	{
		int temp=((int)vRMs[i]->size())/binp.initvRMPerpSwitch;
		for(int j=0;j<temp;j++)
		{
			pSwitches[i]->push_back(pSwitch(j*binp.initvRMPerpSwitch,(j+1)*binp.initvRMPerpSwitch,i,vRMs,pollIntervalpSwitch,tempC[i],tempP[i],tempPi[i],binp.numOfFuncs,binp.Ws));
		}
		numOfpSwitches+=temp;
		if(temp*binp.initvRMPerpSwitch!=((int)vRMs[i]->size()))
		{
			pSwitches[i]->push_back(pSwitch(temp*binp.initvRMPerpSwitch,((int)vRMs[i]->size()),i,vRMs,pollIntervalpSwitch,tempC[i],tempP[i],tempPi[i],binp.numOfFuncs,binp.Ws));
			numOfpSwitches++;
		}
	}

	// Create hierarchical the SOSM pRouters
	for(int i=0;i<numOfTypes;i++)
	{
		pRouters[i]->push_back(pRouter(0,((int)pSwitches[i]->size()),i,pSwitches,pollIntervalpRouter,tempC[i],tempP[i],tempPi[i],binp.numOfFuncs,binp.Ws));
	}

	// Allocate the sPMSA array
	sPMSA=new double*[numOfTypes];
	for(int i=0;i<numOfTypes;i++) {
		sPMSA[i]=new double[8];
	}

	for(int i=0;i<numOfTypes;i++)
	{
		for(list<pRouter>::iterator it = pRouters[i]->begin(); it != pRouters[i]->end(); it++)
		{
			for(int j=0;j<8;j++) {
				sPMSA[i][j]=it->gsPMSA()[j];
			}
		}
	}
	availNetw=network->gavailNetw();
	totNetw=network->gtotalNetw();
	SIs=new double[numOfTypes];
	for(int i=0;i<numOfTypes;i++) {
		SIs[i]=0.0;
	}
	Cs=tempC;
	tempC=NULL;
	Ps=tempP;
	tempP=NULL;
	Pis=tempPi;
	tempPi=NULL;
}
/*
broker::broker(const int &L_numOfTypes, const int *L_types, const int *L_numOfResourcesPerType, resource **resources, power *powerComp, netw *network, const brinputs &binp)
{
	int i=0,j=0;
	double *tempC=NULL,*tempP=NULL,*tempPi=NULL;
	double minP,minC;
	int tminC,tminP;
	alloc=1;
	numOfTypes=L_numOfTypes;
	types=new int[numOfTypes];
	numOfResourcesPerType=new int[numOfTypes];
	for(i=0;i<numOfTypes;i++)
	{
		types[i]=L_types[i];
		numOfResourcesPerType[i]=L_numOfResourcesPerType[i];
	}
	pollIntervalCellM=binp.pollIntervalCellM;
	pollIntervalpRouter=binp.pollIntervalpRouter;
	pollIntervalpSwitch=binp.pollIntervalpSwitch;
	pollIntervalvRM=binp.pollIntervalvRM;

	tempC=new double[numOfTypes];
	tempP=new double[numOfTypes];
	tempPi=new double[numOfTypes];
	for(i=0;i<numOfTypes;i++)
	{
		tempC[i]=resources[i][0].gcompCap()+resources[i][0].gaccCompCap();
		double oz=1.0;
		int active=resources[i][0].gactive();
		int totAcc=resources[i][0].gtotAcc();
		tempP[i]=powerComp[i].cpCons(oz,oz,active,totAcc);
		oz=0.0;
		tempPi[i]=powerComp[i].cpCons(oz,oz,active,totAcc);
	}
	tminC=0;
	tminP=0;
	for(i=1;i<numOfTypes;i++)
	{
		if(tempC[tminC]>tempC[i])
			tminC=i;
		if(tempP[tminP]>tempP[i])
			tminP=i;
	}
	minC=tempC[tminC];
	minP=tempP[tminP];

	for(i=0;i<numOfTypes;i++)
	{
		tempPi[i]=tempPi[i]/tempP[i];
	}

	for(i=0;i<numOfTypes;i++)
	{
		tempC[i]/=minC;
		tempP[i]/=minP;
	}

	numOfFuncs=binp.numOfFuncs;
	Ws=new double[numOfFuncs];
	for(i=0;i<numOfFuncs;i++)
		Ws[i]=binp.Ws[i];

	numOfpRouters=numOfTypes;
	numOfvRMs=0;
	numOfpSwitches=0;
	vRMs=new list<vRM>*[numOfTypes];
	pSwitches=new list<pSwitch>*[numOfTypes];
	pRouters=new list<pRouter>*[numOfTypes];
	for(i=0;i<numOfTypes;i++)
	{
		vRMs[i]=new list<vRM>[1];
		pSwitches[i]=new list<pSwitch>[1];
		pRouters[i]=new list<pRouter>[1];
	}
	for(i=0;i<numOfTypes;i++)
	{
		int temp=numOfResourcesPerType[i]/binp.initResPervRM;
		for(j=0;j<temp;j++)
		{
			vRMs[i]->push_back(vRM(j*binp.initResPervRM,(j+1)*binp.initResPervRM,i,resources,pollIntervalvRM,tempC[i],tempP[i],tempPi[i],binp.initResPervRM,binp.numOfFuncs,binp.Ws,binp.vRMdeploystrategy));
		}
		numOfvRMs+=temp;
		if (temp*binp.initResPervRM!=numOfResourcesPerType[i])
		{
			vRMs[i]->push_back(vRM(temp*binp.initResPervRM,numOfResourcesPerType[i],i,resources,pollIntervalvRM,tempC[i],tempP[i],tempPi[i],binp.initResPervRM,binp.numOfFuncs,binp.Ws,binp.vRMdeploystrategy));
			numOfvRMs++;
		}
	}

	for(i=0;i<numOfTypes;i++)
	{
		int temp=((int)vRMs[i]->size())/binp.initvRMPerpSwitch;
		for(j=0;j<temp;j++)
		{
			pSwitches[i]->push_back(pSwitch(j*binp.initvRMPerpSwitch,(j+1)*binp.initvRMPerpSwitch,i,vRMs,pollIntervalpSwitch,tempC[i],tempP[i],tempPi[i],binp.numOfFuncs,binp.Ws));
		}
		numOfpSwitches+=temp;
		if(temp*binp.initvRMPerpSwitch!=((int)vRMs[i]->size()))
		{
			pSwitches[i]->push_back(pSwitch(temp*binp.initvRMPerpSwitch,((int)vRMs[i]->size()),i,vRMs,pollIntervalpSwitch,tempC[i],tempP[i],tempPi[i],binp.numOfFuncs,binp.Ws));
			numOfpSwitches++;
		}
	}

	for(i=0;i<numOfTypes;i++)
	{
		pRouters[i]->push_back(pRouter(0,((int)pSwitches[i]->size()),i,pSwitches,pollIntervalpRouter,tempC[i],tempP[i],tempPi[i],binp.numOfFuncs,binp.Ws));
	}
	sPMSA=new double*[numOfTypes];
	for(i=0;i<numOfTypes;i++)
		sPMSA[i]=new double[8];

	for(i=0;i<numOfTypes;i++)
	{
		for(list<pRouter>::iterator it = pRouters[i]->begin(); it != pRouters[i]->end(); it++)
		{
			for(j=0;j<8;j++)
				sPMSA[i][j]=it->gsPMSA()[j];
		}
	}
	availNetw=network->gavailNetw();
	totNetw=network->gtotalNetw();
	SIs=new double[numOfTypes];
	for(i=0;i<numOfTypes;i++)
		SIs[i]=0.0;
	Cs=tempC;
	tempC=NULL;
	Ps=tempP;
	tempP=NULL;
	Pis=tempPi;
	tempPi=NULL;
}
*/
broker::broker(const broker &t)
{
	int i,j;
	if(t.galloc())
	{
		alloc=1;
		numOfTypes=t.gnumOfTypes();
		pollIntervalCellM=t.gpollIntervalCellM();
		pollIntervalpRouter=t.gpollIntervalpRouter();
		pollIntervalpSwitch=t.gpollIntervalpSwitch();
		pollIntervalvRM=t.gpollIntervalvRM();
		numOfpRouters=t.gnumOfpRouters();
		numOfpSwitches=t.gnumOfpSwitches();
		numOfvRMs=t.gnumOfvRMs();
		types=new int[numOfTypes];
		numOfResourcesPerType=new int[numOfTypes];
		for(i=0;i<numOfTypes;i++)
		{
			types[i]=t.gtypes()[i];
			numOfResourcesPerType[i]=t.gnumOfResourcesPerType()[i];
		}
		vRMs=new list<vRM>*[numOfTypes];
		pSwitches=new list<pSwitch>*[numOfTypes];
		pRouters=new list<pRouter>*[numOfTypes];
		for(i=0;i<numOfTypes;i++)
		{
			vRMs[i]=new list<vRM>[1];
			pSwitches[i]=new list<pSwitch>[1];
			pRouters[i]=new list<pRouter>[1];
		}
		for(i=0;i<numOfTypes;i++)
			for(list<vRM>::iterator it = t.gvRMs()[i]->begin(); it != t.gvRMs()[i]->end(); it++)
				vRMs[i]->push_back(*it);
		for(i=0;i<numOfTypes;i++)
			pSwitches[i][0]=t.gpSwitches()[i][0];
			for(list<pSwitch>::iterator it = t.gpSwitches()[i]->begin(); it != t.gpSwitches()[i]->end(); it++)
				pSwitches[i]->push_back(*it);
		for(i=0;i<numOfTypes;i++)
			pRouters[i][0]=t.gpRouters()[i][0];
			for(list<pRouter>::iterator it = t.gpRouters()[i]->begin(); it != t.gpRouters()[i]->end(); it++)
				pRouters[i]->push_back(*it);

		sPMSA=new double*[numOfTypes];
		for(i=0;i<numOfTypes;i++)
			sPMSA[i]=new double[8];

		for(i=0;i<numOfTypes;i++)
			for(j=0;j<8;j++)
				sPMSA[i][j]=t.gsPMSA()[i][j];
		availNetw=t.gavailNetw();
		totNetw=t.gtotNetw();
		SIs=new double[numOfTypes];
		for(i=0;i<numOfTypes;i++)
			SIs[i]=t.gSIs()[i];
		Cs=new double[numOfTypes];
		Ps=new double[numOfTypes];
		Pis=new double[numOfTypes];
		for(i=0;i<numOfTypes;i++)
		{
			Cs[i]=t.gCs()[i];
			Ps[i]=t.gPs()[i];
			Pis[i]=t.gPis()[i];
		}
		numOfFuncs=t.gnumOfFuncs();
		Ws=new double[numOfFuncs];
		for(i=0;i<numOfFuncs;i++)
			Ws[i]=t.gWs()[i];
	}
}

broker & broker::operator=(const broker & t)
{
    int i,j;
    if (this!=&t)
    {
	if (alloc)
	{
		alloc=0;
		delete[] types;
		delete[] numOfResourcesPerType;
		types=NULL;
		numOfResourcesPerType=NULL;
		for(i=0;i<numOfTypes;i++)
		{
			vRMs[i]->clear();
			pSwitches[i]->clear();
			pRouters[i]->clear();
			delete[] vRMs[i];
			delete[] pSwitches[i];
			delete[] pRouters[i];
			delete[] sPMSA[i];
		}
		delete[] pRouters;
		delete[] pSwitches;
		delete[] vRMs;
		delete[] sPMSA;
		delete[] SIs;
		SIs=NULL;
		sPMSA=NULL;
		vRMs=NULL;
		pSwitches=NULL;
		pRouters=NULL;
		numOfvRMs=0;
		numOfpSwitches=0;
		numOfpRouters=0;
		pollIntervalCellM=0.0;
		pollIntervalpRouter=0.0;
		pollIntervalpSwitch=0.0;
		pollIntervalvRM=0.0;
		numOfTypes=0;
		availNetw=0.0;
		totNetw=0.0;
		delete[] Cs;
		delete[] Ps;
		delete[] Pis;
		Cs=NULL;
		Ps=NULL;
		Pis=NULL;
		numOfFuncs=0;
		delete[] Ws;
		Ws=NULL;
	}
        alloc=t.galloc();
        if (alloc)
        {
		numOfTypes=t.gnumOfTypes();
		pollIntervalCellM=t.gpollIntervalCellM();
		pollIntervalpRouter=t.gpollIntervalpRouter();
		pollIntervalpSwitch=t.gpollIntervalpSwitch();
		pollIntervalvRM=t.gpollIntervalvRM();
		numOfpRouters=t.gnumOfpRouters();
		numOfpSwitches=t.gnumOfpSwitches();
		numOfvRMs=t.gnumOfvRMs();
		types=new int[numOfTypes];
		numOfResourcesPerType=new int[numOfTypes];
		for(i=0;i<numOfTypes;i++)
		{
			types[i]=t.gtypes()[i];
			numOfResourcesPerType[i]=t.gnumOfResourcesPerType()[i];
		}
		vRMs=new list<vRM>*[numOfTypes];
		pSwitches=new list<pSwitch>*[numOfTypes];
		pRouters=new list<pRouter>*[numOfTypes];
		for(i=0;i<numOfTypes;i++)
		{
			vRMs[i]=new list<vRM>[1];
			pSwitches[i]=new list<pSwitch>[1];
			pRouters[i]=new list<pRouter>[1];
		}
		for(i=0;i<numOfTypes;i++)
			for(list<vRM>::iterator it = t.gvRMs()[i]->begin(); it != t.gvRMs()[i]->end(); it++)
				vRMs[i]->push_back(*it);
		for(i=0;i<numOfTypes;i++)
			for(list<pSwitch>::iterator it = t.gpSwitches()[i]->begin(); it != t.gpSwitches()[i]->end(); it++)
				pSwitches[i]->push_back(*it);
		for(i=0;i<numOfTypes;i++)

			for(list<pRouter>::iterator it = t.gpRouters()[i]->begin(); it != t.gpRouters()[i]->end(); it++)
				pRouters[i]->push_back(*it);

		sPMSA=new double*[numOfTypes];
		for(i=0;i<numOfTypes;i++)
			sPMSA[i]=new double[8];

		for(i=0;i<numOfTypes;i++)
			for(j=0;j<8;j++)
				sPMSA[i][j]=t.gsPMSA()[i][j];
		availNetw=t.gavailNetw();
		totNetw=t.gtotNetw();
		SIs=new double[numOfTypes];
		for(i=0;i<numOfTypes;i++)
			SIs[i]=t.gSIs()[i];
		Cs=new double[numOfTypes];
		Ps=new double[numOfTypes];
		Pis=new double[numOfTypes];
		for(i=0;i<numOfTypes;i++)
		{
			Cs[i]=t.gCs()[i];
			Ps[i]=t.gPs()[i];
			Pis[i]=t.gPis()[i];
		}
		numOfFuncs=t.gnumOfFuncs();
		Ws=new double[numOfFuncs];
		for(i=0;i<numOfFuncs;i++)
			Ws[i]=t.gWs()[i];
	}
    }
    return *this;
}

broker::~broker()
{
	int i;
	if(alloc)
	{
		alloc=0;
		delete[] types;
		delete[] numOfResourcesPerType;
		types=NULL;
		numOfResourcesPerType=NULL;
		for(i=0;i<numOfTypes;i++)
		{
			vRMs[i]->clear();
			pSwitches[i]->clear();
			pRouters[i]->clear();
			delete[] vRMs[i];
			delete[] pSwitches[i];
			delete[] pRouters[i];
			delete[] sPMSA[i];
		}
		delete[] vRMs;
		delete[] pSwitches;
		delete[] pRouters;
		delete[] sPMSA;
		delete[] SIs;
		SIs=NULL;
		vRMs=NULL;
		pSwitches=NULL;
		pRouters=NULL;
		numOfvRMs=0;
		numOfpSwitches=0;
		numOfpRouters=0;
		pollIntervalCellM=0.0;
		pollIntervalpRouter=0.0;
		pollIntervalpSwitch=0.0;
		pollIntervalvRM=0.0;
		numOfTypes=0;
		availNetw=0.0;
		totNetw=0.0;
		delete[] Cs;
		delete[] Ps;
		delete[] Pis;
		Cs=NULL;
		Ps=NULL;
		Pis=NULL;
		delete[] Ws;
		Ws=NULL;
		numOfFuncs=0;
	}
}

void broker::print() const
{
	if(alloc)
	{
		for(int i=0;i<numOfTypes;i++)
		{
			cout<<"No of Type: "<<i<<endl;
			cout<<"     Number of available processing units: "<<sPMSA[i][0]<<endl;
			cout<<"     Number of total processing units: "<<sPMSA[i][1]<<endl;
			cout<<"     Number of available memory: "<<sPMSA[i][2]<<endl;
			cout<<"     Number of total memory: "<<sPMSA[i][3]<<endl;
			cout<<"     Number of available storage: "<<sPMSA[i][4]<<endl;
			cout<<"     Number of total storage: "<<sPMSA[i][5]<<endl;
			cout<<"     Number of available accelerators: "<<sPMSA[i][6]<<endl;
			cout<<"     Number of total accelerators: "<<sPMSA[i][7]<<endl;
		}
		cout<<"Available network bandwidth: "<<availNetw<<endl;
		cout<<"Total network bandwidth: "<<totNetw<<endl;

	}
}

double broker::dassessfuncs(const double &dNu, const double &dNmem,const int &choice, const int &type)
{
	if (sPMSA[type][7]>0)
	{
		switch (choice)
		{
			case 0:
				return (dNu*Cs[type]/sPMSA[type][7]);
				break;
			case 1:
				return (dNmem/sPMSA[type][3]);
				break;
			case 2:
				return (dNu*Pis[type]*Ps[type]*sPMSA[type][7])/((Ps[type]*(sPMSA[type][7]-sPMSA[type][6])+Pis[type]*sPMSA[type][6])*(Ps[type]*(sPMSA[type][7]-sPMSA[type][6])+Pis[type]*sPMSA[type][6]));
				break;
			case 3:
				return (0.2*dNu/(sPMSA[type][7]));
				break;
			default:
				return 0.0;
		}
	}
	else
	{
		switch (choice)
		{
			case 0:
				return (dNu*Cs[type]/sPMSA[type][1]);
				break;
			case 1:
				return (dNmem/sPMSA[type][3]);
				break;
			case 2:
				return (dNu*Pis[type]*Ps[type]*sPMSA[type][1])/((Ps[type]*(sPMSA[type][1]-sPMSA[type][0])+Pis[type]*sPMSA[type][0])*(Ps[type]*(sPMSA[type][1]-sPMSA[type][0])+Pis[type]*sPMSA[type][0]));
				break;
			case 3:
				return (0.2*dNu/(sPMSA[type][1]));
				break;
			default:
				return 0.0;
		}
	}
}

void broker::updateStateInfo(netw *network, const double &tstep)
{
	int i,j,k;
	list<pRouter>::iterator ittt;
	list<pSwitch>::iterator itt;
	list<vRM>::iterator it;
	if (alloc)
	{
		// On every time interval
		if(((int)tstep%(int)pollIntervalvRM)==0)
		{
			int omp_thr,len;
			// For each hardware type
			for(i=0;i<numOfTypes;i++)
			{
				//How many vRMs correspond to each hardware type
				len=(int)vRMs[i]->size();
				omp_thr=atoi(getenv("OMP_NUM_THREADS"));
				#pragma omp parallel default(shared) private(j,k,it) num_threads(omp_thr)
				{
					int tid=omp_get_thread_num();
					it = vRMs[i]->begin();
					for(j=0;j<tid;j++) {
						it++;
					}
					j=tid;
					while(j<len)
					{
						// Update state info on each vRM in parallel
						it->updateStateInfo(tstep);
						j+=omp_thr;
						for(k=0;k<omp_thr;k++) {
							it++;
						}
					}
					#pragma omp barrier
				}
			}
		}
		if(((int)tstep%(int)pollIntervalpSwitch)==0)
		{
			int omp_thr,len;
			for(i=0;i<numOfTypes;i++)
			{
				len=(int)pSwitches[i]->size();
				omp_thr=atoi(getenv("OMP_NUM_THREADS"));
				#pragma omp parallel default(shared) private(j,k,itt) num_threads(omp_thr)
				{
					int tid=omp_get_thread_num();
					itt = pSwitches[i]->begin();
					for(j=0;j<tid;j++)
						itt++;
					j=tid;
					while(j<len)
					{
						itt->updateStateInfo(tstep);
						for(k=0;k<omp_thr;k++)
							itt++;
						j+=omp_thr;
					}
					#pragma omp barrier
				}
			}
		}
		if(((int)tstep%(int)pollIntervalpRouter)==0)
		{
			int omp_thr=atoi(getenv("OMP_NUM_THREADS"));
			#pragma omp parallel for default(shared) private(i,ittt) num_threads(omp_thr) schedule(static,1)
			for(i=0;i<numOfTypes;i++)
			{
				ittt = pRouters[i]->begin();
				ittt->updateStateInfo(tstep);
			}
		}
		if(((int)tstep%(int)pollIntervalCellM)==0)
		{
			for(i=0;i<numOfTypes;i++)
			{
				for(list<pRouter>::iterator itttt = pRouters[i]->begin(); itttt != pRouters[i]->end(); itttt++)
				{
					for(j=0;j<8;j++)
						sPMSA[i][j]=itttt->gsPMSA()[j];
					SIs[i]=itttt->gSI();

				}

			}
		}
		availNetw=network->gavailNetw();
		totNetw=network->gtotalNetw();
	}
}

void broker::deploy(resource **resources, netw *network, stat *stats, task *t)
{
	int type=-1,i,j;
	int L_numOfVMs,*L_availImpl,*rem,*rem2,count;
	double maxSI;
	L_availImpl=t->gavailImpl();

	rem=new int[numOfTypes];
	rem2=new int[t->gnumOfAvailImpl()];
	count=0;

	// Determine how many (count) of the pRouter hardware types match the tasks' requested type, and store the types to the rem and rem arrays
	for(j=0;j<t->gnumOfAvailImpl();j++)
	{
		for(i=0;i<numOfTypes;i++)
		{
			if (types[i]==L_availImpl[j])
			{
				rem[count]=i;
				rem2[count]=j;
				count++;
				break;
			}
		}
	}

//	temp=network[0].probe(t->greqPMNS()[2]);
	if (availNetw<t->greqPMNS()[2])
	{
		stats[rem[0]].rejTasks++;
		delete[] rem;
		delete[] rem2;
		return;
	}
	availNetw-=t->greqPMNS()[2];

	maxSI=0.0;
	type=-1;
	L_numOfVMs=t->gnumOfVMs();
	// Lets find the pRouter out of the pRouters with a compatible Hardware Type with the highest Suitability Index
	for(i=0;i<count;i++)
	{
		list<pRouter>::iterator itt=pRouters[rem[i]]->begin();
		// Compare the current found maximum SI to the SI of each pRouter with a matching Hardware Type and double check that the pRouter offers the requested PMNS resources
		if(maxSI<SIs[rem[i]] && L_numOfVMs*t->greqPMNS()[0]<=sPMSA[rem[i]][0] && L_numOfVMs*t->greqPMNS()[1]<=sPMSA[rem[i]][2] && L_numOfVMs*t->greqPMNS()[3]<=sPMSA[rem[i]][4] && L_numOfVMs*t->gavAcc()[rem2[i]]<=sPMSA[rem[i]][6])
		{
			if(itt->probe(L_numOfVMs*t->greqPMNS()[0],L_numOfVMs*t->greqPMNS()[1],L_numOfVMs*t->greqPMNS()[3],L_numOfVMs*t->gavAcc()[rem2[i]])!=-1)
			{
				maxSI=SIs[rem[i]];
				type=i;
			}
		}
	}
	if (type==-1)
	{
		// Reject tasks if type is still -1
		stats[rem[0]].rejTasks++;
		return;
	}
	t->reduceImpl(&rem2[type]);
	t->remapType(&rem[type],1);
	type=rem[type];
	sPMSA[type][0]-=L_numOfVMs*t->greqPMNS()[0];
	sPMSA[type][2]-=L_numOfVMs*t->greqPMNS()[1];
	sPMSA[type][4]-=L_numOfVMs*t->greqPMNS()[3];
	sPMSA[type][6]-=L_numOfVMs*t->gavAcc()[0];

	// Update SI for the chosen pRouter
	for(i=0;i<4;i++) {
		SIs[type]+=Ws[i]*dassessfuncs(-L_numOfVMs*t->greqPMNS()[0],-L_numOfVMs*t->greqPMNS()[1],i,type);
	}

	list<pRouter>::iterator it = pRouters[type]->begin();
	it->deploy(resources,network,stats,t);
	delete[] rem;
	delete[] rem2;
}


void broker::timestep(resource **resources, netw *network, stat* stats, power *powerComp)
{
// The simulation phase consists of 6 execution steps
// 1. Initialize the running quantities: Proc and memory utilization, rho, network. Set initial values to 0.
// 2. Calculate for all tasks per vRM their running quantites (except network) and add them to calulcate the total
// 3. Calculate the number of instructions per processor can be execute for CPUs and accelerator
// 4. Calculate the power consumption on each timestep
// 5. Execute the tasks, and subtract the instructions already calculated
// 6. Remove the completed tasks

	int i=0,j,rID,len,k;
	double insR,insRa;
	double procUtil;
	double rhoAcc,L_totPcons;
	int active,L_numOfVMs;
	int totalAcc;
	int omp_thr=atoi(getenv("OMP_NUM_THREADS"));
	int chunk=50;
	double *L_net=NULL;
	list<vRM>::iterator itt,**itf;
	list<task>::iterator ittt;
	double ocP;
	int tid,jj;
	if(alloc)
	{
		// Step 1
		// Create a two-dimensional array of pointers to different vRMs (itf) in order to asssign them later to different OpenMP threads
		itf=new list<vRM>::iterator*[numOfTypes];
		for(i=0;i<numOfTypes;i++) {
			itf[i]=new list<vRM>::iterator[omp_thr];
		}
		for(i=0;i<numOfTypes;i++) {
			itf[i][0]=vRMs[i]->begin();
		}
		for(i=0;i<numOfTypes;i++) {
			len=(int)vRMs[i]->size();
			for(j=1;j<omp_thr;j++) {
				itt=itf[i][j-1];
				for(k=0;k<(j*len)/omp_thr-((j-1)*len)/omp_thr;k++) {
					itt++;
				}
				itf[i][j]=itt;
			}
		}

		for(i=0;i<numOfTypes;i++) {
			// Assign different resources (j) of the same hardware type (i) to different threads
			#pragma omp parallel for default(shared) private(j) num_threads(omp_thr) schedule(static,chunk)
			for(j=0;j<numOfResourcesPerType[i];j++)
				// If a given resource has one or more tasks assigned
				if(resources[i][j].gnumOfTasks()>0) {
					// Reset the overcommitment value in terms of processors and memory
					resources[i][j].initRunQuan();
				}
		}
		// Reset the overcommitment value in terms of network
		network[0].initRunQuan();

		// Step 2
		omp_thr=1;
		L_net=new double[omp_thr];
		for(i=0;i<omp_thr;i++) {
			L_net[i]=0.0;
		}
		// Scan all vRMs and for each vRM task retrieve mem and proc utilization and rho (the parallel percentage)
		// Calculate total utilization (by increasing the running quantities) in order to caluclate power consumption and intsructions per seconds to reduce from each task
		for(i=0;i<numOfTypes;i++) {
			len=(int)vRMs[i]->size();
			#pragma omp parallel default(shared) private(itt,ittt,k,rID,j,tid,L_numOfVMs) num_threads(omp_thr)
			{
				tid=omp_get_thread_num();
				itt=itf[i][tid];
				// For every vRM..
				for(k=(tid*len)/omp_thr;k<((tid+1)*len)/omp_thr;k++) {
					// Scan the task queue of the vRM..
					for(ittt=(itt->gqueue())->begin();ittt!=(itt->gqueue())->end();ittt++) {
						// Compute the utilization
						ittt->compcUtilPMNr();
						double *gcU=ittt->gcUtilPMNr();
						int *gr=ittt->gresourceIDs();
						L_net[tid]+=gcU[2];
						L_numOfVMs=ittt->gnumOfVMs();
						for(j=0;j<L_numOfVMs;j++) {
							rID=gr[j];
							// For every [hardware type][server per hardware type] increase the mem and proc utilization and rho
							// 1 vRm has 5 tasks; for every task see which servers it uses, and on every server calculate the running quantities it utilizes
							resources[i][rID].incrRunQuan(gcU[0], gcU[1], gcU[3]);
						}
					}
					itt++;
				}
				#pragma omp barrier
			}
		}

		for(i=1;i<omp_thr;i++) {
			L_net[0]+=L_net[i];
		}
		network[0].incrRunQuan(L_net[0]);
		delete[] L_net;

		// Step 3
		omp_thr=atoi(getenv("OMP_NUM_THREADS"));

		// Run through all resources and calculate the number of instructions to calculate per processing unit (taking into account the overcommitment of the processor resources)
		for(i=0;i<numOfTypes;i++) {
			#pragma omp parallel for default(shared) private(j) num_threads(omp_thr) schedule(static,chunk)
			for(j=0;j<numOfResourcesPerType[i];j++)
			{
				if(resources[i][j].gnumOfTasks()>0)
				{
					resources[i][j].compcCompCapPerProc();
					resources[i][j].compcCompCapPerAcc();
				}
			}
		}
		// Step 4
		// Calculate power consumption per pc and increase consumption on the stats engine
		for(i=0;i<numOfTypes;i++)
		{
			L_totPcons=0.0;
			#pragma omp parallel for default(shared) private(j,procUtil,rhoAcc,active,totalAcc) num_threads(omp_thr) schedule(static) reduction(+:L_totPcons)
			for(j=0;j<numOfResourcesPerType[i];j++)
			{
				procUtil=resources[i][j].gautilProc()/resources[i][j].gtotalProc();
				rhoAcc=resources[i][j].garhoAcc();
				active=resources[i][j].gactive();
				totalAcc=resources[i][j].gtotAcc();
				L_totPcons+=powerComp[i].cpCons(procUtil,rhoAcc,active,totalAcc);
			}
			stats[i].totPcons+=L_totPcons;
		}
		// Step 5
		// Run through all tasks to calculate the minimum of the instructions that can be executed based on the utilization and the overcommitment of each node
		for(i=0;i<numOfTypes;i++)
		{
			ocP=resources[i][0].goverCommitProc();
			len=(int)vRMs[i]->size();
			// For all vRms..
			#pragma omp parallel default(shared) private(itt,ittt,jj,j,tid,rID,insR,insRa,L_numOfVMs) num_threads(omp_thr)
			{
				tid=omp_get_thread_num();
				itt=itf[i][tid];
				// For all tasks of the vRM
				for(jj=(tid*len)/omp_thr;jj<((tid+1)*len)/omp_thr;jj++)
				{
					for(ittt=itt->gqueue()->begin();ittt!=itt->gqueue()->end();ittt++)
					{
						rID=(ittt->gresourceIDs())[0];
						insR=resources[i][rID].gcCompCapPerProc();
						insRa=resources[i][rID].gcCompCapPerAcc();
						L_numOfVMs=ittt->gnumOfVMs();
						// For each VM of the server (resource)
						for(j=1;j<L_numOfVMs;j++)
						{
							rID=(ittt->gresourceIDs())[j];
							// Get the minimum CPU task instructions that can be calculated on the given server (resource) between the requested instructions and the servers' available instructions based on the servers overcommitment value
							insR=MY_MIN(insR,resources[i][rID].gcCompCapPerProc());
							// Get the minimum accelerator task instructions that can be calculated on the given server (resource) between the requested instructions and the servers' available instructions based on the servers overcommitment value
							insRa=MY_MIN(insRa,resources[i][rID].gcCompCapPerAcc());
						}
						// Reduce the instructions left to compute of the given task
						ittt->reduceIns(L_numOfVMs*insR*MY_MIN(ittt->gcUtilPMNr()[0]*ocP,1.0)+L_numOfVMs*insRa*((ittt->gcUtilPMNr())[3]));
					}
					itt++;
				}
				#pragma omp barrier
			}
		}
		// Step 6
		// Estimate when a task has finished, by calculating when the number of instructions left goes to zero. (on the previous step the resources were reduced gradually). Run through all vRMs and free the task' resources that are done and the network
		int *numtasks;
		double **avau;
		numtasks=new int[omp_thr];
		avau=new double*[omp_thr];
		for(i=0;i<omp_thr;i++)
			avau[i]=new double[2];
		for(i=0;i<numOfTypes;i++)
		{
			for(j=0;j<omp_thr;j++)
			{
				avau[j][0]=0.0;
				avau[j][1]=0.0;
				numtasks[j]=0;
			}

			len=(int)vRMs[i]->size();
			#pragma omp parallel default(shared) private(itt,ittt,jj,j,tid,rID,L_numOfVMs) num_threads(omp_thr)
			{

				tid=omp_get_thread_num();
				itt=itf[i][tid];
				for(jj=(tid*len)/omp_thr;jj<((tid+1)*len)/omp_thr;jj++)
				{

					ittt=(itt->gqueue())->begin();
					while (ittt!=(itt->gqueue())->end())
					{
						// If the requested instruction of a task were dropped to zero
						if((ittt->greqIns())<=0.0)
						{
							L_numOfVMs=ittt->gnumOfVMs();
							// For all VMs
							for(j=0;j<L_numOfVMs;j++)
							{
								rID=(ittt->gresourceIDs())[j];
								// Mark the resources used by the task as not used any more and therefore "free" them to be used by another task in a future timestep
								resources[i][rID].unload(ittt);
							}
							numtasks[tid]++;
							avau[tid][0]+=(ittt->greqPMNS())[2];
							avau[tid][1]+=(ittt->gcUtilPMNr())[2];
							// Erase the task (ittt) from the task queue of the specific vRM (itt)
							ittt=(itt->gqueue())->erase(ittt);
						}
						else
							++ittt;
					}
					itt++;
				}
				#pragma omp barrier

			}
			for(j=1;j<omp_thr;j++)
			{
				avau[0][0]+=avau[j][0];
				avau[0][1]+=avau[j][1];
				numtasks[0]+=numtasks[j];
			}
			network[0].unload(avau[0][0],avau[0][1],numtasks[0]);
		}
		for(i=0;i<omp_thr;i++)
			delete[] avau[i];
		delete[] avau;
		delete[] numtasks;
		for(i=0;i<numOfTypes;i++)
			delete[] itf[i];
		delete[] itf;

	}
}


int broker::galloc() const
{
	return alloc;
}

int broker::gnumOfTypes() const
{
	return numOfTypes;
}
int *broker::gtypes() const
{
	return types;
}

int *broker::gnumOfResourcesPerType() const
{
	return numOfResourcesPerType;
}

int broker::gnumOfvRMs() const
{
	return numOfvRMs;
}

int broker::gnumOfpSwitches() const
{
	return numOfpSwitches;
}

int broker::gnumOfpRouters() const
{
	return numOfpRouters;
}

double broker::gpollIntervalCellM() const
{
	return pollIntervalCellM;
}

double broker::gpollIntervalpRouter() const
{
	return pollIntervalpRouter;
}

double broker::gpollIntervalpSwitch() const
{
	return pollIntervalpSwitch;
}

double broker::gpollIntervalvRM() const
{
	return pollIntervalvRM;
}

double broker::gtotNetw() const
{
	return totNetw;
}

double broker::gavailNetw() const
{
	return availNetw;
}

double **broker::gsPMSA() const
{
	return sPMSA;
}

double *broker::gSIs() const
{
	return SIs;
}

double *broker::gCs() const
{
	return Cs;
}

double *broker::gPs() const
{
	return Ps;
}

double *broker::gPis() const
{
	return Pis;
}

double *broker::gWs() const
{
	return Ws;
}

int broker::gnumOfFuncs() const
{
	return numOfFuncs;
}

list<vRM> **broker::gvRMs() const
{
	return vRMs;
}

list<pSwitch> **broker::gpSwitches() const
{
	return pSwitches;
}

list<pRouter> **broker::gpRouters() const
{
	return pRouters;
}

// brokerNOsosm 
brokerNOsosm::brokerNOsosm()
{
	alloc=0;
	numOfTypes=0;
	pollInterval=0.0;
	types=NULL;
	numOfResourcesPerType=NULL;
	availProc=NULL;
	totProc=NULL;
	availMem=NULL;
	totMem=NULL;
	availAcc=NULL;
	totAcc=NULL;
	availSto=NULL;
	totSto=NULL;
	availNetw=0.0;
	totNetw=0.0;
	queue=NULL;
}

brokerNOsosm::brokerNOsosm(const int &L_numOfTypes, const int *L_types, const int *L_numOfResourcesPerType, const double &L_pollInterval)
{
	int i=0;
	alloc=1;
	numOfTypes=L_numOfTypes;
	pollInterval=L_pollInterval;
	types=new int[numOfTypes];
	numOfResourcesPerType=new int[numOfTypes];
	for(i=0;i<numOfTypes;i++)
	{
		types[i]=L_types[i];
		numOfResourcesPerType[i]=L_numOfResourcesPerType[i];
	}
	availProc=new double*[numOfTypes];
	totProc=new double*[numOfTypes];
	availMem=new double*[numOfTypes];
	totMem=new double*[numOfTypes];
	availAcc=new double*[numOfTypes];
	totAcc=new double*[numOfTypes];
	availSto=new double*[numOfTypes];
	totSto=new double*[numOfTypes];
	availNetw=0.0;
	totNetw=0.0;	
	for(i=0;i<numOfTypes;i++)
	{
		availProc[i]=new double[numOfResourcesPerType[i]];
		totProc[i]=new double[numOfResourcesPerType[i]];
		availMem[i]=new double[numOfResourcesPerType[i]];
		totMem[i]=new double[numOfResourcesPerType[i]];
		availAcc[i]=new double[numOfResourcesPerType[i]];
		totAcc[i]=new double[numOfResourcesPerType[i]];
		availSto[i]=new double[numOfResourcesPerType[i]];
		totSto[i]=new double[numOfResourcesPerType[i]];
	}
	queue=new list<task>[1];
}

brokerNOsosm::brokerNOsosm(const brokerNOsosm &t)
{
	int i,j;
	if(t.galloc())
	{
		alloc=1;
		numOfTypes=t.gnumOfTypes();
		pollInterval=t.gpollInterval();
		types=new int[numOfTypes];
		numOfResourcesPerType=new int[numOfTypes];
		for(i=0;i<numOfTypes;i++)
		{
			types[i]=t.gtypes()[i];
			numOfResourcesPerType[i]=t.gnumOfResourcesPerType()[i];
		}
		availProc=new double*[numOfTypes];
		totProc=new double*[numOfTypes];
		availMem=new double*[numOfTypes];
		totMem=new double*[numOfTypes];
		availAcc=new double*[numOfTypes];
		totAcc=new double*[numOfTypes];
		availSto=new double*[numOfTypes];
		totSto=new double*[numOfTypes];
		availNetw=t.gavailNetw();
		totNetw=t.gtotNetw();	
		for(i=0;i<numOfTypes;i++)
		{
			availProc[i]=new double[numOfResourcesPerType[i]];
			totProc[i]=new double[numOfResourcesPerType[i]];
			availMem[i]=new double[numOfResourcesPerType[i]];
			totMem[i]=new double[numOfResourcesPerType[i]];
			availAcc[i]=new double[numOfResourcesPerType[i]];
			totAcc[i]=new double[numOfResourcesPerType[i]];
			availSto[i]=new double[numOfResourcesPerType[i]];
			totSto[i]=new double[numOfResourcesPerType[i]];
		}
		for(i=0;i<numOfTypes;i++)
		{
			for(j=0;j<numOfResourcesPerType[i];j++)
			{
				availProc[i][j]=t.gavailProc()[i][j];
				totProc[i][j]=t.gtotProc()[i][j];
				availMem[i][j]=t.gavailMem()[i][j];
				totMem[i][j]=t.gtotMem()[i][j];
				availAcc[i][j]=t.gavailAcc()[i][j];
				totAcc[i][j]=t.gtotAcc()[i][j];
				availSto[i][j]=t.gavailSto()[i][j];
				totSto[i][j]=t.gtotSto()[i][j];			
			}
		}
		queue=new list<task>[1];
		for(list<task>::iterator it = t.gqueue()->begin(); it != t.gqueue()->end(); it++)
			queue[0].push_back(*it);
	}
}

brokerNOsosm & brokerNOsosm::operator=(const brokerNOsosm & t)
{
    int i,j;
    if (this!=&t)
    {
	if (alloc)
	{
		alloc=0;
		pollInterval=0.0;
		delete[] types;
		delete[] numOfResourcesPerType;
		types=NULL;
		numOfResourcesPerType=NULL;
		for(i=0;i<numOfTypes;i++)
		{
			delete[] availProc[i];
			delete[] totProc[i];
			delete[] availMem[i];
			delete[] totMem[i];
			delete[] availAcc[i];
			delete[] totAcc[i];
			delete[] availSto[i];
			delete[] totSto[i];
		}
		delete[] availProc;
		delete[] totProc;
		delete[] availMem;
		delete[] totMem;
		delete[] availAcc;
		delete[] totAcc;
		delete[] availSto;
		delete[] totSto;
		availProc=NULL;
		totProc=NULL;
		availMem=NULL;
		totMem=NULL;
		availAcc=NULL;
		totAcc=NULL;
		availSto=NULL;
		totSto=NULL;
		availNetw=0.0;
		totNetw=0.0;
		numOfTypes=0;
		(*queue).clear();
		delete[] queue;
	}
        alloc=t.galloc();
        if (alloc)
        {
		numOfTypes=t.gnumOfTypes();
		pollInterval=t.gpollInterval();
		types=new int[numOfTypes];
		numOfResourcesPerType=new int[numOfTypes];
		for(i=0;i<numOfTypes;i++)
		{
			types[i]=t.gtypes()[i];
			numOfResourcesPerType[i]=t.gnumOfResourcesPerType()[i];
		}
		availProc=new double*[numOfTypes];
		totProc=new double*[numOfTypes];
		availMem=new double*[numOfTypes];
		totMem=new double*[numOfTypes];
		availAcc=new double*[numOfTypes];
		totAcc=new double*[numOfTypes];
		availSto=new double*[numOfTypes];
		totSto=new double*[numOfTypes];
		availNetw=t.gavailNetw();
		totNetw=t.gtotNetw();	
		for(i=0;i<numOfTypes;i++)
		{
			availProc[i]=new double[numOfResourcesPerType[i]];
			totProc[i]=new double[numOfResourcesPerType[i]];
			availMem[i]=new double[numOfResourcesPerType[i]];
			totMem[i]=new double[numOfResourcesPerType[i]];
			availAcc[i]=new double[numOfResourcesPerType[i]];
			totAcc[i]=new double[numOfResourcesPerType[i]];
			availSto[i]=new double[numOfResourcesPerType[i]];
			totSto[i]=new double[numOfResourcesPerType[i]];
		}
		for(i=0;i<numOfTypes;i++)
		{
			for(j=0;j<numOfResourcesPerType[i];j++)
			{
				availProc[i][j]=t.gavailProc()[i][j];
				totProc[i][j]=t.gtotProc()[i][j];
				availMem[i][j]=t.gavailMem()[i][j];
				totMem[i][j]=t.gtotMem()[i][j];
				availAcc[i][j]=t.gavailAcc()[i][j];
				totAcc[i][j]=t.gtotAcc()[i][j];
				availSto[i][j]=t.gavailSto()[i][j];
				totSto[i][j]=t.gtotSto()[i][j];			
			}
		}
		queue=new list<task>[1];
		for(list<task>::iterator it = t.gqueue()->begin(); it != t.gqueue()->end(); it++)
			queue[0].push_back(*it);
	}
    }
    return *this;
}

brokerNOsosm::~brokerNOsosm()
{
	int i;
	if(alloc)
	{
		alloc=0;
		pollInterval=0.0;
		delete[] types;
		delete[] numOfResourcesPerType;
		types=NULL;
		numOfResourcesPerType=NULL;
		for(i=0;i<numOfTypes;i++)
		{
			delete[] availProc[i];
			delete[] totProc[i];
			delete[] availMem[i];
			delete[] totMem[i];
			delete[] availAcc[i];
			delete[] totAcc[i];
			delete[] availSto[i];
			delete[] totSto[i];
		}
		delete[] availProc;
		delete[] totProc;
		delete[] availMem;
		delete[] totMem;
		delete[] availAcc;
		delete[] totAcc;
		delete[] availSto;

		delete[] totSto;
		availProc=NULL;
		totProc=NULL;
		availMem=NULL;
		totMem=NULL;
		availAcc=NULL;
		totAcc=NULL;
		availSto=NULL;
		totSto=NULL;
		availNetw=0.0;
		totNetw=0.0;
		numOfTypes=0;
		(*queue).clear();
		delete[] queue;
	}
}

void brokerNOsosm::print() const
{
	if(alloc)
	{
		cout<<"     Broker Poll Interval for Resources: "<<pollInterval<<endl;
	}
}

void brokerNOsosm::updateStateInfo(resource **resources, netw *network, const double &tstep)
{
	int i,j;
	int omp_thr=atoi(getenv("OMP_NUM_THREADS"));
	if (alloc)
	{
		if(((int)tstep%(int)pollInterval)==0)
		{
			for(i=0;i<numOfTypes;i++)
			{
				#pragma omp parallel for default(shared) private(j) num_threads(omp_thr) schedule(static)
				for(j=0;j<numOfResourcesPerType[i];j++)
				{
					availProc[i][j]=resources[i][j].gavailProc();
					totProc[i][j]=resources[i][j].gtotalProc();
					availMem[i][j]=resources[i][j].gavailMem();
					totMem[i][j]=resources[i][j].gtotalMem();
					availAcc[i][j]=resources[i][j].gavailAcc();
					totAcc[i][j]=resources[i][j].gtotAcc();
					availSto[i][j]=resources[i][j].gavailSto();
					totSto[i][j]=resources[i][j].gtotalSto();
				}
			}
			availNetw=network[0].gavailNetw();
			totNetw=network[0].gtotalNetw();
		}
	}
}

void brokerNOsosm::deploy(resource **resources, netw *network, stat *stats, task *t)
{

	int type=-1,i,j;
	int *IDs;
	int tID;
	int L_ID=-1;
	double *reqPMNS;
	int avAcc,L_numOfVMs,L_availImpl;
	reqPMNS=t->greqPMNS();
	avAcc=t->gavAcc()[0];
	L_availImpl=t->gavailImpl()[0];
	L_numOfVMs=t->gnumOfVMs();
	int omp_thr=atoi(getenv("OMP_NUM_THREADS"));
	for(i=0;i<numOfTypes;i++)
	{
		if (types[i]==L_availImpl)
		{
			type=i;
			t->remapType(&i,1);
			break;
		}
	}
	if (type==-1)
	{
		cout<<"Broker::deploy catastrophic error: "<<endl;
		exit(0);
	}
	
	L_ID=network[0].probe(reqPMNS[2]);
	if (L_ID==-1)
	{
		stats[type].rejTasks++;
		return;
	}
		
	availNetw-=reqPMNS[2];
	IDs=new int[L_numOfVMs];

	for(j=0;j<L_numOfVMs;j++)
		IDs[j]=-1;

/*	for(j=0;j<L_numOfVMs;j++)
	{
		L_ID=-1;
		for(i=0;i<numOfResourcesPerType[type];i++)
		{
			if(availProc[type][i]>=reqPMNS[0] && availMem[type][i]>=reqPMNS[1] && availSto[type][i]>=reqPMNS[3] && availAcc[type][i]>=avAcc)
			{
				L_ID=resources[type][i].probe(reqPMNS[0],reqPMNS[1],reqPMNS[3],avAcc);
				if(L_ID==i)
				{
					IDs[j]=i;
					availProc[type][i]-=reqPMNS[0];
					availMem[type][i]-=reqPMNS[1];
					availSto[type][i]-=reqPMNS[3];
					availAcc[type][i]-=avAcc;
					break;
				}
			}
		}
		if(L_ID==-1)
		{
			break;
		}
	}*/


	for(j=0;j<L_numOfVMs;j++)
	{
		L_ID=-1;
		tID=-1;

		#pragma omp parallel default(shared) private(i,tID) num_threads(omp_thr)
		{
			tID=-1;
			i=omp_get_thread_num();
			while (i<numOfResourcesPerType[type] && L_ID==-1)
			{
				if(availProc[type][i]>=reqPMNS[0] && availMem[type][i]>=reqPMNS[1] && availSto[type][i]>=reqPMNS[3] && availAcc[type][i]>=avAcc)
				{
					tID=resources[type][i].probe(reqPMNS[0],reqPMNS[1],reqPMNS[3],avAcc);
					if(tID==i)
					{
						#pragma omp single nowait
						{
							if (L_ID==-1)
							{					
								L_ID=i;	
								#pragma omp flush(L_ID)
								IDs[j]=i;
							}
						}
					}
					
				}

				i+=omp_thr;
				

			}
		}
		if(L_ID==-1)
		{
			break;
		}
		else
		{
			availProc[type][L_ID]-=reqPMNS[0];
			availMem[type][L_ID]-=reqPMNS[1];
			availSto[type][L_ID]-=reqPMNS[3];
			availAcc[type][L_ID]-=avAcc;
		}
	}

	if(L_ID==-1)
	{
		for(j=0;j<L_numOfVMs;j++)
		{
			if(IDs[j]==-1)
			{
				break;
			}
			availProc[type][IDs[j]]+=reqPMNS[0];
			availMem[type][IDs[j]]+=reqPMNS[1];
			availSto[type][IDs[j]]+=reqPMNS[3];
			availAcc[type][IDs[j]]+=avAcc;				
		}
		availNetw+=reqPMNS[2];
		stats[type].rejTasks++;
	}
	else
	{
		for(j=0;j<L_numOfVMs;j++)
		{
			resources[type][IDs[j]].deploy(t);
		}

		network[0].deploy(t);
		t->attachResources(IDs);
		enque(t);
		stats[type].accTasks++;		
	}
	delete[] IDs;

}

void brokerNOsosm::enque(const task *t)
{
	if(alloc)
	{
		queue->push_back(*t);
	}
}

void brokerNOsosm::timestep(resource **resources, netw *network, stat* stats, power *powerComp)
{
	int i,j,rID,type=-1;
	double insR,insRa;
	double procUtil;
	double rhoAcc,L_totPcons;	
	int active,L_numOfVMs;
	int totalAcc,len;
	int omp_thr=atoi(getenv("OMP_NUM_THREADS"));
	list<task>::iterator it;
	len=(int)(*queue).size();
	int chunk=50;
	double L_net=0.0;
	if(alloc)
	{
		
		for(i=0;i<numOfTypes;i++)
		{
			#pragma omp parallel for default(shared) private(j) num_threads(omp_thr) schedule(static,chunk)
			for(j=0;j<numOfResourcesPerType[i];j++){
				if(resources[i][j].gnumOfTasks()>0)	
					resources[i][j].initRunQuan();
			}
		}

		network[0].initRunQuan();

		L_net=0.0;
	//	#pragma omp parallel for default(shared) private(i,it,type) num_threads(omp_thr) schedule(static,chunk) reduction(+:L_net)
		for(it=queue->begin();it!=queue->end();it++)
		{
			type=(it->gavailImpl())[0];

			it->compcUtilPMNr();
			L_net+=it->gcUtilPMNr()[2];

			for(j=0;j<it->gnumOfVMs();j++)
			{
				rID=it->gresourceIDs()[j];
				resources[type][rID].incrRunQuan(it->gcUtilPMNr()[0], it->gcUtilPMNr()[1], it->gcUtilPMNr()[3]);
			}
			//network[0].incrRunQuan(it->gcUtilPMNr()[2]);
		}
		network[0].incrRunQuan(L_net);


		for(i=0;i<numOfTypes;i++)
		{
			#pragma omp parallel for default(shared) private(j) num_threads(omp_thr) schedule(static,chunk)
			for(j=0;j<numOfResourcesPerType[i];j++)
			{
				if(resources[i][j].gnumOfTasks()>0)
				{
					resources[i][j].compcCompCapPerProc();
					resources[i][j].compcCompCapPerAcc();
				}
			}
		}

		for(i=0;i<numOfTypes;i++)
		{
			L_totPcons=0.0;
			#pragma omp parallel for default(shared) private(j,procUtil,rhoAcc,active,totalAcc) num_threads(omp_thr) schedule(static) reduction(+:L_totPcons)
			for(j=0;j<numOfResourcesPerType[i];j++)
			{
				procUtil=resources[i][j].gautilProc()/resources[i][j].gtotalProc();
				rhoAcc=resources[i][j].garhoAcc();
				active=resources[i][j].gactive();
				totalAcc=resources[i][j].gtotAcc();
				L_totPcons+=powerComp[i].cpCons(procUtil,rhoAcc,active,totalAcc);		
			}
			stats[i].totPcons+=L_totPcons;
		}

		#pragma omp parallel default(shared) private(i,it,j,rID,insR,insRa,type,L_numOfVMs) num_threads(omp_thr)
		{
			int tid=omp_get_thread_num();
			it=queue->begin();
			for(i=0;i<tid;i++)
				it++;
			i=tid;
			double ocP=resources[type][0].goverCommitProc();
			while(i<len)
			{
				type=(it->gavailImpl())[0];
	
				rID=(it->gresourceIDs())[0];
				insR=resources[type][rID].gcCompCapPerProc();
				insRa=resources[type][rID].gcCompCapPerAcc();
				L_numOfVMs=it->gnumOfVMs();
				for(j=1;j<L_numOfVMs;j++)
				{
					rID=(it->gresourceIDs())[j];		
					insR=MY_MIN(insR,resources[type][rID].gcCompCapPerProc());
					insRa=MY_MIN(insRa,resources[type][rID].gcCompCapPerAcc());
				}
				it->reduceIns(L_numOfVMs*insR*MY_MIN(it->gcUtilPMNr()[0]*ocP,1.0)+L_numOfVMs*insRa*((it->gcUtilPMNr())[3]));
				for(j=0;j<omp_thr;j++)
				{				
					i++;
					it++;
				}
			}
		}
		it=queue->begin();
		while(it!=queue->end())
		{

			if((it->greqIns())<=0.0)
			{
				type=(it->gavailImpl())[0];
				L_numOfVMs=it->gnumOfVMs();
				for(j=0;j<L_numOfVMs;j++)
				{
					rID=(it->gresourceIDs())[j];
					resources[type][rID].unload(it);

				}

				network[0].unload(it);

				it=queue->erase(it);
		
			}
			else
				++it;

		}

	}
}

int brokerNOsosm::galloc() const
{
	return alloc;	
}

double brokerNOsosm::gpollInterval() const
{
	return pollInterval;
}

int brokerNOsosm::gnumOfTypes() const
{
	return numOfTypes;
}
int *brokerNOsosm::gtypes() const
{
	return types;
}

int *brokerNOsosm::gnumOfResourcesPerType() const
{
	return numOfResourcesPerType;
}

double **brokerNOsosm::gavailProc() const
{
	return availProc;
}
double **brokerNOsosm::gtotProc() const
{
	return totProc;
}

double **brokerNOsosm::gavailMem() const
{
	return availMem;
}

double **brokerNOsosm::gtotMem() const
{
	return totMem;
}

double **brokerNOsosm::gavailAcc() const
{
	return availAcc;
}

double **brokerNOsosm::gtotAcc() const
{
	return totAcc;
}
double **brokerNOsosm::gavailSto() const
{
	return availSto;
}

double **brokerNOsosm::gtotSto() const
{
	return totSto;
}

double brokerNOsosm::gavailNetw() const
{
	return availNetw;
}

double brokerNOsosm::gtotNetw() const
{
	return totNetw;
}

list<task>* brokerNOsosm::gqueue() const
{
	return queue;
}