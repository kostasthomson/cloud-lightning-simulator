#include <cell.h>
#include <comm.h>
#include <gs.h>
#include <inputs.h>
#include <mpi.h>
#include <string.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

void commSimParameters(struct siminputs& si, const int& rank, const int& numtasks, const MPI_Comm& Comm)
{
  int i, j;
  int one = 1;
  MPI_Status status;

  if (rank == 0) {
    for (i = 0; i < numtasks - 1; i++) {
      // Send simulator-specific info
      MPI_Send(&si.maxTime, 1, MPI_DOUBLE, i + 1, i + 1, Comm);
      MPI_Send(&si.upInterval, 1, MPI_DOUBLE, i + 1, i + 1, Comm);
      MPI_Send(&one, 1, MPI_INT, i + 1, i + 1, Comm);

      // Send cell-specific info
      MPI_Send(&si.cinp[i].ID, 1, MPI_INT, i + 1, i + 1, Comm);
      MPI_Send(&si.cinp[i].numOfTypes, 1, MPI_INT, i + 1, i + 1, Comm);
      MPI_Send(si.cinp[i].numOfResourcesPerType, si.cinp[i].numOfTypes, MPI_INT, i + 1, i + 1, Comm);
      MPI_Send(si.cinp[i].types, si.cinp[i].numOfTypes, MPI_INT, i + 1, i + 1, Comm);

      MPI_Send(&si.cinp[i].binp[0].numOfFuncs, 1, MPI_INT, i + 1, i + 1, Comm);
      MPI_Send(si.cinp[i].binp[0].Ws, si.cinp[i].binp[0].numOfFuncs, MPI_DOUBLE, i + 1, i + 1, Comm);
      MPI_Send(&si.cinp[i].binp[0].initResPervRM, 1, MPI_INT, i + 1, i + 1, Comm);
      MPI_Send(&si.cinp[i].binp[0].initvRMPerpSwitch, 1, MPI_INT, i + 1, i + 1, Comm);
      MPI_Send(&si.cinp[i].binp[0].initpSwitchPerpRouter, 1, MPI_INT, i + 1, i + 1, Comm);
      MPI_Send(&si.cinp[i].binp[0].pollIntervalCellM, 1, MPI_DOUBLE, i + 1, i + 1, Comm);
      MPI_Send(&si.cinp[i].binp[0].pollIntervalpRouter, 1, MPI_DOUBLE, i + 1, i + 1, Comm);
      MPI_Send(&si.cinp[i].binp[0].pollIntervalpSwitch, 1, MPI_DOUBLE, i + 1, i + 1, Comm);
      MPI_Send(&si.cinp[i].binp[0].pollIntervalvRM, 1, MPI_DOUBLE, i + 1, i + 1, Comm);
      MPI_Send(&si.cinp[i].binp[0].vRMdeploystrategy, 1, MPI_INT, i + 1, i + 1, Comm);

      MPI_Send(&si.cinp[i].ninp[0].netBW, 1, MPI_DOUBLE, i + 1, i + 1, Comm);
      MPI_Send(&si.cinp[i].ninp[0].overCommitNet, 1, MPI_DOUBLE, i + 1, i + 1, Comm);

      // Send resource-specific info
      for (j = 0; j < si.cinp[i].numOfTypes; j++) {
        MPI_Send(&si.cinp[i].rinp[j].type, 1, MPI_INT, i + 1, i + 1, Comm);
        MPI_Send(&si.cinp[i].rinp[j].numOfProcUnits, 1, MPI_DOUBLE, i + 1, i + 1, Comm);
        MPI_Send(&si.cinp[i].rinp[j].totMem, 1, MPI_DOUBLE, i + 1, i + 1, Comm);
        MPI_Send(&si.cinp[i].rinp[j].totSto, 1, MPI_DOUBLE, i + 1, i + 1, Comm);
        MPI_Send(&si.cinp[i].rinp[j].overCommitProc, 1, MPI_DOUBLE, i + 1, i + 1, Comm);
        MPI_Send(&si.cinp[i].rinp[j].overCommitMem, 1, MPI_DOUBLE, i + 1, i + 1, Comm);
        MPI_Send(&si.cinp[i].rinp[j].compCap, 1, MPI_DOUBLE, i + 1, i + 1, Comm);
        MPI_Send(&si.cinp[i].rinp[j].accelerator, 1, MPI_INT, i + 1, i + 1, Comm);
        MPI_Send(&si.cinp[i].rinp[j].totAcc, 1, MPI_INT, i + 1, i + 1, Comm);
        MPI_Send(&si.cinp[i].rinp[j].accCompCap, 1, MPI_DOUBLE, i + 1, i + 1, Comm);

        MPI_Send(&si.cinp[i].pinp[j].typeCpu, 1, MPI_INT, i + 1, i + 1, Comm);
        MPI_Send(&si.cinp[i].pinp[j].cpuPmin, 1, MPI_DOUBLE, i + 1, i + 1, Comm);
        MPI_Send(&si.cinp[i].pinp[j].cpuPmax, 1, MPI_DOUBLE, i + 1, i + 1, Comm);
        MPI_Send(&si.cinp[i].pinp[j].numOfPoints, 1, MPI_INT, i + 1, i + 1, Comm);
        if (si.cinp[i].pinp[j].numOfPoints > 0) {
          MPI_Send(si.cinp[i].pinp[j].cpubins, si.cinp[i].pinp[j].numOfPoints, MPI_DOUBLE, i + 1, i + 1, Comm);

          MPI_Send(si.cinp[i].pinp[j].cpuP, si.cinp[i].pinp[j].numOfPoints, MPI_DOUBLE, i + 1, i + 1, Comm);
        }
        MPI_Send(&si.cinp[i].pinp[j].cpuC, 1, MPI_DOUBLE, i + 1, i + 1, Comm);
        MPI_Send(&si.cinp[i].pinp[j].typeAcc, 1, MPI_INT, i + 1, i + 1, Comm);
        MPI_Send(&si.cinp[i].pinp[j].accPmin, 1, MPI_DOUBLE, i + 1, i + 1, Comm);
        MPI_Send(&si.cinp[i].pinp[j].accPmax, 1, MPI_DOUBLE, i + 1, i + 1, Comm);
        MPI_Send(&si.cinp[i].pinp[j].accC, 1, MPI_DOUBLE, i + 1, i + 1, Comm);
      }
    }
  } else {
    // Receive Simulator specific info
    si.alloc = 1;
    MPI_Recv(&si.maxTime, 1, MPI_DOUBLE, 0, rank, Comm, &status);
    MPI_Recv(&si.upInterval, 1, MPI_DOUBLE, 0, rank, Comm, &status);
    MPI_Recv(&si.numOfCells, 1, MPI_INT, 0, rank, Comm, &status);

    // Receive Cell specific info
    si.cinp = new cellinputs[1];
    si.cinp->alloc = 1;
    MPI_Recv(&si.cinp->ID, 1, MPI_INT, 0, rank, Comm, &status);
    MPI_Recv(&si.cinp->numOfTypes, 1, MPI_INT, 0, rank, Comm, &status);
    si.cinp->numOfResourcesPerType = new int[si.cinp->numOfTypes];
    si.cinp->types = new int[si.cinp->numOfTypes];
    MPI_Recv(si.cinp->numOfResourcesPerType, si.cinp->numOfTypes, MPI_INT, 0, rank, Comm, &status);
    MPI_Recv(si.cinp->types, si.cinp->numOfTypes, MPI_INT, 0, rank, Comm, &status);

    // Receive Broker info
    si.cinp->binp = new brinputs[1];
    si.cinp->binp[0].alloc = 1;
    MPI_Recv(&si.cinp->binp[0].numOfFuncs, 1, MPI_INT, 0, rank, Comm, &status);
    si.cinp->binp[0].Ws = new double[si.cinp->binp[0].numOfFuncs];
    MPI_Recv(si.cinp->binp[0].Ws, si.cinp->binp[0].numOfFuncs, MPI_DOUBLE, 0, rank, Comm, &status);
    MPI_Recv(&si.cinp->binp[0].initResPervRM, 1, MPI_INT, 0, rank, Comm, &status);
    MPI_Recv(&si.cinp->binp[0].initvRMPerpSwitch, 1, MPI_INT, 0, rank, Comm, &status);
    MPI_Recv(&si.cinp->binp[0].initpSwitchPerpRouter, 1, MPI_INT, 0, rank, Comm, &status);
    MPI_Recv(&si.cinp->binp[0].pollIntervalCellM, 1, MPI_DOUBLE, 0, rank, Comm, &status);
    MPI_Recv(&si.cinp->binp[0].pollIntervalpRouter, 1, MPI_DOUBLE, 0, rank, Comm, &status);
    MPI_Recv(&si.cinp->binp[0].pollIntervalpSwitch, 1, MPI_DOUBLE, 0, rank, Comm, &status);
    MPI_Recv(&si.cinp->binp[0].pollIntervalvRM, 1, MPI_DOUBLE, 0, rank, Comm, &status);
    MPI_Recv(&si.cinp->binp[0].vRMdeploystrategy, 1, MPI_INT, 0, rank, Comm, &status);

    // Receive Interconnection info
    si.cinp->ninp = new netinputs[1];
    si.cinp->ninp[0].alloc = 1;
    MPI_Recv(&si.cinp->ninp[0].netBW, 1, MPI_DOUBLE, 0, rank, Comm, &status);
    MPI_Recv(&si.cinp->ninp[0].overCommitNet, 1, MPI_DOUBLE, 0, rank, Comm, &status);

    // Receive Resource specific info
    si.cinp->rinp = new resinputs[si.cinp->numOfTypes];
    si.cinp->pinp = new powinputs[si.cinp->numOfTypes];
    for (j = 0; j < si.cinp->numOfTypes; j++) {
      si.cinp->rinp[j].alloc = 1;
      MPI_Recv(&si.cinp->rinp[j].type, 1, MPI_INT, 0, rank, Comm, &status);
      MPI_Recv(&si.cinp->rinp[j].numOfProcUnits, 1, MPI_DOUBLE, 0, rank, Comm, &status);
      MPI_Recv(&si.cinp->rinp[j].totMem, 1, MPI_DOUBLE, 0, rank, Comm, &status);
      MPI_Recv(&si.cinp->rinp[j].totSto, 1, MPI_DOUBLE, 0, rank, Comm, &status);
      MPI_Recv(&si.cinp->rinp[j].overCommitProc, 1, MPI_DOUBLE, 0, rank, Comm, &status);
      MPI_Recv(&si.cinp->rinp[j].overCommitMem, 1, MPI_DOUBLE, 0, rank, Comm, &status);
      MPI_Recv(&si.cinp->rinp[j].compCap, 1, MPI_DOUBLE, 0, rank, Comm, &status);
      MPI_Recv(&si.cinp->rinp[j].accelerator, 1, MPI_INT, 0, rank, Comm, &status);
      MPI_Recv(&si.cinp->rinp[j].totAcc, 1, MPI_INT, 0, rank, Comm, &status);
      MPI_Recv(&si.cinp->rinp[j].accCompCap, 1, MPI_DOUBLE, 0, rank, Comm, &status);

      si.cinp->pinp[j].alloc = 1;
      si.cinp->pinp[j].accelerator = si.cinp->rinp[j].accelerator;
      MPI_Recv(&si.cinp->pinp[j].typeCpu, 1, MPI_INT, 0, rank, Comm, &status);
      MPI_Recv(&si.cinp->pinp[j].cpuPmin, 1, MPI_DOUBLE, 0, rank, Comm, &status);
      MPI_Recv(&si.cinp->pinp[j].cpuPmax, 1, MPI_DOUBLE, 0, rank, Comm, &status);
      MPI_Recv(&si.cinp->pinp[j].numOfPoints, 1, MPI_INT, 0, rank, Comm, &status);
      if (si.cinp->pinp[j].numOfPoints > 0) {
        si.cinp->pinp[j].cpubins = new double[si.cinp->pinp[j].numOfPoints];
        MPI_Recv(si.cinp->pinp[j].cpubins, si.cinp->pinp[j].numOfPoints, MPI_DOUBLE, 0, rank, Comm, &status);
        si.cinp->pinp[j].cpuP = new double[si.cinp->pinp[j].numOfPoints];
        MPI_Recv(si.cinp->pinp[j].cpuP, si.cinp->pinp[j].numOfPoints, MPI_DOUBLE, 0, rank, Comm, &status);
      }
      MPI_Recv(&si.cinp->pinp[j].cpuC, 1, MPI_DOUBLE, 0, rank, Comm, &status);
      MPI_Recv(&si.cinp->pinp[j].typeAcc, 1, MPI_INT, 0, rank, Comm, &status);
      MPI_Recv(&si.cinp->pinp[j].accPmin, 1, MPI_DOUBLE, 0, rank, Comm, &status);
      MPI_Recv(&si.cinp->pinp[j].accPmax, 1, MPI_DOUBLE, 0, rank, Comm, &status);
      MPI_Recv(&si.cinp->pinp[j].accC, 1, MPI_DOUBLE, 0, rank, Comm, &status);
    }
  }
}

