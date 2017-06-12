#include <resource.h>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <iomanip>
#include <string>
#include <fstream>
using namespace std;

brinputs::brinputs()
{
	alloc=0;
	numOfFuncs=0;
	Ws=NULL;
	initResPervRM=0;
	initvRMPerpSwitch=0;
	initpSwitchPerpRouter=0;
	pollIntervalCellM=0.0;
	pollIntervalpRouter=0.0;
	pollIntervalpSwitch=0.0;
	pollIntervalvRM=0.0;
	vRMdeploystrategy=0;

}

brinputs::brinputs(const brinputs & t)
{
	int i;
	if(t.alloc)
	{
		alloc=t.alloc;
		numOfFuncs=t.numOfFuncs;
		Ws=new double[numOfFuncs];
		for(i=0;i<numOfFuncs;i++)
			Ws[i]=t.Ws[i];
		initResPervRM=t.initResPervRM;
		initvRMPerpSwitch=t.initvRMPerpSwitch;
		initpSwitchPerpRouter=t.initpSwitchPerpRouter;
		pollIntervalCellM=t.pollIntervalCellM;
		pollIntervalpRouter=t.pollIntervalpRouter;
		pollIntervalpSwitch=t.pollIntervalpSwitch;
		pollIntervalvRM=t.pollIntervalvRM;
		vRMdeploystrategy=t.vRMdeploystrategy;
	}
}

brinputs & brinputs::operator=(const brinputs & t)
{
    int i;
    if (this!=&t)
    {
	if(alloc)
	{
		alloc=0;
		numOfFuncs=0;
		delete[] Ws;
		Ws=NULL;
		initResPervRM=0;
		initvRMPerpSwitch=0;
		initpSwitchPerpRouter=0;
		pollIntervalCellM=0.0;
		pollIntervalpRouter=0.0;
		pollIntervalpSwitch=0.0;
		pollIntervalvRM=0.0;
		vRMdeploystrategy=0;
	}
	alloc=t.alloc;
	if(alloc)
	{
		numOfFuncs=t.numOfFuncs;
		Ws=new double[numOfFuncs];
		for(i=0;i<numOfFuncs;i++)
			Ws[i]=t.Ws[i];
		initResPervRM=t.initResPervRM;
		initvRMPerpSwitch=t.initvRMPerpSwitch;
		initpSwitchPerpRouter=t.initpSwitchPerpRouter;
		pollIntervalCellM=t.pollIntervalCellM;
		pollIntervalpRouter=t.pollIntervalpRouter;
		pollIntervalpSwitch=t.pollIntervalpSwitch;
		pollIntervalvRM=t.pollIntervalvRM;
		vRMdeploystrategy=t.vRMdeploystrategy;
	}
   }
   return *this;
}

brinputs::~brinputs()
{
	if(alloc)
	{
		alloc=0;
		numOfFuncs=0;
		delete[] Ws;
		Ws=NULL;
		initResPervRM=0;
		initvRMPerpSwitch=0;
		initpSwitchPerpRouter=0;
		pollIntervalCellM=0.0;
		pollIntervalpRouter=0.0;
		pollIntervalpSwitch=0.0;
		pollIntervalvRM=0.0;
		vRMdeploystrategy=0;
	}
}

void brinputs::print()
{
	int i;
	if (alloc)
	{
		cout<<"    =========== Broker ==========="<<endl;
		cout<<"        Number of Assessment Functions: "<<numOfFuncs<<endl;
		cout<<"        Weights: ";
		for(i=0;i<numOfFuncs;i++)
			cout<<Ws[i]<<" ";
		cout<<endl;
		cout<<"        Initial number of Resources per vRM: "<<initResPervRM<<endl;
		cout<<"        Initial number of vRMs per pSwitch: "<<initvRMPerpSwitch<<endl;
		cout<<"        Initial number of pSwitches per pRouter: "<<initpSwitchPerpRouter<<endl;
		cout<<"        Poll Interval for the Cell Manager: "<<pollIntervalCellM<<endl;
		cout<<"        Poll Interval for the pRouteres: "<<pollIntervalpRouter<<endl;
		cout<<"        Poll Interval for the pSwitches: "<<pollIntervalpSwitch<<endl;
		cout<<"        Poll Interval for the vRMs: "<<pollIntervalvRM<<endl;
		cout<<"        vRM deployment strategy: "<<vRMdeploystrategy<<endl;
		cout<<endl;
	}
}

void brinputs::parse(ifstream &outname)
{
	int i;
	alloc=1;
	outname>>numOfFuncs;
	Ws=new double[numOfFuncs];
	for(i=0;i<numOfFuncs;i++)
		outname>>Ws[i];
	outname>>initResPervRM;
	outname>>initvRMPerpSwitch;
	outname>>initpSwitchPerpRouter;
	outname>>pollIntervalCellM;
	outname>>pollIntervalpRouter;
	outname>>pollIntervalpSwitch;
	outname>>pollIntervalvRM;
	outname>>vRMdeploystrategy;
}

void brinputs::printfile(const string &outname, const ios::openmode &mode)
{
	fstream file;
	int i;
	if (alloc)
	{
		file.open(outname.c_str(),mode);
		file<<"    =========== Broker ==========="<<endl;
		file<<"        Number of Assessment Functions: "<<numOfFuncs<<endl;
		file<<"        Weights: ";
		for(i=0;i<numOfFuncs;i++)
			file<<Ws[i]<<" ";
		file<<endl;
		file<<"        Initial number of Resources per vRM: "<<initResPervRM<<endl;
		file<<"        Initial number of vRMs per pSwitch: "<<initvRMPerpSwitch<<endl;
		file<<"        Initial number of pSwitches per pRouter: "<<initpSwitchPerpRouter<<endl;
		file<<"        Poll Interval for the Cell Manager: "<<pollIntervalCellM<<endl;
		file<<"        Poll Interval for the pRouteres: "<<pollIntervalpRouter<<endl;
		file<<"        Poll Interval for the pSwitches: "<<pollIntervalpSwitch<<endl;
		file<<"        Poll Interval for the vRMs: "<<pollIntervalvRM<<endl;
		file<<"        vRM deployment strategy: "<<vRMdeploystrategy<<endl;
		file<<endl;
		file.close();
	}
}

