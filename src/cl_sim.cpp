#include <cell.h>
#include <comm.h>
#include <gs.h>
#include <inputs.h>
#include <mpi.h>
#include <resource.h>
#include <task.h>
#include <task.h>
#include <tce.h>
#include <cstdio>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char** argv)
{
  // Pointer to siminputs
  gs* gates = nullptr;

  // Cell
  cell* clCell = nullptr;

  // Current Job List
  list<task> jobs;

  // Initialize MPI
  int rc = MPI_Init(&argc, &argv);
  if (rc != MPI_SUCCESS) {
    cout << "Error starting MPI program. Terminating..." << endl;
    MPI_Abort(MPI_COMM_WORLD, rc);
  }

  // Get ranks and cluster size
  int numberOfTasks = 0, rank = 0, len = 0;
  char hostname[MPI_MAX_PROCESSOR_NAME];
  MPI_Comm Comm;

  MPI_Comm_size(MPI_COMM_WORLD, &numberOfTasks);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Get_processor_name(hostname, &len);
  MPI_Comm_dup(MPI_COMM_WORLD, &Comm);

  unsigned int omp_thr = std::max(atoi(std::getenv("OMP_NUM_THREADS")), 1);

  // Print cluster info
  double startTime = 0.0;
  if (rank == 0) {
    startTime = MPI_Wtime();
    cout << endl << "INITIALIZATION PHASE" << endl;
    cout << "------------------------------- " << endl;
    cout << "Cluster Size        : " << numberOfTasks << endl;
    cout << "Threads per machine : " << omp_thr << endl;
    cout << "------------------------------- " << endl;
  }
  MPI_Barrier(MPI_COMM_WORLD);

  if (rank == 0) {
    cout << "Gateway Service : " << rank << " Running on " << hostname << endl;
  } else {
    cout << "Cell            : " << rank << " Running on " << hostname << endl;
  }
  MPI_Barrier(MPI_COMM_WORLD);

  // Initialization phase
  // Communicate Cell and Resource parameters and create all components
  int sosmIntegration = 0;
  double endTime = 0.0, updateInterval = 0;

  if (rank == 0) {
    gates = new gs[1];
    gates[0] = gs("../input/CellData.json", "../input/AppData.json", "../input/BrokerData.json");
    gates[0].printfile("systeminfo", ios::out);

    commSimParameters(gates[0].gsi()[0], rank, numberOfTasks, MPI_COMM_WORLD);

    endTime = gates[0].gsi()[0].maxTime;
    updateInterval = gates[0].gsi()[0].updateInterval;
    sosmIntegration=gates[0].gsi()[0].sosmIntegration;
  } else {
    struct siminputs* si;
    si = new siminputs[1];

    // Receive program values from the master
    commSimParameters(*si, rank, numberOfTasks, MPI_COMM_WORLD);

    sosmIntegration=si->sosmIntegration;
    clCell = new cell[1];

    // Initialize cells based on user-defined configuration
    *clCell = cell(*(si->cinp),sosmIntegration);

    // Initialization and update outside of the constructor to avoid changing the values of the pointers to components
    // (can be avoided with move copy constructor in (>=C++11))
    // Creation of the SOSM hierarchical topology
    if (sosmIntegration){
      clCell[0].getBroker()->initBroker(clCell->getNumberOfTypes(), clCell->getTypes(), clCell->getNumberOfResourcesPerType(),
                                  clCell->getResources(), clCell->getPowerConsumption(), clCell->getNetwork(), si->cinp->binp[0]);
      // State information update and Assessment Functions and SI calculation
      clCell[0].getBroker()->updateStateInfo(clCell[0].getNetwork(), 0.0);
    }
    endTime = si->maxTime;
    updateInterval = si->updateInterval;
  }
  MPI_Barrier(MPI_COMM_WORLD);

  // Receive statistics from the cells
  if (rank == 0) {
    commStats(gates, clCell, rank, numberOfTasks, MPI_COMM_WORLD);
    gates[0].printStats("../output/output", ios::out);
  } else {
    commStats(gates, clCell, rank, numberOfTasks, MPI_COMM_WORLD);
  }
  MPI_Barrier(MPI_COMM_WORLD);

  if (rank==0){
    if (sosmIntegration){
      cout<<"Resource Allocation Mechanism: SOSM"<<endl;
    }
    else{
      cout<<"Resource Allocation Mechanism: Traditional"<<endl;
    }
  }

  int ss = 0, allTasks = 0;
  int* commCells = nullptr;

  // For every time step
  for (double time = 0.0; time < endTime; time += 1.0) {
    if (rank == 0) {
      // Create one or more tasks based on AppData configuration
      taskCreationEngine(jobs, gates[0].gai()[0]);
      allTasks += jobs.size();
      if(sosmIntegration==0){
      	taskImplSelect(jobs);
      }	
      // For each task, retrieve the list of candidate cells and select the most appropriate
      taskCellSelect(jobs, gates, &commCells);

      // Send the task to the selected cell
      commTaskParameters(jobs, rank, numberOfTasks, commCells, MPI_COMM_WORLD);

      // Empty the task list
      jobs.clear();
    } else {
      // Receive task
      commTaskParameters(jobs, rank, numberOfTasks, commCells, MPI_COMM_WORLD);
      // Begin deployment of the task:
      // Traverse the components tree to locate the most suitable vRM
      clCell[0].deploy(&jobs);
      ss += (int)jobs.size();
      jobs.clear();

      // Simulation phase:
      // Actually simulate the process of the task by the vRM
      clCell[0].timestep(time);
    }
    MPI_Barrier(MPI_COMM_WORLD);

    // On every defined interval
    if (((int)time + 1) % ((int)updateInterval) == 0) {
      if (rank == 0) {
        // Receive statistics from the cells and print to files
        commStats(gates, clCell, rank, numberOfTasks, MPI_COMM_WORLD);
        gates[0].printStats("../output/output", ios::out | ios::app);
        cout << std::fixed << setprecision(2) << "\r Simulation at: " << 100.0 * (time + 1) / (endTime) << " %" << flush;
      } else {
        commStats(gates, clCell, rank, numberOfTasks, MPI_COMM_WORLD);
      }
    }
  }

  if (rank == 0) {
    // Print output to json file
    //remove("../output/outputCLsim.json");
    gates[0].printStatsJson("../output/output", ios::out, endTime, updateInterval);

    commStats(gates, clCell, rank, numberOfTasks, MPI_COMM_WORLD);

    gates[0].printStats("output", ios::out | ios::app);
    cout << endl << "Elapsed time: " << MPI_Wtime() - startTime << " sec" << endl;
    cout << "Total number of submitted tasks: " << allTasks << endl;
    delete[] gates;
  } else {
    // cout<<endl<<ss<<endl;
    commStats(gates, clCell, rank, numberOfTasks, MPI_COMM_WORLD);
    delete[] clCell;
  }
  MPI_Finalize();
  return 0;
}