void commTaskParameters(list<task>& jobs, const int& rank, const int& numtasks, const int* commCell,
                        const MPI_Comm& Comm)
{
  int i, j, k, who;
  int L_type, L_numOfAvailImpl;
  int* L_availImpl;
  double L_reqIns;
  int L_numOfVMs;
  double* L_reqPMNS;
  int* L_typeactPMN;
  double** L_minmaxactPMN;
  int* L_avAcc;
  double* L_rhoAcc;
  MPI_Status status;
  int establish;
  if (rank == 0) {
    establish = jobs.size();
    for (i = 0; i < numtasks - 1; i++)
      MPI_Send(&establish, 1, MPI_INT, i + 1, i + 1, Comm);
  } else {
    MPI_Recv(&establish, 1, MPI_INT, 0, rank, Comm, &status);
  }
  if (!establish)
    return;
  if (rank == 0) {
    list<task>::iterator it = jobs.begin();
    for (i = 0; i < establish; i++) {
      who = commCell[i];
      for (j = 0; j < numtasks - 1; j++)
        MPI_Send(&who, 1, MPI_INT, j + 1, j + 1, Comm);
      if (who > 0) {
        L_type = (*it).gtype();
        MPI_Send(&L_type, 1, MPI_INT, who, who, Comm);

        L_numOfAvailImpl = (*it).gnumOfAvailImpl();
        MPI_Send(&L_numOfAvailImpl, 1, MPI_INT, who, who, Comm);

        L_availImpl = new int[L_numOfAvailImpl];
        for (j = 0; j < L_numOfAvailImpl; j++)
          L_availImpl[j] = (*it).gavailImpl()[j];
        MPI_Send(L_availImpl, L_numOfAvailImpl, MPI_INT, who, who, Comm);

        L_reqIns = (*it).greqIns();
        MPI_Send(&L_reqIns, 1, MPI_DOUBLE, who, who, Comm);

        L_numOfVMs = (*it).gnumOfVMs();
        MPI_Send(&L_numOfVMs, 1, MPI_INT, who, who, Comm);

        L_reqPMNS = new double[4];
        for (j = 0; j < 4; j++)
          L_reqPMNS[j] = (*it).greqPMNS()[j];
        MPI_Send(L_reqPMNS, 4, MPI_DOUBLE, who, who, Comm);

        L_typeactPMN = new int[3];
        for (j = 0; j < 3; j++)
          L_typeactPMN[j] = (*it).gtypeactPMN()[j];
        MPI_Send(L_typeactPMN, 3, MPI_INT, who, who, Comm);

        L_minmaxactPMN = new double*[3];
        for (j = 0; j < 3; j++)
          L_minmaxactPMN[j] = new double[2];
        for (j = 0; j < 3; j++)
          for (k = 0; k < 2; k++)
            L_minmaxactPMN[j][k] = (*it).gminmaxactPMN()[j][k];
        for (j = 0; j < 3; j++)
          MPI_Send(&L_minmaxactPMN[j][0], 2, MPI_DOUBLE, who, who, Comm);

        L_avAcc = new int[L_numOfAvailImpl];
        for (j = 0; j < L_numOfAvailImpl; j++)
          L_avAcc[j] = (*it).gavAcc()[j];
        MPI_Send(L_avAcc, L_numOfAvailImpl, MPI_INT, who, who, Comm);

        L_rhoAcc = new double[L_numOfAvailImpl];
        for (j = 0; j < L_numOfAvailImpl; j++)
          L_rhoAcc[j] = (*it).grhoAcc()[j];
        MPI_Send(L_rhoAcc, L_numOfAvailImpl, MPI_DOUBLE, who, who, Comm);

        delete[] L_availImpl;
        delete[] L_reqPMNS;
        delete[] L_typeactPMN;
        for (j = 0; j < 3; j++)
          delete[] L_minmaxactPMN[j];
        delete[] L_minmaxactPMN;
        delete[] L_avAcc;
        delete[] L_rhoAcc;
      }
      it++;
    }
  } else {
    for (i = 0; i < establish; i++) {
      MPI_Recv(&who, 1, MPI_INT, 0, rank, Comm, &status);
      if (who == rank) {
        MPI_Recv(&L_type, 1, MPI_INT, 0, rank, Comm, &status);
        MPI_Recv(&L_numOfAvailImpl, 1, MPI_INT, 0, rank, Comm, &status);
        L_availImpl = new int[L_numOfAvailImpl];
        MPI_Recv(L_availImpl, L_numOfAvailImpl, MPI_INT, 0, rank, Comm, &status);
        MPI_Recv(&L_reqIns, 1, MPI_DOUBLE, 0, rank, Comm, &status);
        MPI_Recv(&L_numOfVMs, 1, MPI_INT, 0, rank, Comm, &status);
        L_reqPMNS = new double[4];
        MPI_Recv(L_reqPMNS, 4, MPI_DOUBLE, 0, rank, Comm, &status);
        L_typeactPMN = new int[3];
        MPI_Recv(L_typeactPMN, 3, MPI_INT, 0, rank, Comm, &status);
        L_minmaxactPMN = new double*[3];

        for (j = 0; j < 3; j++)
          L_minmaxactPMN[j] = new double[2];
        for (j = 0; j < 3; j++)
          MPI_Recv(&L_minmaxactPMN[j][0], 2, MPI_DOUBLE, 0, rank, Comm, &status);
        L_avAcc = new int[L_numOfAvailImpl];
        MPI_Recv(L_avAcc, L_numOfAvailImpl, MPI_INT, 0, rank, Comm, &status);
        L_rhoAcc = new double[L_numOfAvailImpl];
        MPI_Recv(L_rhoAcc, L_numOfAvailImpl, MPI_DOUBLE, 0, rank, Comm, &status);

        jobs.push_back(task(L_type, L_numOfAvailImpl, L_availImpl, L_reqIns, L_numOfVMs, L_reqPMNS[0], L_reqPMNS[1],
                            L_reqPMNS[2], L_reqPMNS[3], L_typeactPMN[0], L_typeactPMN[1], L_typeactPMN[2],
                            &L_minmaxactPMN[0][0], &L_minmaxactPMN[1][0], &L_minmaxactPMN[2][0], L_avAcc, L_rhoAcc));

        delete[] L_availImpl;
        delete[] L_reqPMNS;
        delete[] L_typeactPMN;
        for (j = 0; j < 3; j++) {
          delete[] L_minmaxactPMN[j];
        }
        delete[] L_minmaxactPMN;
        delete[] L_avAcc;
        delete[] L_rhoAcc;
      }
    }
  }
}