appinputs::appinputs()
{
	alloc=0;
	minmaxJobsPerSec=NULL;
	numOfApps=0;
	numOfAvailImplPerApp=NULL;
	availImplPerApp=NULL;
	minmaxInsPerApp=NULL;
	minmaxVMPerApp=NULL;
	minmaxProcPerVM=NULL;
	minmaxMemPerVM=NULL;
	minmaxStoPerVM=NULL;
	minmaxNetPerApp=NULL;
	typeOfActP=NULL;
	typeOfActM=NULL;
	typeOfActN=NULL;
	minmaxActP=NULL;
	minmaxActM=NULL;
	minmaxActN=NULL;
	accelerator=NULL;
	rhoAcc=NULL;
}

appinputs::appinputs(const appinputs & t)
{
	int i,j;
	if(t.alloc)
	{
		alloc=t.alloc;
		minmaxJobsPerSec=new double[2];
		for(i=0;i<2;i++)
			minmaxJobsPerSec[i]=t.minmaxJobsPerSec[i];
		numOfApps=t.numOfApps;
		numOfAvailImplPerApp=new int[numOfApps];
		typeOfActP=new int[numOfApps];
		typeOfActM=new int[numOfApps];
		typeOfActN=new int[numOfApps];
		for(i=0;i<numOfApps;i++)
		{
			numOfAvailImplPerApp[i]=t.numOfAvailImplPerApp[i];
			typeOfActP[i]=t.typeOfActP[i];
			typeOfActM[i]=t.typeOfActM[i];
			typeOfActN[i]=t.typeOfActN[i];			
		}

		availImplPerApp=new int*[numOfApps];
		minmaxInsPerApp=new double*[numOfApps];
		minmaxVMPerApp=new int*[numOfApps];
		minmaxProcPerVM=new double*[numOfApps];
		minmaxMemPerVM=new double*[numOfApps];
		minmaxStoPerVM=new double*[numOfApps];
		minmaxNetPerApp=new double*[numOfApps];
		minmaxActP=new double*[numOfApps];
		minmaxActM=new double*[numOfApps];
		minmaxActN=new double*[numOfApps];
		accelerator=new int*[numOfApps];
		rhoAcc=new double*[numOfApps];

		for(i=0;i<numOfApps;i++)
		{
			availImplPerApp[i]=new int[numOfAvailImplPerApp[i]];
			accelerator[i]=new int[numOfAvailImplPerApp[i]];
			rhoAcc[i]=new double[numOfAvailImplPerApp[i]];
			for(j=0;j<numOfAvailImplPerApp[i];j++)
			{
				availImplPerApp[i][j]=t.availImplPerApp[i][j];
				accelerator[i][j]=t.accelerator[i][j];
				rhoAcc[i][j]=t.rhoAcc[i][j];
			}
			minmaxInsPerApp[i]=new double[2];
			minmaxVMPerApp[i]=new int[2];
			minmaxProcPerVM[i]=new double[2];
			minmaxMemPerVM[i]=new double[2];
			minmaxStoPerVM[i]=new double[2];
			minmaxNetPerApp[i]=new double[2];
			minmaxActP[i]=new double[2];
			minmaxActM[i]=new double[2];
			minmaxActN[i]=new double[2];
			for(j=0;j<2;j++)
			{
				minmaxInsPerApp[i][j]=t.minmaxInsPerApp[i][j];
				minmaxVMPerApp[i][j]=t.minmaxVMPerApp[i][j];
				minmaxProcPerVM[i][j]=t.minmaxProcPerVM[i][j];
				minmaxMemPerVM[i][j]=t.minmaxMemPerVM[i][j];
				minmaxStoPerVM[i][j]=t.minmaxStoPerVM[i][j];
				minmaxNetPerApp[i][j]=t.minmaxNetPerApp[i][j];
				minmaxActP[i][j]=t.minmaxActP[i][j];
				minmaxActM[i][j]=t.minmaxActM[i][j];
				minmaxActN[i][j]=t.minmaxActN[i][j];			
			}		
		}
	}
}

