#ifndef COMM_H
#define COMM_H
#include <inputs.h>
#include <list>
#include <comm.h>
#include <string>
#include <mpi.h>
#include <task.h>
#include <stat.h>
#include <gs.h>
#include <cell.h>
using namespace std;

void commSimParameters(struct siminputs & si, const int &rank, const int &numtasks, const MPI_Comm &Comm);
void commStats(const gs *gates, const cell *clCell, const int &rank, const int &numtasks, const MPI_Comm &Comm);
void commTaskParameters(list<task> &jobs, const int &rank, const int &numtasks, const int *commCell, const MPI_Comm &Comm);

#endif
