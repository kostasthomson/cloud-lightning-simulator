#include<stat.h>
#include<iostream>
#include<fstream>
#include<string>
#include<json.hpp>
using namespace std;

nlohmann::json js;
nlohmann::json cl;
nlohmann::json hw;

nlohmann::json hw_list;
nlohmann::json js_list;
nlohmann::json output_list;
nlohmann::json cl_list;

//---------------------------------
//        CONSTRUCTORS
//---------------------------------
stat::stat()
{
	alloc=0;
	currTstep=0.0;
	phyMem=0.0;
	phyProc=0.0;
	phySto=0.0;
	phyNetw=0.0;
	totMem=0.0;
	totProc=0.0;
	availProc=0.0;
	availMem=0.0;
	utilProc=0.0;
	utilMem=0.0;
	totSto=0.0;
	availSto=0.0;
	utilSto=0.0;
	totNetw=0.0;
	availNetw=0.0;
	utilNetw=0.0;
	totPcons=0.0;
	
	totAcc=0;
	availAcc=0;
	utilAcc=0;
	activeSrvs=0;
	numOfTasks=0;
	rejTasks=0;
	accTasks=0;

	autilProc=0.0;
	autilMem=0.0;
	autilNetw=0.0;

	
}

stat::stat(const stat &t)
{
	if(t.alloc)
	{
		alloc=t.alloc;
		currTstep=t.currTstep;
		phyMem=t.phyMem;
		phyProc=t.phyProc;
		phySto=t.phySto;
		phyNetw=t.phyNetw;
		totMem=t.totMem;
		totProc=t.totProc;
		availProc=t.availProc;
		availMem=t.availMem;
		utilProc=t.utilProc;
		utilMem=t.utilMem;
		totSto=t.totSto;
		availSto=t.availSto;
		utilSto=t.utilSto;
		totNetw=t.totNetw;
		availNetw=t.availNetw;
		utilNetw=t.utilNetw;
		totPcons=t.totPcons;

		totAcc=t.totAcc;
		availAcc=t.availAcc;
		utilAcc=t.utilAcc;
		activeSrvs=t.activeSrvs;
		numOfTasks=t.numOfTasks;
		rejTasks=t.rejTasks;
		accTasks=t.accTasks;

		autilProc=t.autilProc;
		autilMem=t.autilMem;
		autilNetw=t.autilNetw;
	}
}

stat & stat::operator=(const stat &t)
{
	if(this!=&t)
	{
		if(alloc)
		{
			alloc=0;
			currTstep=0.0;
			phyMem=0.0;
			phyProc=0.0;
			phySto=0.0;
			phyNetw=0.0;
			totMem=0.0;
			totProc=0.0;
			availProc=0.0;
			availMem=0.0;
			utilProc=0.0;
			utilMem=0.0;
			totSto=0.0;
			availSto=0.0;
			utilSto=0.0;
			totNetw=0.0;
			availNetw=0.0;
			utilNetw=0.0;
			totPcons=0.0;

			totAcc=0;
			availAcc=0;
			utilAcc=0;
			activeSrvs=0;
			numOfTasks=0;
			rejTasks=0;
			accTasks=0;

			autilProc=0.0;
			autilMem=0.0;
			autilNetw=0.0;
		}
		alloc=t.alloc;
		if(alloc)
		{
			currTstep=t.currTstep;
			phyMem=t.phyMem;
			phyProc=t.phyProc;
			phySto=t.phySto;
			phyNetw=t.phyNetw;
			totMem=t.totMem;
			totProc=t.totProc;
			availProc=t.availProc;
			availMem=t.availMem;
			utilProc=t.utilProc;
			utilMem=t.utilMem;
			totSto=t.totSto;
			availSto=t.availSto;
			utilSto=t.utilSto;
			totNetw=t.totNetw;
			availNetw=t.availNetw;
			utilNetw=t.utilNetw;
			totPcons=t.totPcons;

			totAcc=t.totAcc;
			availAcc=t.availAcc;
			utilAcc=t.utilAcc;
			activeSrvs=t.activeSrvs;
			numOfTasks=t.numOfTasks;
			rejTasks=t.rejTasks;
			accTasks=t.accTasks;

			autilProc=t.autilProc;
			autilMem=t.autilMem;
			autilNetw=t.utilNetw;
		}
	}
	return *this;
}

stat::~stat()
{
	if(alloc)
	{
		alloc=0;
		currTstep=0.0;
		phyMem=0.0;
		phyProc=0.0;
		phySto=0.0;
		phyNetw=0.0;
		totMem=0.0;
		totProc=0.0;
		availProc=0.0;
		availMem=0.0;
		utilProc=0.0;
		utilMem=0.0;
		totSto=0.0;
		availSto=0.0;
		utilSto=0.0;
		totNetw=0.0;
		availNetw=0.0;
		utilNetw=0.0;
		totPcons=0.0;

		totAcc=0;
		availAcc=0;
		utilAcc=0;
		activeSrvs=0;
		numOfTasks=0;
		rejTasks=0;
		accTasks=0;

		autilProc=0.0;
		autilMem=0.0;
		autilNetw=0.0;
	}
}