appinputs & appinputs::operator=(const appinputs & t)
{
    int i,j;
    if (this!=&t)
    {
	if (alloc)
	{
		alloc=0;
		delete[] minmaxJobsPerSec;
		delete[] numOfAvailImplPerApp;
		delete[] typeOfActP;
		delete[] typeOfActM;
		delete[] typeOfActN;
		for(i=0;i<numOfApps;i++)
		{
			delete[] availImplPerApp[i];
			delete[] minmaxInsPerApp[i];
			delete[] minmaxVMPerApp[i];
			delete[] minmaxProcPerVM[i];
			delete[] minmaxMemPerVM[i];
			delete[] minmaxStoPerVM[i];
			delete[] minmaxNetPerApp[i];
			delete[] minmaxActP[i];
			delete[] minmaxActM[i];
			delete[] minmaxActN[i];
			delete[] rhoAcc[i];
			delete[] accelerator[i];	
		}
		delete[] availImplPerApp;
		delete[] minmaxInsPerApp;
		delete[] minmaxVMPerApp;
		delete[] minmaxProcPerVM;
		delete[] minmaxMemPerVM;
		delete[] minmaxStoPerVM;
		delete[] minmaxNetPerApp;
		delete[] minmaxActP;
		delete[] minmaxActM;
		delete[] minmaxActN;
		delete[] accelerator;
		delete[] rhoAcc;
		numOfAvailImplPerApp=NULL;
		availImplPerApp=NULL;
		minmaxInsPerApp=NULL;
		minmaxVMPerApp=NULL;
		minmaxProcPerVM=NULL;
		minmaxMemPerVM=NULL;
		minmaxStoPerVM=NULL;
		minmaxNetPerApp=NULL;
		typeOfActP=NULL;
		typeOfActM=NULL;
		typeOfActN=NULL;
		minmaxActP=NULL;
		minmaxActM=NULL;
		minmaxActN=NULL;
		accelerator=NULL;
		rhoAcc=NULL;
		numOfApps=0;		
	}
        alloc=t.alloc;
        if (alloc)
       	{
		minmaxJobsPerSec=new double[2];
		for(i=0;i<2;i++)
			minmaxJobsPerSec[i]=t.minmaxJobsPerSec[i];
		numOfApps=t.numOfApps;
		numOfAvailImplPerApp=new int[numOfApps];
		typeOfActP=new int[numOfApps];
		typeOfActM=new int[numOfApps];
		typeOfActN=new int[numOfApps];
		for(i=0;i<numOfApps;i++)
		{
			numOfAvailImplPerApp[i]=t.numOfAvailImplPerApp[i];
			typeOfActP[i]=t.typeOfActP[i];
			typeOfActM[i]=t.typeOfActM[i];
			typeOfActN[i]=t.typeOfActN[i];			
		}

		availImplPerApp=new int*[numOfApps];
		minmaxInsPerApp=new double*[numOfApps];
		minmaxVMPerApp=new int*[numOfApps];
		minmaxProcPerVM=new double*[numOfApps];
		minmaxMemPerVM=new double*[numOfApps];
		minmaxStoPerVM=new double*[numOfApps];
		minmaxNetPerApp=new double*[numOfApps];
		minmaxActP=new double*[numOfApps];
		minmaxActM=new double*[numOfApps];
		minmaxActN=new double*[numOfApps];
		accelerator=new int*[numOfApps];
		rhoAcc=new double*[numOfApps];

		for(i=0;i<numOfApps;i++)
		{
			availImplPerApp[i]=new int[numOfAvailImplPerApp[i]];
			accelerator[i]=new int[numOfAvailImplPerApp[i]];
			rhoAcc[i]=new double[numOfAvailImplPerApp[i]];
			for(j=0;j<numOfAvailImplPerApp[i];j++)
			{
				availImplPerApp[i][j]=t.availImplPerApp[i][j];
				accelerator[i][j]=t.accelerator[i][j];
				rhoAcc[i][j]=t.rhoAcc[i][j];
			}
			minmaxInsPerApp[i]=new double[2];
			minmaxVMPerApp[i]=new int[2];
			minmaxProcPerVM[i]=new double[2];
			minmaxMemPerVM[i]=new double[2];
			minmaxStoPerVM[i]=new double[2];
			minmaxNetPerApp[i]=new double[2];
			minmaxActP[i]=new double[2];
			minmaxActM[i]=new double[2];
			minmaxActN[i]=new double[2];
			for(j=0;j<2;j++)
			{
				minmaxInsPerApp[i][j]=t.minmaxInsPerApp[i][j];
				minmaxVMPerApp[i][j]=t.minmaxVMPerApp[i][j];
				minmaxProcPerVM[i][j]=t.minmaxProcPerVM[i][j];
				minmaxMemPerVM[i][j]=t.minmaxMemPerVM[i][j];
				minmaxStoPerVM[i][j]=t.minmaxStoPerVM[i][j];
				minmaxNetPerApp[i][j]=t.minmaxNetPerApp[i][j];
				minmaxActP[i][j]=t.minmaxActP[i][j];
				minmaxActM[i][j]=t.minmaxActM[i][j];
				minmaxActN[i][j]=t.minmaxActN[i][j];			
			}		
		}	
	}
    }
    return *this;
}

void appinputs::parse(const string &fname)
{
	ifstream file;
	int i,j;
	file.open(fname.c_str());
	if (file)
	{
		alloc=1;
		minmaxJobsPerSec=new double[2];
		for(i=0;i<2;i++)
			file>>minmaxJobsPerSec[i];
		file>>numOfApps;
		numOfAvailImplPerApp=new int[numOfApps];
		availImplPerApp=new int*[numOfApps];
		minmaxInsPerApp=new double*[numOfApps];
		minmaxVMPerApp=new int*[numOfApps];
		minmaxProcPerVM=new double*[numOfApps];
		minmaxMemPerVM=new double*[numOfApps];
		minmaxStoPerVM=new double*[numOfApps];
		minmaxNetPerApp=new double*[numOfApps];
		minmaxActP=new double*[numOfApps];
		minmaxActM=new double*[numOfApps];
		minmaxActN=new double*[numOfApps];
		accelerator=new int*[numOfApps];
		rhoAcc=new double*[numOfApps];
		typeOfActP=new int[numOfApps];
		typeOfActM=new int[numOfApps];
		typeOfActN=new int[numOfApps];
		for(i=0;i<numOfApps;i++)
		{
			
			file>>numOfAvailImplPerApp[i];
			availImplPerApp[i]=new int[numOfAvailImplPerApp[i]];
			for(j=0;j<numOfAvailImplPerApp[i];j++)
			{
				file>>availImplPerApp[i][j];
			}

			minmaxInsPerApp[i]=new double[2];
			minmaxVMPerApp[i]=new int[2];
			minmaxProcPerVM[i]=new double[2];
			minmaxMemPerVM[i]=new double[2];
			minmaxStoPerVM[i]=new double[2];
			minmaxNetPerApp[i]=new double[2];

			for(j=0;j<2;j++)
			{
				file>>minmaxInsPerApp[i][j];
			}
			for(j=0;j<2;j++)
			{
				file>>minmaxVMPerApp[i][j];
			}
			for(j=0;j<2;j++)
			{
				file>>minmaxProcPerVM[i][j];
			}
			for(j=0;j<2;j++)
			{
				file>>minmaxMemPerVM[i][j];
			}
			for(j=0;j<2;j++)
			{
				file>>minmaxStoPerVM[i][j];
			}
			for(j=0;j<2;j++)
			{
				file>>minmaxNetPerApp[i][j];
			}

			file>>typeOfActP[i]>>typeOfActM[i]>>typeOfActN[i];

			minmaxActP[i]=new double[2];
			minmaxActM[i]=new double[2];
			minmaxActN[i]=new double[2];
			for(j=0;j<2;j++)
			{
				file>>minmaxActP[i][j];
			}
			for(j=0;j<2;j++)
			{
				file>>minmaxActM[i][j];
			}
			for(j=0;j<2;j++)
			{
				file>>minmaxActN[i][j];
			}
			accelerator[i]=new int[numOfAvailImplPerApp[i]];
			for(j=0;j<numOfAvailImplPerApp[i];j++)
			{
				file>>accelerator[i][j];
			}			
			rhoAcc[i]=new double[numOfAvailImplPerApp[i]];
			for(j=0;j<numOfAvailImplPerApp[i];j++)
			{
				file>>rhoAcc[i][j];
			}
		}
	}	
}

