/* Copyright 2017 The CloudLightning Simulation Framework Authors. All
Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include <cell.h>         // for cell
#include <communicator.h> // for communicator
#include <gs.h>           // for gs
#include <inputs.h>       // for siminputs, brinputs, cellinputs
#include <sosmBroker.h>   // for sosmBroker
#include <mlBroker.h>     // for mlBroker
#include <task.h>
#include <tce.h>
#include <traditionalBroker.h> // for traditionalBroker
#include <iomanip>
#include <iostream>
#include <httpClient.h>

using std::cout;
using std::endl;
using std::flush;
using std::max;
using std::setprecision;

int main(int argc, char **argv)
{
  // Initialize MPI
  int rc = MPI_Init(&argc, &argv);
  if (rc != MPI_SUCCESS)
  {
    cout << "Error starting MPI program. Terminating..." << endl;
    MPI_Abort(MPI_COMM_WORLD, rc);
  }

  // Get ranks and cluster size
  int clusterSize = 0, rank = 0, len = 0;
  char hostname[MPI_MAX_PROCESSOR_NAME];
  MPI_Comm mpiComm;

  MPI_Comm_size(MPI_COMM_WORLD, &clusterSize);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Get_processor_name(hostname, &len);
  MPI_Comm_dup(MPI_COMM_WORLD, &mpiComm);

  unsigned int omp_thr = max(atoi(std::getenv("OMP_NUM_THREADS")), 1);

  // Print cluster info
  double startTime = 0.0;
  if (rank == 0)
  {
    startTime = MPI_Wtime();
    cout << endl
         << "INITIALIZATION PHASE" << endl;
    cout << "------------------------------- " << endl;
    cout << "Cluster Size        : " << clusterSize << endl;
    cout << "Threads per machine : " << omp_thr << endl;
    cout << "------------------------------- " << endl;
  }

  MPI_Barrier(MPI_COMM_WORLD);

  // Initialization phase
  // Communicate Cell and Resource parameters and create all components
  int decisionMaking = 0;
  double endTime = 0.0, updateInterval = 0;

  // Pointer to siminputs
  gs *gates = nullptr;

  // Cell
  cell *clCell = nullptr;

  communicator comm;

  if (rank == 0)
  {
    cout << "Gateway Service : " << rank << " Running on " << hostname << endl;

    gates = new gs[1];
    gates[0] = gs("input/CellData.json", "input/AppData.json", "input/BrokerData.json");
    gates[0].printfile("output/systeminfo", ios::out);

    comm.simulationParameters(gates[0].gsi()[0], rank, clusterSize, MPI_COMM_WORLD);

    endTime = gates[0].gsi()[0].maxTime;
    updateInterval = gates[0].gsi()[0].updateInterval;
    decisionMaking = gates[0].gsi()[0].decisionMaking;
  }
  else
  {
    cout << "Cell            : " << rank << " Running on " << hostname << endl;

    siminputs *si = new siminputs[1];

    // Receive program values from the master
    comm.simulationParameters(*si, rank, clusterSize, MPI_COMM_WORLD);

    decisionMaking = si->decisionMaking;
    clCell = new cell[1];

    // Initialize cells based on user-defined configuration
    *clCell = cell(*(si->cinp), decisionMaking);

    // Initialization and update outside of the constructor to avoid changing the values of the pointers to components
    // (can be avoided with move copy constructor in (>=C++11))

    clCell[0].getBroker()->init(clCell, si);
    clCell[0].getBroker()->updateStateInfo(clCell, 0.0);

    endTime = si->maxTime;
    updateInterval = si->updateInterval;
  }
  MPI_Barrier(MPI_COMM_WORLD);

  // Send statistics from the cells to the gateway
  comm.cellStatistics(gates, clCell, rank, clusterSize, MPI_COMM_WORLD);

  if (rank == 0)
  {
    // print stats to txt files
    // gates[0].printStats("output/output", ios::out);

    cout << "Resource allocation mechanism: ";
    if (decisionMaking == 0)
    {
      cout << "Traditional" << endl;
    }
    else if (decisionMaking == 1)
    {
      cout << "SOSM" << endl;
    }
    else if (decisionMaking == 2)
    {
      cout << "Improved SOSM" << endl;
    }
    else if (decisionMaking == 3)
    {
      cout << "ML-Based with Traditional Fallback" << endl;

      // test http client
      std::cout << "Checking connection to external service..." << std::endl;
      if (!checkHealth())
      {
        std::cerr << "Failed to connect to external service" << std::endl;
        return 1;
      }
      std::cout << "Connection successful!!!" << std::endl;
    }
  }

  MPI_Barrier(MPI_COMM_WORLD);

  int allTasks = 0;
  int *commCells = nullptr;
  // Current job list
  list<task> jobs;

  // For every time step
  for (double time = 0.0; time < endTime; time += 1.0)
  {

    if (time == 0 && rank == 0)
    {
      gates[0].printStatsToJsonDirect("output/output", ios::out, endTime, updateInterval, decisionMaking, allTasks, time);
    }

    if (rank == 0)
    {
      // Create one or more tasks based on AppData configuration
      taskCreationEngine(jobs, gates[0].gai()[0]);
      allTasks += jobs.size();

      if (decisionMaking == 0)
      {
        taskImplSelect(jobs);
      }
      // For each task, retrieve the list of candidate cells and select the most appropriate
      taskCellSelect(jobs, gates, &commCells);
    }

    // Send the task from the gateway to the selected cell
    comm.taskParameters(jobs, rank, clusterSize, commCells, MPI_COMM_WORLD);

    if (rank != 0)
    {
      // Task deployment: Traverse the components tree to locate the most suitable vRM
      clCell[0].deploy(jobs);

      // Calls broker::timestep to perform the simulation phase, update the state information and update cell statistics
      if (clCell->galloc())
      {
        clCell[0].getBroker()->timestep(clCell);
        clCell[0].getBroker()->updateStateInfo(clCell, time);
        clCell[0].updateStats(time);
      }
    }
    // Empty the jobs list
    jobs.clear();

    MPI_Barrier(MPI_COMM_WORLD);

    // On every defined interval
    if (((int)time + 1) % ((int)updateInterval) == 0)
    {
      // Receive statistics from the cells
      comm.cellStatistics(gates, clCell, rank, clusterSize, MPI_COMM_WORLD);

      if (rank == 0)
      {
        // print stats to txt files (older versions)
        // gates[0].printStats("output/output", ios::out | ios::app);
        cout << std::fixed << setprecision(2) << "\r Simulation at: " << 100.0 * (time + 1) / (endTime) << " %"
             << flush;
        // print stats to json file
        gates[0].printStatsToJsonDirect("output/output", ios::out, endTime, updateInterval, decisionMaking, allTasks, time);
      }
    }
  }
  comm.cellStatistics(gates, clCell, rank, clusterSize, MPI_COMM_WORLD);

  if (rank == 0)
  {
    if (decisionMaking == 3)
    {
      // save model logs to file
      postJSON("/save_logs", "");
    }
    // Convert output file to json after reading the txt outputs (older versions)
    // gates[0].printStatsJson("output/output", ios::out, endTime, updateInterval, decisionMaking, allTasks);
    cout << endl
         << "Elapsed time: " << MPI_Wtime() - startTime << " sec" << endl;
    cout << "Total number of submitted tasks: " << allTasks << endl;
    delete[] gates;
  }
  else
  {
    delete[] clCell;
  }
  MPI_Finalize();
  return 0;
}