void stat::print() const
{
	if(alloc)
	{
		cout<<"         Active Servers: "<<activeSrvs<<endl;
		cout<<"         Time Step: "<<currTstep<<endl;
		cout<<"         Total Number of currently running VMs: "<<numOfTasks<<endl;
		cout<<"         Total Number of accepted Tasks: "<<accTasks<<endl;
		cout<<"         Total Number of rejected Tasks: "<<rejTasks<<endl;
		cout<<"           Total Physical Processors: "<<phyProc<<" Proc. Units"<<endl;
		cout<<"           Total Processors: "<<totProc<<" Proc. Units"<<endl;
		cout<<"           Utilized Processors: "<<utilProc<<" Proc. Units"<<endl;
		cout<<"           Actual Utilized Processors: "<<autilProc<<" Proc. Units"<<endl;
		cout<<"           Available Processors: "<<availProc<<" Proc. Units"<<endl;
		cout<<"           Total Physical Memory: "<<phyMem<<" GBytes"<<endl;
		cout<<"           Total Memory: "<<totMem<<" GBytes"<<endl;
		cout<<"           Utilized Memory: "<<utilMem<<" GBytes"<<endl;
		cout<<"           Actual Utilized Memory: "<<autilMem<<" Proc. Units"<<endl;
		cout<<"           Available Memory: "<<availMem<<" GBytes"<<endl;
		cout<<"           Total Physical Storage: "<<phySto<<" TBytes"<<endl;
		cout<<"           Total Storage: "<<totSto<<" TBytes"<<endl;
		cout<<"           Utilized Storage: "<<utilSto<<" TBytes"<<endl;
		cout<<"           Available Storage: "<<availSto<<" TBytes"<<endl;
		cout<<"           Total Physical Network: "<<phyNetw<<" Gbps"<<endl;
		cout<<"           Total Network: "<<totNetw<<" Gbps"<<endl;
		cout<<"           Utilized Network: "<<utilNetw<<" Gbps"<<endl;
		cout<<"           Actual Utilized Network: "<<autilNetw<<" Gbps"<<endl;
		cout<<"           Available Network: "<<availNetw<<" Gbps"<<endl;
		cout<<"           Total Energy Consumption: "<<totPcons<<" GWh"<<endl;
		cout<<"           Total Accelerators: "<<totAcc<<endl;
		cout<<"           Utilized Accelerators: "<<utilAcc<<endl;
		cout<<"           Available Accelerators: "<<availAcc<<endl;
	}	
}

void stat::printfile(const string &outfile, const ios::openmode &mode)
{
	fstream file;
	if (alloc)
	{
		file.open(outfile.c_str(),mode);
		file<<currTstep<<" "<<activeSrvs<<" "<<numOfTasks<<" "<<accTasks<<" "<<rejTasks<<" "<<availProc<<" "<<utilProc<<" "<<autilProc<<" "<<totProc<<" "<<phyProc<<" "<<availMem<<" "<<utilMem<<" "<<autilMem<<" "<<totMem<<" "<<phyMem<<" "<<availSto<<" "<<utilSto<<" "<<availSto<<" "<<phySto<<" "<<availNetw<<" "<<utilNetw<<" "<<autilNetw<<" "<<totNetw<<" "<<phyNetw<<" "<<availAcc<<" "<<utilAcc<<" "<<totAcc<<" "<<totPcons<<endl;
		file.close();
	}
}

void stat::printfileJson(const string &outfile, const string &inputfile, const ios::openmode &mode, int a, int b, int overallRecords, int numOfCells, int numOfTypes, int j){

	ifstream file;
	file.open(inputfile.c_str());
	std::ofstream ff(outfile,mode);
    output_list.clear();
    hw_list.clear();

    int k;
    for(k=0;k<overallRecords;k++)
    {
    	file>>currTstep;
		file>>activeSrvs;
		file>>numOfTasks;
		file>>accTasks;
		file>>rejTasks;
		file>>availProc;
	    file>>utilProc;	
		file>>autilProc;
		file>>totProc;
		file>>phyProc;
		file>>availMem;
		file>>utilMem;
		file>>autilMem;
		file>>totMem;
		file>>phyMem;
		file>>availSto;
		file>>utilSto;
		file>>totSto;
		file>>phySto;
		file>>availNetw;
		file>>utilNetw;
		file>>autilNetw;
		file>>totNetw;
		file>>phyNetw;
		file>>availAcc;
		file>>utilAcc;
		file>>totAcc;
		file>>totPcons;
    
	
		js = {
				
				{"Time Step", currTstep},
				{"Total Energy Consumption",totPcons},
			  	{"Active Servers", activeSrvs},
				{"Total Number of currently running VMs",numOfTasks},
				{"Total Number of accepted Tasks",accTasks},
				{"Total Number of rejected Tasks",rejTasks},
				{"Total Physical Processors",phyProc},
				{"Total Processors",totProc},
				{"Utilized Processors",utilProc},
				{"Actual Utilized Processors",autilProc},
				{"Available Processors",availProc},
				{"Total Physical Memory",phyMem},
				{"Total Memory",totMem},
				{"Utilized Memory",utilMem},
				{"Actual Utilized Memory",autilMem},
				{"Available Memory",availMem},
				{"Total Physical Storage",phySto},
				{"Total Storage",totSto},
				{"Utilized Storage",utilSto},
				{"Available Storage",availSto},
				{"Total Physical Network",phyNetw},
				{"Total Network",totNetw},
				{"Utilized Network",utilNetw},
				{"Actual Utilized Network",autilNetw},
				{"Available Network",availNetw},
				{"Total Accelerators",totAcc},
				{"Utilized Accelerators",utilAcc},
				{"Available Accelerators",availAcc}	
				
		};
		output_list.push_back(js);
    }
    //hw = 
    //{
    //	{"HW Type", j},
    //	{"Outputs: ", output_list}
   // };
   // hw_list.push_back(hw);

	cl = 
    {
    	{"Cell", a},
    	{"HW Type", b},
    	{"Outputs", output_list}
    };

    cl_list.push_back(cl);

	if(a==numOfCells && j==numOfTypes)
    ff << std::setw(4) << cl_list << std::endl;
 file.close();
}