void appinputs::print()
{
	int i,j;
	if(alloc)
	{
		cout<<"=========== App Catalog ==========="<<endl;
		cout<<"Number of Applications: "<<numOfApps<<endl;
		cout<<"Minimum and Maximum Jobs Per Second: ";
		for(i=0;i<2;i++)
			cout<<minmaxJobsPerSec[i]<<" ";
		cout<<endl;
		cout<<"-----------------------------------"<<endl;
		for(i=0;i<numOfApps;i++)
		{
			cout<<"     Application: "<<i+1<<endl;
			cout<<"     Number of Available Implementations: "<<numOfAvailImplPerApp[i]<<endl;
			cout<<"     Available Implementations: ";
			for(j=0;j<numOfAvailImplPerApp[i];j++)
				cout<<availImplPerApp[i][j]<<" ";
			cout<<endl;
			cout<<"     Minimum - Maximum Instructions Per App: ";
			for(j=0;j<2;j++)
			{
				cout<<minmaxInsPerApp[i][j]<<" ";
			}
			cout<<endl;
			cout<<"     Minimum - Maximum VMs Per App: ";
			for(j=0;j<2;j++)
			{
				cout<<minmaxVMPerApp[i][j]<<" ";
			}
			cout<<endl;
			cout<<"     Minimum - Maximum vCPUs Per VM: ";
			for(j=0;j<2;j++)
			{
				cout<<minmaxProcPerVM[i][j]<<" ";
			}
			cout<<endl;
			cout<<"     Minimum - Maximum Memory Per VM: ";
			for(j=0;j<2;j++)
			{
				cout<<minmaxMemPerVM[i][j]<<" ";
			}
			cout<<"GBytes"<<endl;
			cout<<"     Minimum - Maximum Storage Per VM: ";
			for(j=0;j<2;j++)
			{
				cout<<minmaxStoPerVM[i][j]<<" ";
			}
			cout<<"TBytes"<<endl;
			cout<<"     Minimum - Maximum Network Per VM: ";
			for(j=0;j<2;j++)
			{
				cout<<minmaxNetPerApp[i][j]<<" ";
			}
			cout<<"Gbps"<<endl;
			cout<<"     Type of Actual Utilization (vCPU,Memory,Network): "<<typeOfActP[i]<<" "<<typeOfActM[i]<<" "<<typeOfActN[i]<<endl;
			cout<<"     Minimum - Maximum Actual vCPU Utilization: ";
			for(j=0;j<2;j++)
			{
				cout<<minmaxActP[i][j]<<" ";
			}
			cout<<endl;
			cout<<"     Minimum - Maximum Actual Memory Utilization: ";
			for(j=0;j<2;j++)
			{
				cout<<minmaxActM[i][j]<<" ";
			}
			cout<<endl;
			cout<<"     Minimum - Maximum Actual Network Utilization: ";
			for(j=0;j<2;j++)
			{
				cout<<minmaxActN[i][j]<<" ";
			}
			cout<<endl;
			cout<<"     Accelerator support: ";
			for(j=0;j<numOfAvailImplPerApp[i];j++)
			{
				cout<<accelerator[i][j]<<" ";
			}
			cout<<endl;
			cout<<"     Rho for accelerator execution per Implementation: ";
			for(j=0;j<numOfAvailImplPerApp[i];j++)
			{
				cout<<rhoAcc[i][j]<<" ";
			}
			cout<<endl;						
			cout<<"-----------------------------------"<<endl;

		}
	}
}

void appinputs::printfile(const string &outname, const ios::openmode &mode)
{
	int i,j;
	fstream file;
	if(alloc)
	{
		file.open(outname.c_str(),mode);
		file<<"=========== App Catalog ==========="<<endl;
		file<<"Number of Applications: "<<numOfApps<<endl;
		file<<"Minimum and Maximum Jobs Per Second: ";
		for(i=0;i<2;i++)
			file<<minmaxJobsPerSec[i]<<" ";
		file<<endl;
		file<<"-----------------------------------"<<endl;
		for(i=0;i<numOfApps;i++)
		{
			file<<"     Application: "<<i+1<<endl;
			file<<"     Number of Available Implementations: "<<numOfAvailImplPerApp[i]<<endl;
			file<<"     Available Implementations: ";
			for(j=0;j<numOfAvailImplPerApp[i];j++)
				file<<availImplPerApp[i][j]<<" ";
			file<<endl;
			file<<"     Minimum - Maximum Instructions Per App: ";
			for(j=0;j<2;j++)
			{
				file<<minmaxInsPerApp[i][j]<<" ";
			}
			file<<endl;
			file<<"     Minimum - Maximum VMs Per App: ";
			for(j=0;j<2;j++)
			{
				file<<minmaxVMPerApp[i][j]<<" ";
			}
			file<<endl;
			file<<"     Minimum - Maximum vCPUs Per VM: ";
			for(j=0;j<2;j++)
			{
				file<<minmaxProcPerVM[i][j]<<" ";
			}
			file<<endl;
			file<<"     Minimum - Maximum Memory Per VM: ";
			for(j=0;j<2;j++)
			{
				file<<minmaxMemPerVM[i][j]<<" ";
			}
			file<<"GBytes"<<endl;
			file<<"     Minimum - Maximum Storage Per VM: ";
			for(j=0;j<2;j++)
			{
				file<<minmaxStoPerVM[i][j]<<" ";
			}
			file<<"TBytes"<<endl;
			file<<"     Minimum - Maximum Network Per VM: ";
			for(j=0;j<2;j++)
			{
				file<<minmaxNetPerApp[i][j]<<" ";
			}
			file<<"Gbps"<<endl;
			file<<"     Type of Actual Utilization (vCPU,Memory,Network): "<<typeOfActP[i]<<" "<<typeOfActM[i]<<" "<<typeOfActN[i]<<endl;
			file<<"     Minimum - Maximum Actual vCPU Utilization: ";
			for(j=0;j<2;j++)
			{
				file<<minmaxActP[i][j]<<" ";
			}
			file<<endl;
			file<<"     Minimum - Maximum Actual Memory Utilization: ";
			for(j=0;j<2;j++)
			{
				file<<minmaxActM[i][j]<<" ";
			}
			file<<endl;
			file<<"     Minimum - Maximum Actual Network Utilization: ";
			for(j=0;j<2;j++)
			{
				file<<minmaxActN[i][j]<<" ";
			}
			file<<endl;
			file<<"     Accelerator support: ";
			for(j=0;j<numOfAvailImplPerApp[i];j++)
			{
				file<<accelerator[i][j]<<" ";
			}
			file<<endl;
			file<<"     Rho for accelerator execution per Implementation: ";
			for(j=0;j<numOfAvailImplPerApp[i];j++)
			{
				file<<rhoAcc[i][j]<<" ";
			}
			file<<endl;						
			file<<"-----------------------------------"<<endl;

		}
		file.close();
	}
}

