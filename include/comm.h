#ifndef COMM_H
#define COMM_H
#include <cell.h>
#include <comm.h>
#include <gs.h>
#include <inputs.h>
#include <mpi.h>
#include <stat.h>
#include <task.h>
#include <list>
#include <string>
using namespace std;

/// Sends configuration parameters from the gateway to the cells
void commSimParameters(struct siminputs& si, const int& rank, const int& numtasks, const MPI_Comm& Comm);

/// Retrieves statistics from the cells
void commStats(const gs* gates, const cell* clCell, const int& rank, const int& numtasks, const MPI_Comm& Comm);

/// Sends tasks to the appropriate cells for further processing
void commTaskParameters(list<task>& jobs, const int& rank, const int& numtasks, const int* commCell,
                        const MPI_Comm& Comm);

#endif
