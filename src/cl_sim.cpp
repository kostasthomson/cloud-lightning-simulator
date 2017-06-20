#include <mpi.h>
#include <task.h>
#include <resource.h>
#include <inputs.h>
#include <comm.h>
#include <cell.h>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <task.h>
#include <tce.h>
#include <gs.h>
#include <vector>
#include <iomanip>
#include <json.hpp>
using namespace std;

int main(int argc, char **argv)
{
  //Pointer to siminputs
	gs *gates=nullptr;

	// Cell
	cell *clCell=nullptr;

	//Current Job List
	list<task> jobs;

	// Initialize MPI
	int rc=MPI_Init(&argc,&argv);
	if (rc != MPI_SUCCESS)
	{
		cout<<"Error starting MPI program. Terminating..."<<endl;
		MPI_Abort(MPI_COMM_WORLD,rc);
	}

	// Get ranks and cluster size
	int numtasks = 0, rank = 0, len = 0;
  char hostname[MPI_MAX_PROCESSOR_NAME];
  MPI_Comm Comm;

  MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Get_processor_name(hostname,&len);
	MPI_Comm_dup(MPI_COMM_WORLD,&Comm);

  unsigned int omp_thr = std::max(atoi(std::getenv("OMP_NUM_THREADS")), 1);

	// Print cluster info
  double startTime=0.0;
	if (rank==0)
	{
		startTime=MPI_Wtime();
		cout<< endl <<"INITIALIZATION PHASE"<<endl;
		cout<<"------------------------------- "<<endl;
		cout<<"Cluster Size        : "<<numtasks<<endl;
		cout<<"Threads per machine : "<<omp_thr<<endl;
		cout<<"------------------------------- "<<endl;
	}
	MPI_Barrier(MPI_COMM_WORLD);

	if (rank==0) {
		cout<<"Gateway Service : "<<rank<<" Running on "<<hostname<<endl;
  }	else {
		cout<<"Cell            : "<<rank<<" Running on "<<hostname<<endl;
  }
	MPI_Barrier(MPI_COMM_WORLD);

  // Initialization phase
	// Communicate Cell and Resource parameters and create all components
  double endTime=0.0, upInterval=0;
	if (rank==0)
	{
		gates=new gs[1];
		gates[0]=gs("../input/CellData", "../input/AppData", "../input/BrokerData");
		gates[0].printfile("systeminfo",ios::out);
    commSimParameters(gates[0].gsi()[0],rank,numtasks,MPI_COMM_WORLD);

		endTime=gates[0].gsi()[0].maxTime;
		upInterval=gates[0].gsi()[0].upInterval;
	}
	else
	{
		struct siminputs *si;
		si=new siminputs[1];

    // Receive program values from the master
		commSimParameters(*si,rank,numtasks,MPI_COMM_WORLD);

		clCell=new cell[1];

    // Initialize cells based on user-defined configuration
		*clCell=cell(*(si->cinp));

		// Initialization and Update outside of the constructor to avoid changing the values of the pointers to components (can be avoided with move copy constructor in (>=C++11))
		// Creation of the SOSM hierarchical topology
    clCell[0].gbrok()->initbroker(clCell->gnumOfTypes(),clCell->gtypes(),clCell->gnumOfResourcesPerType(),clCell->gresources(),clCell->gpowerComp(),clCell->gnetwork(),si->cinp->binp[0]);
		// State information update and Assessment Functions and SI calculation
    clCell[0].gbrok()->updateStateInfo(clCell[0].gnetwork(),0.0);
		endTime=si->maxTime;
		upInterval=si->upInterval;

	}
	MPI_Barrier(MPI_COMM_WORLD);

  // Receive statistics from the cells
	if (rank==0) {
		commStats(gates,clCell,rank,numtasks,MPI_COMM_WORLD);
		gates[0].printStats("../output/output",ios::out);
	}	else {
		commStats(gates,clCell,rank,numtasks,MPI_COMM_WORLD);
	}
	MPI_Barrier(MPI_COMM_WORLD);

  int ss = 0, allTasks = 0;
  int *commCells=nullptr;

  // Simulation phase
	for(double t=0.0;t<endTime;t+=1.0) {

		if (rank==0) {
      // Create one or more tasks based on AppData configuration
			taskCreationEngine(jobs,gates[0].gai()[0]);
			allTasks+=jobs.size();
		//	taskImplSelect(jobs);
			// For each task, retrieve the list of candidate cells and select the most appropriate
      taskCellSelect(jobs,gates,&commCells);
      // Send the task to the selected cell
			commTaskParameters(jobs,rank,numtasks,commCells,MPI_COMM_WORLD);
      // Empty the task list
			jobs.clear();
		}
		else {
      // Receive task
			commTaskParameters(jobs,rank,numtasks,commCells,MPI_COMM_WORLD);
      // Begin deployment of task:
      // Traverse the components tree to locate most sutiable vRM
      clCell[0].deploy(&jobs);
			ss+=(int)jobs.size();
			jobs.clear();

      // Simulation phase:
      // Actually simulate the process of the task by the vRM
			clCell[0].timestep(t);
		}
		MPI_Barrier(MPI_COMM_WORLD);

    // On every defined interval
		if(((int)t+1)%((int)upInterval)==0)
		{
			if(rank==0)
			{
        // Receive statistics from the cells and print to files
				commStats(gates,clCell,rank,numtasks,MPI_COMM_WORLD);
				gates[0].printStats("../output/output",ios::out | ios::app);
				cout<<std::fixed<<setprecision(2)<<"\r Simulation at: "<<100.0*(t+1)/(endTime)<<" %"<<flush;
			}
			else
			{
				commStats(gates,clCell,rank,numtasks,MPI_COMM_WORLD);
			}
		}
	}

	// Print output to json file
  if(rank==0) {
		gates[0].printStatsJson("../output/output",ios::out | ios::app, endTime, upInterval);

		commStats(gates,clCell,rank,numtasks,MPI_COMM_WORLD);

  	gates[0].printStats("output",ios::out | ios::app);
		cout << endl << "Elapsed time: "<<MPI_Wtime() - startTime<<" sec"<<endl;
		cout<<"Total number of submitted tasks: "<<allTasks<<endl;
		delete[] gates;
	}
	else
	{
		//cout<<endl<<ss<<endl;
		commStats(gates,clCell,rank,numtasks,MPI_COMM_WORLD);
		delete[] clCell;
	}
	MPI_Finalize();
	return 0;
}