appinputs::~appinputs()
{
	int i;
	if (alloc)
	{
		alloc=0;
		delete[] minmaxJobsPerSec;
		delete[] numOfAvailImplPerApp;
		delete[] typeOfActP;
		delete[] typeOfActM;
		delete[] typeOfActN;
		for(i=0;i<numOfApps;i++)
		{
			delete[] availImplPerApp[i];
			delete[] minmaxInsPerApp[i];
			delete[] minmaxVMPerApp[i];
			delete[] minmaxProcPerVM[i];
			delete[] minmaxMemPerVM[i];
			delete[] minmaxStoPerVM[i];
			delete[] minmaxNetPerApp[i];
			delete[] minmaxActP[i];
			delete[] minmaxActM[i];
			delete[] minmaxActN[i];
			delete[] accelerator[i];
			delete[] rhoAcc[i];		
		}
		delete[] availImplPerApp;
		delete[] minmaxInsPerApp;
		delete[] minmaxVMPerApp;
		delete[] minmaxProcPerVM;
		delete[] minmaxMemPerVM;
		delete[] minmaxStoPerVM;
		delete[] minmaxNetPerApp;
		delete[] minmaxActP;
		delete[] minmaxActM;
		delete[] minmaxActN;
		delete[] accelerator;
		delete[] rhoAcc;
		numOfAvailImplPerApp=NULL;
		availImplPerApp=NULL;
		minmaxInsPerApp=NULL;
		minmaxVMPerApp=NULL;
		minmaxProcPerVM=NULL;
		minmaxMemPerVM=NULL;
		minmaxStoPerVM=NULL;
		minmaxNetPerApp=NULL;
		typeOfActP=NULL;
		typeOfActM=NULL;
		typeOfActN=NULL;
		minmaxActP=NULL;
		minmaxActM=NULL;
		minmaxActN=NULL;
		accelerator=NULL;
		rhoAcc=NULL;
		numOfApps=0;
	}
}

netinputs::netinputs()
{
	alloc=0;
	netBW=0.0;
	overCommitNet=1.0;
}

netinputs::netinputs(const netinputs &t)
{
	if (t.alloc)
	{
		alloc=t.alloc;
		netBW=t.netBW;
		overCommitNet=t.overCommitNet;
	}
}

netinputs & netinputs::operator=(const netinputs & t)
{
	if(this!=&t)
	{
		if (alloc)
		{
			alloc=0;
			netBW=0.0;
			overCommitNet=1.0;
		}
		alloc=t.alloc;
		if (alloc)
		{
			netBW=t.netBW;
			overCommitNet=t.overCommitNet;
		}
	}
	return *this;
}

void netinputs::print()
{
	if(alloc)
	{
		cout<<"=====Network Interconnection====="<<endl;
		cout<<"     Interconnection Bandwidth: "<<netBW<<" Gbps"<<endl;
		cout<<"     Bandwidth Over Commitment Ration: "<<overCommitNet<<endl;
		cout<<"================================="<<endl;
	}
}

netinputs::~netinputs()
{
	if(alloc)
	{
		alloc=0;
		netBW=0.0;
		overCommitNet=1.0;
	}
}

powinputs::powinputs()
{
	alloc=0;
	typeCpu=0;
	typeAcc=0;
	cpuPmin=0.0;
	cpuPmax=0.0;
	cpuC=0.0;
	numOfPoints=0;
	cpubins=NULL;
	cpuP=NULL;
	accelerator=0;
	accPmin=0.0;
	accPmax=0.0;
	accC=0.0;
}

powinputs::powinputs(const powinputs &t)
{
	int i;
	if (t.alloc)
	{
		alloc=t.alloc;
		typeCpu=t.typeCpu;
		typeAcc=t.typeAcc;
		cpuPmin=t.cpuPmin;
		cpuPmax=t.cpuPmax;
		cpuC=t.cpuC;
		numOfPoints=t.numOfPoints;
		if (numOfPoints>0)
		{
			cpubins=new double[numOfPoints];
			cpuP=new double[numOfPoints];
			for(i=0;i<numOfPoints;i++)
			{
				cpubins[i]=t.cpubins[i];
				cpuP[i]=t.cpuP[i];
			}
		}
		else
		{
			cpubins=NULL;
			cpuP=NULL;
		}
		accelerator=t.accelerator;
		accPmin=t.accPmin;
		accPmax=t.accPmax;
		accC=t.accC;
	}
}

powinputs & powinputs::operator=(const powinputs &t)
{
    int i;
    if (this!=&t)
    {
	if (alloc)
	{
		alloc=0;
		typeCpu=0;
		typeAcc=0;
		cpuPmin=0.0;
		cpuPmax=0.0;
		cpuC=0.0;
		accelerator=0;
		accPmin=0.0;
		accPmax=0.0;
		accC=0.0;		
		if (numOfPoints!=0)
		{
			numOfPoints=0;
			delete[] cpubins;
			delete[] cpuP;
		}
	}
        alloc=t.alloc;
        if (alloc)
       	{
		typeCpu=t.typeCpu;
		typeAcc=t.typeAcc;
		cpuPmin=t.cpuPmin;
		cpuPmax=t.cpuPmax;
		cpuC=t.cpuC;
		numOfPoints=t.numOfPoints;
		if (numOfPoints>0)
		{
			cpubins=new double[numOfPoints];
			cpuP=new double[numOfPoints];
			for(i=0;i<numOfPoints;i++)
			{
				cpubins[i]=t.cpubins[i];
				cpuP[i]=t.cpuP[i];
			}
		}
		else
		{
			cpubins=NULL;
			cpuP=NULL;
		}
		accelerator=t.accelerator;
		accPmin=t.accPmin;
		accPmax=t.accPmax;
		accC=t.accC;	
	}
    }
    return *this;	
}

powinputs::~powinputs()
{
	if (alloc)
	{
		alloc=0;
		typeCpu=0;
		typeAcc=0;
		cpuPmin=0.0;
		cpuPmax=0.0;
		cpuC=0.0;
		accelerator=0;
		accPmin=0.0;
		accPmax=0.0;
		accC=0.0;		
		if (numOfPoints!=0)
		{
			numOfPoints=0;
			delete[] cpubins;
			delete[] cpuP;
		}
	}
}

