#include <cell.h>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <inputs.h>
#include <power.h>
#include <netw.h>
#include <brokers.h>
#include <stat.h>
using namespace std;

//---------------------------------
//        CONSTRUCTORS
//---------------------------------
cell::cell()
{
	ID=0;
	alloc=0;
	types=NULL;
	numOfTypes=0;
	numOfResourcesPerType=NULL;
	resources=NULL;
	powerComp=NULL;
	network=NULL;
	brok=NULL;
	stats=NULL;
}

cell::cell(const cellinputs & setup)
{
	int i,j;
	alloc=1;
	ID=setup.ID;
	numOfTypes=setup.numOfTypes;
	types=new int[numOfTypes];
	numOfResourcesPerType=new int[numOfTypes];
	for(i=0;i<numOfTypes;i++)
	{
		types[i]=setup.types[i];
		numOfResourcesPerType[i]=setup.numOfResourcesPerType[i];
	}
	resources=new resource*[numOfTypes];
	for(i=0;i<numOfTypes;i++)
	{
		resources[i]=new resource[numOfResourcesPerType[i]];
	}
	for(i=0;i<numOfTypes;i++)
	{
		for(j=0;j<numOfResourcesPerType[i];j++)
			resources[i][j]=resource(setup.rinp[i],j);
	}
	powerComp=new power[numOfTypes];
	for(i=0;i<numOfTypes;i++)
		powerComp[i]=power(setup.pinp[i]);	
	network=new netw[1];
	network[0]=netw(setup.ninp[0]);

	brok=new broker[1];
//	brok[0]=broker(numOfTypes,types,numOfResourcesPerType,resources,powerComp,network,setup.binp[0]);

	stats=new stat[numOfTypes];
	for(i=0;i<numOfTypes;i++)
		stats[i]=stat();

	//brok->updateStateInfo(network,0.0);
	updateStats(0.0);
}

cell::cell(const cell & t)
{
	int i,j;
	if (t.galloc())
	{
		ID=t.gID();
		alloc=t.galloc();
		numOfTypes=t.gnumOfTypes();
		types=new int[numOfTypes];
		numOfResourcesPerType=new int[numOfTypes];
		for(i=0;i<numOfTypes;i++)
		{
			types[i]=t.gtypes()[i];
			numOfResourcesPerType[i]=t.gnumOfResourcesPerType()[i];
		}
		resources=new resource*[numOfTypes];
		for(i=0;i<numOfTypes;i++)
		{
			resources[i]=new resource[numOfResourcesPerType[i]];
		}
		for(i=0;i<numOfTypes;i++)
		{
			for(j=0;j<numOfResourcesPerType[i];j++)
				resources[i][j]=t.gresources()[i][j];
		}
		powerComp=new power[numOfTypes];
		for(i=0;i<numOfTypes;i++)
			powerComp[i]=t.gpowerComp()[i];	
		network=new netw[1];
		network[0]=t.gnetwork()[0];
		brok=new broker[1];
		brok[0]=t.gbrok()[0];
		stats=new stat[numOfTypes];
		for(i=0;i<numOfTypes;i++)
			stats[i]=t.gstats()[i];
	}
}