void commStats(const gs* gates, const cell* clCell, const int& rank, const int& numtasks, const MPI_Comm& Comm)
{
  int i, j;
  MPI_Status status;
  if (rank == 0) {
    for (i = 0; i < numtasks - 1; i++) {
      for (j = 0; j < gates[0].gsi()[0].cinp[i].numOfTypes; j++) {
        MPI_Recv(&gates[0].gstats()[i][j].alloc, 1, MPI_INT, i + 1, i + 1, Comm, &status);
        MPI_Recv(&gates[0].gstats()[i][j].currTstep, 1, MPI_DOUBLE, i + 1, i + 1, Comm, &status);
        MPI_Recv(&gates[0].gstats()[i][j].procActServs, 1, MPI_INT, i + 1, i + 1, Comm, &status);
        MPI_Recv(&gates[0].gstats()[i][j].memActServs, 1, MPI_DOUBLE, i + 1, i + 1, Comm, &status);
        MPI_Recv(&gates[0].gstats()[i][j].stoActServs, 1, MPI_DOUBLE, i + 1, i + 1, Comm, &status);
        MPI_Recv(&gates[0].gstats()[i][j].accActServs, 1, MPI_INT, i + 1, i + 1, Comm, &status);
        MPI_Recv(&gates[0].gstats()[i][j].phyMem, 1, MPI_DOUBLE, i + 1, i + 1, Comm, &status);
        MPI_Recv(&gates[0].gstats()[i][j].phyProc, 1, MPI_DOUBLE, i + 1, i + 1, Comm, &status);
        MPI_Recv(&gates[0].gstats()[i][j].phySto, 1, MPI_DOUBLE, i + 1, i + 1, Comm, &status);
        MPI_Recv(&gates[0].gstats()[i][j].phyNetw, 1, MPI_DOUBLE, i + 1, i + 1, Comm, &status);
        MPI_Recv(&gates[0].gstats()[i][j].totMem, 1, MPI_DOUBLE, i + 1, i + 1, Comm, &status);
        MPI_Recv(&gates[0].gstats()[i][j].totProc, 1, MPI_DOUBLE, i + 1, i + 1, Comm, &status);
        MPI_Recv(&gates[0].gstats()[i][j].totSto, 1, MPI_DOUBLE, i + 1, i + 1, Comm, &status);
        MPI_Recv(&gates[0].gstats()[i][j].availMem, 1, MPI_DOUBLE, i + 1, i + 1, Comm, &status);
        MPI_Recv(&gates[0].gstats()[i][j].availProc, 1, MPI_DOUBLE, i + 1, i + 1, Comm, &status);
        MPI_Recv(&gates[0].gstats()[i][j].availSto, 1, MPI_DOUBLE, i + 1, i + 1, Comm, &status);

        MPI_Recv(&gates[0].gstats()[i][j].autilMem, 1, MPI_DOUBLE, i + 1, i + 1, Comm, &status);
        MPI_Recv(&gates[0].gstats()[i][j].autilProc, 1, MPI_DOUBLE, i + 1, i + 1, Comm, &status);

        gates[0].gstats()[i][j].utilProc = gates[0].gstats()[i][j].totProc - gates[0].gstats()[i][j].availProc;
        gates[0].gstats()[i][j].utilMem = gates[0].gstats()[i][j].totMem - gates[0].gstats()[i][j].availMem;
        gates[0].gstats()[i][j].utilSto = gates[0].gstats()[i][j].totSto - gates[0].gstats()[i][j].availSto;

        //				MPI_Recv(&gates[0].gstats()[i][j].utilProc,1,MPI_DOUBLE,i+1,i+1,Comm,&status);
        //				MPI_Recv(&gates[0].gstats()[i][j].utilMem,1,MPI_DOUBLE,i+1,i+1,Comm,&status);
        //				MPI_Recv(&gates[0].gstats()[i][j].utilSto,1,MPI_DOUBLE,i+1,i+1,Comm,&status);

        MPI_Recv(&gates[0].gstats()[i][j].totNetw, 1, MPI_DOUBLE, i + 1, i + 1, Comm, &status);
        MPI_Recv(&gates[0].gstats()[i][j].availNetw, 1, MPI_DOUBLE, i + 1, i + 1, Comm, &status);

        MPI_Recv(&gates[0].gstats()[i][j].autilNetw, 1, MPI_DOUBLE, i + 1, i + 1, Comm, &status);

        //				MPI_Recv(&gates[0].gstats()[i][j].utilNetw,1,MPI_DOUBLE,i+1,i+1,Comm,&status);

        gates[0].gstats()[i][j].utilNetw = gates[0].gstats()[i][j].totNetw - gates[0].gstats()[i][j].availNetw;

        MPI_Recv(&gates[0].gstats()[i][j].totPcons, 1, MPI_DOUBLE, i + 1, i + 1, Comm, &status);
        MPI_Recv(&gates[0].gstats()[i][j].totAcc, 1, MPI_INT, i + 1, i + 1, Comm, &status);
        MPI_Recv(&gates[0].gstats()[i][j].availAcc, 1, MPI_INT, i + 1, i + 1, Comm, &status);

        //				MPI_Recv(&gates[0].gstats()[i][j].utilAcc,1,MPI_INT,i+1,i+1,Comm,&status);

        gates[0].gstats()[i][j].utilAcc = gates[0].gstats()[i][j].totAcc - gates[0].gstats()[i][j].availAcc;

        MPI_Recv(&gates[0].gstats()[i][j].activeSrvs, 1, MPI_INT, i + 1, i + 1, Comm, &status);
        MPI_Recv(&gates[0].gstats()[i][j].numOfTasks, 1, MPI_INT, i + 1, i + 1, Comm, &status);
        MPI_Recv(&gates[0].gstats()[i][j].rejTasks, 1, MPI_INT, i + 1, i + 1, Comm, &status);
        MPI_Recv(&gates[0].gstats()[i][j].accTasks, 1, MPI_INT, i + 1, i + 1, Comm, &status);
      }
    }
  } else {
    for (i = 0; i < clCell[0].gnumOfTypes(); i++) {
      MPI_Send(&clCell[0].gstats()[i].alloc, 1, MPI_INT, 0, rank, Comm);
      MPI_Send(&clCell[0].gstats()[i].currTstep, 1, MPI_DOUBLE, 0, rank, Comm);
      MPI_Send(&clCell[0].gstats()[i].procActServs, 1, MPI_INT, 0, rank, Comm);
      MPI_Send(&clCell[0].gstats()[i].memActServs, 1, MPI_DOUBLE, 0, rank, Comm);
      MPI_Send(&clCell[0].gstats()[i].stoActServs, 1, MPI_DOUBLE, 0, rank, Comm);
      MPI_Send(&clCell[0].gstats()[i].accActServs, 1, MPI_INT, 0, rank, Comm);
      MPI_Send(&clCell[0].gstats()[i].phyMem, 1, MPI_DOUBLE, 0, rank, Comm);
      MPI_Send(&clCell[0].gstats()[i].phyProc, 1, MPI_DOUBLE, 0, rank, Comm);
      MPI_Send(&clCell[0].gstats()[i].phySto, 1, MPI_DOUBLE, 0, rank, Comm);
      MPI_Send(&clCell[0].gstats()[i].phyNetw, 1, MPI_DOUBLE, 0, rank, Comm);
      MPI_Send(&clCell[0].gstats()[i].totMem, 1, MPI_DOUBLE, 0, rank, Comm);
      MPI_Send(&clCell[0].gstats()[i].totProc, 1, MPI_DOUBLE, 0, rank, Comm);
      MPI_Send(&clCell[0].gstats()[i].totSto, 1, MPI_DOUBLE, 0, rank, Comm);
      MPI_Send(&clCell[0].gstats()[i].availMem, 1, MPI_DOUBLE, 0, rank, Comm);
      MPI_Send(&clCell[0].gstats()[i].availProc, 1, MPI_DOUBLE, 0, rank, Comm);
      MPI_Send(&clCell[0].gstats()[i].availSto, 1, MPI_DOUBLE, 0, rank, Comm);

      MPI_Send(&clCell[0].gstats()[i].autilMem, 1, MPI_DOUBLE, 0, rank, Comm);
      MPI_Send(&clCell[0].gstats()[i].autilProc, 1, MPI_DOUBLE, 0, rank, Comm);

      //			MPI_Send(&clCell[0].gstats()[i].utilProc,1,MPI_DOUBLE,0,rank,Comm);
      //			MPI_Send(&clCell[0].gstats()[i].utilMem,1,MPI_DOUBLE,0,rank,Comm);
      //			MPI_Send(&clCell[0].gstats()[i].utilSto,1,MPI_DOUBLE,0,rank,Comm);

      MPI_Send(&clCell[0].gstats()[i].totNetw, 1, MPI_DOUBLE, 0, rank, Comm);
      MPI_Send(&clCell[0].gstats()[i].availNetw, 1, MPI_DOUBLE, 0, rank, Comm);

      MPI_Send(&clCell[0].gstats()[i].autilNetw, 1, MPI_DOUBLE, 0, rank, Comm);

      //			MPI_Send(&clCell[0].gstats()[i].utilNetw,1,MPI_DOUBLE,0,rank,Comm);

      MPI_Send(&clCell[0].gstats()[i].totPcons, 1, MPI_DOUBLE, 0, rank, Comm);
      MPI_Send(&clCell[0].gstats()[i].totAcc, 1, MPI_INT, 0, rank, Comm);
      MPI_Send(&clCell[0].gstats()[i].availAcc, 1, MPI_INT, 0, rank, Comm);

      //			MPI_Send(&clCell[0].gstats()[i].utilAcc,1,MPI_INT,0,rank,Comm);

      MPI_Send(&clCell[0].gstats()[i].activeSrvs, 1, MPI_INT, 0, rank, Comm);
      MPI_Send(&clCell[0].gstats()[i].numOfTasks, 1, MPI_INT, 0, rank, Comm);
      MPI_Send(&clCell[0].gstats()[i].rejTasks, 1, MPI_INT, 0, rank, Comm);
      MPI_Send(&clCell[0].gstats()[i].accTasks, 1, MPI_INT, 0, rank, Comm);
    }
  }
}