resinputs::resinputs()
{
	alloc=0;
	numOfProcUnits=0.0;
	totMem=0.0;
	totSto=0.0;
	overCommitProc=0.0;
	overCommitMem=0.0;
	compCap=0.0;
	accelerator=0;
	accCompCap=0.0;
	totAcc=0;
	type=0;
}

resinputs::resinputs(const resinputs &t)
{
	if(t.alloc)
	{
		alloc=t.alloc;
		numOfProcUnits=t.numOfProcUnits;
		totMem=t.totMem;
		totSto=t.totSto;
		overCommitProc=t.overCommitProc;
		overCommitMem=t.overCommitMem;
		compCap=t.compCap;
		accelerator=t.accelerator;
		accCompCap=t.accCompCap;
		totAcc=t.totAcc;		
		type=t.type;		
	}
}

resinputs & resinputs::operator=(const resinputs &t)
{

    if (this!=&t)
    {
	if (alloc)
	{
		alloc=0;
		numOfProcUnits=0.0;
		totMem=0.0;
		totSto=0.0;
		overCommitProc=0.0;
		compCap=0.0;
		accelerator=0;
		accCompCap=0.0;
		totAcc=0;
		type=0;	
	}
        alloc=t.alloc;
        if (alloc)
        {
		alloc=t.alloc;
		numOfProcUnits=t.numOfProcUnits;
		totMem=t.totMem;
		totSto=t.totSto;
		overCommitProc=t.overCommitProc;
		overCommitMem=t.overCommitMem;
		compCap=t.compCap;
		accelerator=t.accelerator;
		accCompCap=t.accCompCap;
		totAcc=t.totAcc;
		type=t.type;		
	}
    }
    return *this;	
}

resinputs::~resinputs()
{
	if (alloc)
	{
		alloc=0;
		numOfProcUnits=0.0;
		totMem=0.0;
		totSto=0.0;
		overCommitProc=0.0;
		compCap=0.0;
		accelerator=0;
		accCompCap=0.0;
		totAcc=0;
		type=0;	
	}
}


cellinputs::cellinputs()
{
	alloc=0;
	ID=0;
	numOfTypes=0;
	types=NULL;
	numOfResourcesPerType=NULL;
	rinp=NULL;
	pinp=NULL;
	ninp=NULL;
	binp=NULL;
}

cellinputs::cellinputs(const cellinputs &t)
{
	int i;
	if (t.alloc)
	{
		ID=t.ID;
		alloc=t.alloc;
		numOfTypes=t.numOfTypes;
		types=new int[numOfTypes];
		numOfResourcesPerType=new int[numOfTypes];
		rinp=new resinputs[numOfTypes];
		pinp=new powinputs[numOfTypes];
		ninp=new netinputs[1];
		binp=new brinputs[1];
		ninp[0]=t.ninp[0];
		binp[0]=t.binp[0];
		for(i=0;i<numOfTypes;i++)
		{
			types[i]=t.types[i];
			numOfResourcesPerType[i]=t.numOfResourcesPerType[i];
			rinp[i]=t.rinp[i];
			pinp[i]=t.pinp[i];
		}

	}
}

cellinputs::~cellinputs()
{
	if (alloc)
	{	
		alloc=0;
		ID=0;
		numOfTypes=0;
		delete[] types;
		delete[] numOfResourcesPerType;
		delete[] rinp;
		delete[] pinp;
		delete[] ninp;
		delete[] binp;
	}
}

cellinputs & cellinputs::operator=(const cellinputs &t)
{
    int i;
    if (this!=&t)
    {
	if (alloc)
	{
		alloc=0;
		ID=0;
		numOfTypes=0;
		delete[] types;
		delete[] numOfResourcesPerType;
		delete[] rinp;
		delete[] pinp;
		delete[] ninp;
		delete[] binp;
	}
        alloc=t.alloc;
        if (alloc)
        {
		ID=t.ID;
		numOfTypes=t.numOfTypes;
		types=new int[numOfTypes];
		numOfResourcesPerType=new int[numOfTypes];
		rinp=new resinputs[numOfTypes];
		pinp=new powinputs[numOfTypes];
		ninp=new netinputs[1];
		binp=new brinputs[1];
		ninp[0]=t.ninp[0];
		binp[0]=t.binp[0];
		for(i=0;i<numOfTypes;i++)
		{
			types[i]=t.types[i];
			numOfResourcesPerType[i]=t.numOfResourcesPerType[i];
			rinp[i]=t.rinp[i];
			pinp[i]=t.pinp[i];
		}	
	}
    }
    return *this;	
}

siminputs::siminputs()
{
	alloc=0;
	numOfCells=0;
	maxTime=0.0;
	upInterval=0.0;
	cinp=NULL;
}

siminputs::siminputs(const siminputs &t)
{
	int i;
	if(t.alloc)
	{
		alloc=t.alloc;
		numOfCells=t.numOfCells;
		maxTime=t.maxTime;
		upInterval=t.upInterval;
		cinp=new cellinputs[numOfCells];
		for(i=0;i<numOfCells;i++)
			cinp[i]=t.cinp[i];
	}
}

siminputs::~siminputs()
{
	
	if (alloc)
	{
		alloc=0;
		numOfCells=0;
		maxTime=0.0;
		upInterval=0.0;
		delete[] cinp;
	}
}

siminputs & siminputs::operator=(const siminputs & t)
{
    int i;
    if (this!=&t)
    {
	if (alloc)
	{
		alloc=0;
		numOfCells=0;
		maxTime=0.0;
		upInterval=0.0;
		delete[] cinp;
	}
        alloc=t.alloc;
        if (alloc)
        {
		numOfCells=t.numOfCells;
		maxTime=t.maxTime;
		upInterval=t.upInterval;
		cinp=new cellinputs[numOfCells];
		for(i=0;i<numOfCells;i++)
			cinp[i]=t.cinp[i];	
	}
    }
    return *this;
}