//---------------------------------
//    ASSIGNMENT OPERATOR
//---------------------------------
cell & cell::operator=(const cell & t)
{
    int i,j;
    if (this!=&t)
    {
	if (alloc)
	{
		ID=0;
		alloc=0;
		delete[] types;
		types=NULL;
		delete[] numOfResourcesPerType;
		numOfResourcesPerType=NULL;
		for(i=0;i<numOfTypes;i++)
		{
			delete[] resources[i];
		}		
		delete[] resources;
		delete[] powerComp;
		delete[] network;
		delete[] brok;
		delete[] stats;
		resources=NULL;
		powerComp=NULL;
		network=NULL;
		brok=NULL;
		stats=NULL;
		numOfTypes=0;
	}
        alloc=t.galloc();
        if (alloc)
        {
		ID=t.gID();
		numOfTypes=t.gnumOfTypes();
		types=new int[numOfTypes];
		numOfResourcesPerType=new int[numOfTypes];
		for(i=0;i<numOfTypes;i++)
		{
			types[i]=t.gtypes()[i];
			numOfResourcesPerType[i]=t.gnumOfResourcesPerType()[i];
		}

		resources=new resource*[numOfTypes];
		for(i=0;i<numOfTypes;i++)
		{
			resources[i]=new resource[numOfResourcesPerType[i]];
		}
		for(i=0;i<numOfTypes;i++)
		{
			for(j=0;j<numOfResourcesPerType[i];j++)
				resources[i][j]=t.gresources()[i][j];
		}
		powerComp=new power[numOfTypes];
		for(i=0;i<numOfTypes;i++)
			powerComp[i]=t.gpowerComp()[i];
		network=new netw[1];
		network[0]=t.gnetwork()[0];
		brok=new broker[1];
		brok[0]=t.gbrok()[0];
		stats=new stat[numOfTypes];
		for(i=0;i<numOfTypes;i++)
			stats[i]=t.gstats()[i];
	}
    }
    return *this;
}

//---------------------------------
//      DECONSTRUCTOR
//---------------------------------
cell::~cell()
{
	int i;
	if (alloc)
	{
		ID=0;
		alloc=0;

		delete[] types;
		delete[] numOfResourcesPerType;
		types=NULL;
		numOfResourcesPerType=NULL;
		for(i=0;i<numOfTypes;i++)
		{
			delete[] resources[i];
		}		
		delete[] resources;
		delete[] powerComp;
		delete[] network;
		delete[] brok;
		delete[] stats;	
		resources=NULL;	
		powerComp=NULL;
		network=NULL;
		brok=NULL;
		stats=NULL;
		numOfTypes=0;
	}
}

//---------------------------------
//        TIMESTEPPING
//---------------------------------
void cell::timestep(const double &tstep)
{
	if (alloc)
	{
		brok[0].timestep(resources,network,stats,powerComp);
		brok->updateStateInfo(network,tstep);
		updateStats(tstep);
	}
}

//---------------------------------
//        HELPERS
//---------------------------------


//---------------------------------
//        GATEWAYS
//---------------------------------
int cell::gID() const
{
	return ID;
}
int cell::galloc() const
{
	return alloc;
}

int cell::gnumOfTypes() const
{
	return numOfTypes;
}

int *cell::gtypes() const
{
	return types;
}

int *cell::gnumOfResourcesPerType() const
{
	return numOfResourcesPerType;
}
 
resource **cell::gresources() const
{
	return resources;
}

power *cell::gpowerComp() const
{
	return powerComp;
}

broker *cell::gbrok() const
{
	return brok;
}

netw *cell::gnetwork() const
{
	return network;
}

stat *cell::gstats() const
{
	return stats;
}

//---------------------------------
//           DEPLOY
//---------------------------------
void cell::deploy(list<task> *jobs)
{
	list<task>::iterator it;
	if (alloc)
	{
		for(it=jobs->begin();it!=jobs->end();it++)
		{
				
			brok[0].deploy(resources,network,stats,&(*it));	
		}
		
	}
}

