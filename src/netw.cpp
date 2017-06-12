#include <task.h>
#include <iostream>
#include <cstdlib>
#include <inputs.h>
#include <netw.h>
#include <iterator>
#include <list>
using namespace std;

//---------------------------------
//        CONSTRUCTORS
//---------------------------------
netw::netw()
{
	alloc=0;
	numTasks=0;
	phyNetw=0.0;
	totalNetw=0.0;
	utilNetw=0.0;
	availNetw=0.0;
	autilNetw=0.0;
	overCommitNet=1.0;
}

netw::netw(const netinputs & setup)
{
	alloc=1;
	numTasks=0;
	phyNetw=setup.netBW;
	totalNetw=setup.netBW*setup.overCommitNet;
	utilNetw=0.0;
	availNetw=totalNetw;
	autilNetw=0.0;
	overCommitNet=setup.overCommitNet;
}

netw::netw(const netw & t)
{
	if(t.galloc())
	{
		alloc=t.galloc();
		numTasks=t.gnumTasks();
		phyNetw=t.gphyNetw();
		totalNetw=t.gtotalNetw();
		utilNetw=t.gutilNetw();
		availNetw=t.gavailNetw();
		autilNetw=t.gautilNetw();
		overCommitNet=t.goverCommitNet();
	}
}

netw & netw::operator=(const netw & t)
{
	if(this!=&t)
	{
		if(alloc)
		{
			alloc=0;
			numTasks=0;
			phyNetw=0.0;
			totalNetw=0.0;
			utilNetw=0.0;
			availNetw=0.0;			
			autilNetw=0.0;
			overCommitNet=1.0;
		}
		alloc=t.galloc();
		if(alloc)
		{
			numTasks=t.gnumTasks();
			phyNetw=t.gphyNetw();
			totalNetw=t.gtotalNetw();
			utilNetw=t.gutilNetw();
			availNetw=t.gavailNetw();
			autilNetw=t.gautilNetw();
			overCommitNet=t.goverCommitNet();
		}
	}
	return *this;
}

netw::~netw()
{
	if(alloc)
	{
		alloc=0;
		numTasks=0;
		phyNetw=0.0;
		totalNetw=0.0;
		utilNetw=0.0;
		availNetw=0.0;
		autilNetw=0.0;
		overCommitNet=1.0;
	}
}

int netw::galloc() const
{
	return alloc;
}

double netw::gphyNetw() const
{
	return phyNetw;
}

double netw::gtotalNetw() const
{
	return totalNetw;
}

double netw::gutilNetw() const
{
	return utilNetw;
}

double netw::gavailNetw() const
{
	return availNetw;
}

double netw::gautilNetw() const
{
	return autilNetw;
}

int netw::gnumTasks() const
{
	return numTasks;
}

double netw::goverCommitNet() const
{
	return overCommitNet;
}

void netw::initRunQuan()
{
	if(alloc)
	{
		autilNetw=0.0;
	}
}

void netw::incrRunQuan(const double &uNetw)
{
	if(alloc)
	{
		autilNetw+=uNetw;
	}
}

void netw::sutilNetw(const double &L_utilNetw)
{
	utilNetw=L_utilNetw;
	availNetw=(totalNetw-utilNetw)<0?0:(totalNetw-utilNetw);
}

void netw::print() const
{
	if (alloc)
	{
		cout<<"     Utilized Interconnection Bandwidth: "<<utilNetw<<" Gbps"<<endl;
		cout<<"     Available Interconnection Bandwidth: "<<availNetw<<" Gbps"<<endl;
		cout<<"     Total Interconnection Bandwidth: "<<totalNetw<<" Gbps"<<endl;
		cout<<"     Physical Interconnection Bandwidth: "<<phyNetw<<" Gbps"<<endl;
		cout<<"     Interconnection Bandwidth Overcommitment ratio: "<<overCommitNet<<endl;
	}
}

int netw::probe(const double &reqNetw) const
{
	int choice=-1;
	if(reqNetw<=availNetw)
		choice=1;
	return choice;

}

void netw::deploy(const task * t)
{
	availNetw-=t->greqPMNS()[2];
	utilNetw=totalNetw-availNetw;
	numTasks++;
}

void netw::unload(const double &L_availNetw,const double &L_autilNetw,const int &L_numtasks)
{
	availNetw+=L_availNetw;
	utilNetw=totalNetw-availNetw;
	autilNetw-=L_autilNetw;
	numTasks-=L_numtasks;
	if (numTasks==0)
	{
		availNetw=totalNetw;
		utilNetw=0.0;
		autilNetw=0.0;
	}	
}

void netw::unload(list<task>::iterator &t)
{
	availNetw+=t->greqPMNS()[2];
	utilNetw=totalNetw-availNetw;
	autilNetw-=t->gcUtilPMNr()[2];
	numTasks--;
	if (numTasks==0)
	{
		availNetw=totalNetw;
		utilNetw=0.0;
		autilNetw=0.0;
	}
}