void siminputs::parse(const string &fname, const string &bname)
{
	int i,j,k;
	double dummy;
	ifstream file,file2;
	

	file.open(fname.c_str());
	file2.open(bname.c_str());
	if (file && file2)
	{
		file>>maxTime;
		file>>upInterval;
		file>>numOfCells;

		cinp=new cellinputs[numOfCells];
		alloc=1;
		//Looping over all Cells from the serial file
		for(i=0;i<numOfCells;i++)
		{
			
			cinp[i].binp=new brinputs[1];
			cinp[i].binp[0].alloc=1;
			cinp[i].binp[0].parse(file2);
			file>>cinp[i].numOfTypes;
			cinp[i].alloc=1;
			cinp[i].ID=i+1;
			cinp[i].types=new int[cinp[i].numOfTypes];
			cinp[i].numOfResourcesPerType=new int[cinp[i].numOfTypes];
			cinp[i].rinp=new resinputs[cinp[i].numOfTypes];
			cinp[i].pinp=new powinputs[cinp[i].numOfTypes];
			cinp[i].ninp=new netinputs[1];
			cinp[i].ninp[0].alloc=1;
			file>>cinp[i].ninp[0].netBW;
			file>>cinp[i].ninp[0].overCommitNet;
			for(j=0;j<cinp[i].numOfTypes;j++)
			{
				file>>cinp[i].types[j];
				file>>cinp[i].numOfResourcesPerType[j];
				cinp[i].rinp[j].alloc=1;
				cinp[i].rinp[j].type=cinp[i].types[j];	
				file>>cinp[i].rinp[j].numOfProcUnits;
				file>>cinp[i].rinp[j].totMem;
				file>>cinp[i].rinp[j].totSto;
				file>>cinp[i].rinp[j].overCommitProc;
				file>>cinp[i].rinp[j].overCommitMem;
				file>>cinp[i].rinp[j].compCap;
				file>>cinp[i].rinp[j].accelerator;
				file>>cinp[i].rinp[j].totAcc;
				file>>cinp[i].rinp[j].accCompCap;
 				cinp[i].pinp[j].alloc=1;
				cinp[i].pinp[j].accelerator=cinp[i].rinp[j].accelerator;
				file>>cinp[i].pinp[j].typeCpu;
				file>>cinp[i].pinp[j].cpuPmin;
				file>>cinp[i].pinp[j].cpuPmax;
				file>>cinp[i].pinp[j].numOfPoints;
				if(cinp[i].pinp[j].numOfPoints>0)
				{
					cinp[i].pinp[j].cpubins=new double[cinp[i].pinp[j].numOfPoints];
					for(k=0;k<cinp[i].pinp[j].numOfPoints;k++)
						file>>cinp[i].pinp[j].cpubins[k];
					cinp[i].pinp[j].cpuP=new double[cinp[i].pinp[j].numOfPoints];
					for(k=0;k<cinp[i].pinp[j].numOfPoints;k++)
						file>>cinp[i].pinp[j].cpuP[k];
				}
				else
				{
					file>>dummy;
					file>>dummy;
				}
				file>>cinp[i].pinp[j].cpuC;
				file>>cinp[i].pinp[j].typeAcc;				
				file>>cinp[i].pinp[j].accPmin;
				file>>cinp[i].pinp[j].accPmax;
				file>>cinp[i].pinp[j].accC;
			}
		}
		file.close();
		file2.close();
	}
	else
	{
		cout<<endl<<"Catastrophic Error: File(s) not found"<<endl;
		exit(0);
	}
}


//Print Simulation Inputs (Mainly for Debugging)
void siminputs::print()
{
	int i,j,k;
	if (alloc)
	{
		cout<<"=========== Global Parameters ==========="<<endl;
		cout<<"Maximum Simulation Time: "<<maxTime<<" seconds"<<endl;
		cout<<"Update Interval: "<<upInterval<<" seconds"<<endl;
		cout<<endl;
		cout<<"=========== Cell Parameters ==========="<<endl;
		cout<<"Number of Cells: "<<numOfCells<<endl;
		cout<<"---------------------------------------"<<endl;
		//Looping over all Cells from the serial file
		for(i=0;i<numOfCells;i++)
		{
			if (cinp[i].alloc)
			{
				cout<<"    Cell: "<<cinp[i].ID<<endl;
				cout<<"         Number of HW Types: "<<cinp[i].numOfTypes<<endl;
				if(cinp[i].ninp[0].alloc)
				{
					cout<<"         Cell Interconnection Bandwidth: "<<cinp[i].ninp[0].netBW<<" Gbps"<<endl;
					cout<<"         Network Bandwidth Overcommitment ratio: "<<cinp[i].ninp[0].overCommitNet<<endl;
				}
				for(j=0;j<cinp[i].numOfTypes;j++)
				{
					if (cinp[i].rinp[j].alloc)
					{
						cout<<"              HW Type: "<<cinp[i].types[j]<<endl;
						cout<<"                   Number of Servers: "<<cinp[i].numOfResourcesPerType[j]<<endl;
						cout<<"                   Number of Proc. Units per Server:"<<cinp[i].rinp[j].numOfProcUnits<<endl;
						cout<<"                   Memory per Server: "<<cinp[i].rinp[j].totMem<<" GBytes"<<endl;
						cout<<"                   Storage per Server: "<<cinp[i].rinp[j].totSto<<" TBytes"<<endl;

						cout<<"                   Proc. Overcommitment ratio: "<<cinp[i].rinp[j].overCommitProc<<endl;
						cout<<"                   Memory Overcommitment ratio: "<<cinp[i].rinp[j].overCommitMem<<endl;
						cout<<setprecision(10)<<"                   Compute Capability: "<<cinp[i].rinp[j].compCap<< " MIPS"<<endl;
						cout<<"                   Accelerators: "<<cinp[i].rinp[j].accelerator<<endl;
						cout<<"                   Total Number of Accelerators per Server: "<<cinp[i].rinp[j].totAcc<<endl;
						cout<<"                   Accelerator Compute Capability: "<<cinp[i].rinp[j].accCompCap<<" MIPS"<<endl;
					}
					if(cinp[i].pinp[j].alloc)
					{
						cout<<"                   Type of CPU model: "<<cinp[i].pinp[j].typeCpu<<endl;
						if (cinp[i].pinp[j].typeCpu<0)
						{
							cout<<"                   CPU Idle Power Consumption: "<<cinp[i].pinp[j].cpuPmin<<" Watts"<<endl;
							cout<<"                   CPU Max Power Consumption: "<<cinp[i].pinp[j].cpuPmax<<" Watts"<<endl;
						}
						else if (cinp[i].pinp[j].typeCpu>0)
						{
							cout<<"                   CPU Number of Points for Interpolation: "<<cinp[i].pinp[j].numOfPoints<<endl;
							cout<<"                   CPU Utilization Bins: ";
							for(k=0;k<cinp[i].pinp[j].numOfPoints;k++)
								cout<<cinp[i].pinp[j].cpubins[k]<<" ";
							cout<<endl;
							cout<<"                   CPU Power Consuption: ";
							for(k=0;k<cinp[i].pinp[j].numOfPoints;k++)
								cout<<cinp[i].pinp[j].cpuP[k]<<" ";
							cout<<endl;
						}
						cout<<"                   CPU Sleep Power Consumption: "<<cinp[i].pinp[j].cpuC<<" Watts"<<endl;
						if(cinp[i].pinp[j].accelerator)
						{
							cout<<"                   Type of Accelerator Model: "<<cinp[i].pinp[j].typeAcc<<endl;
							cout<<"                   Accelerator Idle Power Consumption: "<<cinp[i].pinp[j].accPmin<<" Watts"<<endl;
							cout<<"                   Accelerator Max Power Consumption: "<<cinp[i].pinp[j].accPmax<<" Watts"<<endl;
							cout<<"                   Accelerator Sleep Power Consumption: "<<cinp[i].pinp[j].accC<<" Watts"<<endl;

						}
					}
				}
				if (cinp[i].binp[0].alloc)
				{
					cinp[i].binp[0].print();
				}
			}
		}
		cout<<"---------------------------------------"<<endl;
		cout<<endl;
	}
}