//---------------------------------
//        UPDATE STATS
//---------------------------------
void cell::updateStats(const double &tstep)
{
	int i,j;
	double phyMem, phyProc, phySto, totMem, totProc, availProc, availMem, totSto, availSto, autilProc, autilMem;
	int totAcc,availAcc,activeSrvs,numOfTasks, procActServs, procPerServ, memActServs, memPerServ, stoActServs, stoPerServ, accActServs, accPerServ;
	int omp_thr=atoi(getenv("OMP_NUM_THREADS"));
	for(i=0;i<numOfTypes;i++)
	{
		stats[i].alloc=1;
		stats[i].currTstep=tstep;

		stats[i].totNetw=network[0].gtotalNetw();
		stats[i].availNetw=network[0].gavailNetw();
		stats[i].utilNetw=stats[i].totNetw-stats[i].availNetw;
		stats[i].autilNetw=network[0].gautilNetw();

		procPerServ=resources[i][0].gtotalProc(); 		// Processors per server, e.g. --> 44
		memPerServ=resources[i][0].gtotalMem();			// Memory capacity per server, e.g. --> 128 GB
		stoPerServ=resources[i][0].gtotalSto(); 		// Storage per server, e.g. --> 40 TBs
		accPerServ=resources[i][0].gtotAcc();			// Accelerators per server, e.g. --> 4
		procActServs=0;
		memActServs=0;
		stoActServs=0;
		accActServs=0;

		phyMem=0.0;
		phyProc=0.0;
		phySto=0.0;
		totMem=0.0;
		totProc=0.0;
		availProc=0.0;
		availMem=0.0;

		totSto=0.0;
		availSto=0.0;
		totAcc=0;
		availAcc=0;
		activeSrvs=0;
		numOfTasks=0;

		autilProc=0.0;
		autilMem=0.0;


		#pragma omp parallel for default(shared) private(j) num_threads(omp_thr) schedule(static) reduction(+:phyProc,totProc,availProc,phyMem,totMem,availMem,phySto,totSto,availSto,totAcc,availAcc,activeSrvs,numOfTasks,autilProc,autilMem)
		for(j=0;j<numOfResourcesPerType[i];j++)
		{				
			phyProc+=resources[i][j].gphysProc();
			totProc+=resources[i][j].gtotalProc();
			availProc+=resources[i][j].gavailProc();
			phyMem+=resources[i][j].gphysMem();
			totMem+=resources[i][j].gtotalMem();
			availMem+=resources[i][j].gavailMem();
			phySto+=resources[i][j].gphysSto();
			totSto+=resources[i][j].gtotalSto();
			availSto+=resources[i][j].gavailSto();
			totAcc+=resources[i][j].gtotAcc();
			availAcc+=resources[i][j].gavailAcc();
			activeSrvs+=resources[i][j].gactive();
			numOfTasks+=resources[i][j].gnumOfTasks();

			autilProc+=resources[i][j].gautilProc();
			autilMem+=resources[i][j].gautilMem();
		}
		stats[i].phyProc=phyProc;
		stats[i].totProc=totProc;
		stats[i].availProc=availProc;
		stats[i].utilProc=totProc-availProc;
		stats[i].phyMem=phyMem;
		stats[i].totMem=totMem;
		stats[i].availMem=availMem;
		stats[i].utilMem=totMem-availMem;
		stats[i].phySto=phySto;
		stats[i].totSto=totSto;
		stats[i].availSto=availSto;
		stats[i].utilSto=totSto-availSto;
		stats[i].totAcc=totAcc;
		stats[i].availAcc=availAcc;
		stats[i].utilAcc=totAcc-availAcc;
		stats[i].activeSrvs=activeSrvs;
		stats[i].numOfTasks=numOfTasks;
		stats[i].autilProc=autilProc;
		stats[i].autilMem=autilMem;

		procActServs=activeSrvs*procPerServ;
		memActServs=activeSrvs*memPerServ;
		stoActServs=activeSrvs*stoPerServ;
		accActServs=activeSrvs*accPerServ;

		stats[i].procActServs=procActServs;
		stats[i].memActServs=memActServs;
		stats[i].stoActServs=stoActServs;
		stats[i].accActServs=accActServs;
		
	}
}

//---------------------------------
//        PRINT
//---------------------------------
void cell::print()
{
	int i;
	
	if(alloc)
	{
		cout<<endl;
		cout<<"Cell ID: "<<ID<<endl;
		cout<<"Number of HW types: "<<numOfTypes<<endl;
		cout<<"HW types: ";
		for(i=0;i<numOfTypes;i++)
			cout<<types[i]<<" ";
		cout<<endl;
		cout<<"Number of Resources Per Type: ";
		for(i=0;i<numOfTypes;i++)
			cout<<numOfResourcesPerType[i]<<" ";
		cout<<endl;
		cout<<"---------------------------------------------"<<endl;
		network[0].print();
		brok[0].print();
		for(i=0;i<numOfTypes;i++)
		{
			cout<<"     Resource Type: "<<types[i]<<endl;
			powerComp[i].print();
			stats[i].print();
		}
				
	}
}