void siminputs::printfile(const string &outname, const ios::openmode &mode)
{
	int i,j,k;
	fstream file;
	if (alloc)
	{
		file.open(outname.c_str(),mode);

		file<<"=========== Global Parameters ==========="<<endl;
		file<<"Maximum Simulation Time: "<<maxTime<<" seconds"<<endl;
		file<<"Update Interval: "<<upInterval<<" seconds"<<endl;
		file<<endl;
		file<<"=========== Cell Parameters ==========="<<endl;
		file<<"Number of Cells: "<<numOfCells<<endl;
		file<<"---------------------------------------"<<endl;
		//Looping over all Cells from the serial file
		for(i=0;i<numOfCells;i++)
		{
			if (cinp[i].alloc)
			{
				file<<"    Cell: "<<cinp[i].ID<<endl;
				file<<"         Number of HW Types: "<<cinp[i].numOfTypes<<endl;
				if(cinp[i].ninp[0].alloc)
				{
					file<<"         Cell Interconnection Bandwidth: "<<cinp[i].ninp[0].netBW<<" Gbps"<<endl;
					file<<"         Network Bandwidth Overcommitment ratio: "<<cinp[i].ninp[0].overCommitNet<<endl;
				}
				for(j=0;j<cinp[i].numOfTypes;j++)
				{
					if (cinp[i].rinp[j].alloc)
					{
						file<<"              HW Type: "<<cinp[i].types[j]<<endl;
						file<<"                   Number of Servers: "<<cinp[i].numOfResourcesPerType[j]<<endl;
						file<<"                   Number of Proc. Units per Server:"<<cinp[i].rinp[j].numOfProcUnits<<endl;
						file<<"                   Memory per Server: "<<cinp[i].rinp[j].totMem<<" GBytes"<<endl;
						file<<"                   Storage per Server: "<<cinp[i].rinp[j].totSto<<" TBytes"<<endl;

						file<<"                   Proc. Overcommitment ratio: "<<cinp[i].rinp[j].overCommitProc<<endl;
						file<<"                   Memory Overcommitment ratio: "<<cinp[i].rinp[j].overCommitMem<<endl;
						file<<setprecision(10)<<"                   Compute Capability: "<<cinp[i].rinp[j].compCap<< " MIPS"<<endl;
						file<<"                   Accelerators: "<<cinp[i].rinp[j].accelerator<<endl;
						file<<"                   Total Number of Accelerators per Server: "<<cinp[i].rinp[j].totAcc<<endl;
						file<<"                   Accelerator Compute Capability: "<<cinp[i].rinp[j].accCompCap<<" MIPS"<<endl;
					}
					if(cinp[i].pinp[j].alloc)
					{
						file<<"                   Type of CPU model: "<<cinp[i].pinp[j].typeCpu<<endl;
						if (cinp[i].pinp[j].typeCpu<0)
						{
							file<<"                   CPU Idle Power Consumption: "<<cinp[i].pinp[j].cpuPmin<<" Watts"<<endl;
							file<<"                   CPU Max Power Consumption: "<<cinp[i].pinp[j].cpuPmax<<" Watts"<<endl;
						}
						else if (cinp[i].pinp[j].typeCpu>0)
						{
							file<<"                   CPU Number of Points for Interpolation: "<<cinp[i].pinp[j].numOfPoints<<endl;
							file<<"                   CPU Utilization Bins: ";
							for(k=0;k<cinp[i].pinp[j].numOfPoints;k++)
								file<<cinp[i].pinp[j].cpubins[k]<<" ";
							file<<endl;
							file<<"                   CPU Power Consuption: ";
							for(k=0;k<cinp[i].pinp[j].numOfPoints;k++)
								file<<cinp[i].pinp[j].cpuP[k]<<" ";
							file<<endl;
						}
						file<<"                   CPU Sleep Power Consumption: "<<cinp[i].pinp[j].cpuC<<" Watts"<<endl;
						if(cinp[i].pinp[j].accelerator)
						{
							file<<"                   Type of Accelerator Model: "<<cinp[i].pinp[j].typeAcc<<endl;
							file<<"                   Accelerator Idle Power Consumption: "<<cinp[i].pinp[j].accPmin<<" Watts"<<endl;
							file<<"                   Accelerator Max Power Consumption: "<<cinp[i].pinp[j].accPmax<<" Watts"<<endl;
							file<<"                   Accelerator Sleep Power Consumption: "<<cinp[i].pinp[j].accC<<" Watts"<<endl;

						}
					}
				}
				file.close();
				if (cinp[i].binp[0].alloc)
				{
					cinp[i].binp[0].printfile(outname,ios::out | ios::app);
				}
				file.open(outname.c_str(),ios::out | ios::app);
				
			}
		}
		file<<"---------------------------------------"<<endl;
		file<<endl;
		file.close();
	}	
}

