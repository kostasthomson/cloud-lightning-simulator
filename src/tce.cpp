#include <task.h>
#include <inputs.h>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <tce.h>
#include <gs.h>
#include <list>
#define my_round(x) (x<0?ceil((x)-0.5):floor((x)+0.5))
using namespace std;

void taskCreationEngine(list<task> &jobs, const struct appinputs &app)
{
	srand(0);
	double r=((double)rand())/RAND_MAX;
	int numOfTasks,i;
	int curr_task,curr_numOfVMs;
	double curr_reqIns,curr_reqP,curr_reqM,curr_reqN,curr_reqS;
	numOfTasks=round(app.minmaxJobsPerSec[0]+r*(app.minmaxJobsPerSec[1]-app.minmaxJobsPerSec[0]));
	for(i=0;i<numOfTasks;i++)
	{
		r=((double)rand())/RAND_MAX;
		curr_task=floor(app.numOfApps*r);
		r=((double)rand())/RAND_MAX;
		curr_reqIns=app.minmaxInsPerApp[curr_task][0]+r*(app.minmaxInsPerApp[curr_task][1]-app.minmaxInsPerApp[curr_task][0]);
		curr_numOfVMs=round(app.minmaxVMPerApp[curr_task][0]+r*(app.minmaxVMPerApp[curr_task][1]-app.minmaxVMPerApp[curr_task][0]));

		// Requirements for resources
		curr_reqP=round(app.minmaxProcPerVM[curr_task][0]+r*(app.minmaxProcPerVM[curr_task][1]-app.minmaxProcPerVM[curr_task][0]));
		curr_reqM=app.minmaxMemPerVM[curr_task][0]+r*(app.minmaxMemPerVM[curr_task][1]-app.minmaxMemPerVM[curr_task][0]);
		curr_reqN=app.minmaxNetPerApp[curr_task][0]+r*(app.minmaxNetPerApp[curr_task][1]-app.minmaxNetPerApp[curr_task][0]);
		curr_reqS=app.minmaxStoPerVM[curr_task][0]+r*(app.minmaxStoPerVM[curr_task][1]-app.minmaxStoPerVM[curr_task][0]);

		jobs.push_back(task(curr_task,app.numOfAvailImplPerApp[curr_task],&app.availImplPerApp[curr_task][0],curr_reqIns, curr_numOfVMs, curr_reqP, curr_reqM, curr_reqN, curr_reqS, app.typeOfActP[curr_task], app.typeOfActM[curr_task], app.typeOfActN[curr_task], &app.minmaxActP[curr_task][0], &app.minmaxActM[curr_task][0], &app.minmaxActN[curr_task][0], &app.accelerator[curr_task][0], &app.rhoAcc[curr_task][0]));
	}
}

void taskImplSelect(list<task> &jobs)
{

	int numtasks=jobs.size(),impltype,i,j,k;
	double r;

	int L_type, L_numOfAvailImpl;
	int *L_availImpl;
	double L_reqIns;
	int L_numOfVMs;
	double *L_reqPMNS;
	int *L_typeactPMN;
	double **L_minmaxactPMN;
	int *L_avAcc;
	double *L_rhoAcc;
	list<task>::iterator it=jobs.begin();
	for(i=0;i<numtasks;i++)
	{
		r=((double)rand())/RAND_MAX;
		impltype=floor(r*((double)(*it).gnumOfAvailImpl()));
		L_numOfAvailImpl=1;
		L_type=(*it).gtype();
		L_availImpl=new int[1];
		L_availImpl[0]=(*it).gavailImpl()[impltype];
		L_reqIns=(*it).greqIns();
		L_numOfVMs=(*it).gnumOfVMs();
		L_reqPMNS=new double[4];
		for(j=0;j<4;j++)
		{
			L_reqPMNS[j]=(*it).greqPMNS()[j];
		}
		L_typeactPMN=new int[3];
		for(j=0;j<3;j++)
		{
			L_typeactPMN[j]=(*it).gtypeactPMN()[j];
		}
		L_minmaxactPMN=new double*[3];
		for(j=0;j<3;j++)
			L_minmaxactPMN[j]=new double[2];
		for(j=0;j<3;j++)
			for(k=0;k<2;k++)
				L_minmaxactPMN[j][k]=(*it).gminmaxactPMN()[j][k];
		L_avAcc=new int[1];
		L_rhoAcc=new double[1];
		L_avAcc[0]=(*it).gavAcc()[impltype];
		L_rhoAcc[0]=(*it).grhoAcc()[impltype];

		(*it)=(task(L_type,L_numOfAvailImpl,L_availImpl,L_reqIns, L_numOfVMs, L_reqPMNS[0], L_reqPMNS[1], L_reqPMNS[2], L_reqPMNS[3], L_typeactPMN[0], L_typeactPMN[1], L_typeactPMN[2], &L_minmaxactPMN[0][0], &L_minmaxactPMN[1][0], &L_minmaxactPMN[2][0], L_avAcc, L_rhoAcc));
		delete[] L_availImpl;
		delete[] L_reqPMNS;
		delete[] L_typeactPMN;
		for(j=0;j<3;j++)
			delete[] L_minmaxactPMN[j];
		delete[] L_avAcc;
		delete[] L_rhoAcc;
		it++;
	}
}

void taskCellSelect(list<task> &jobs, const gs *gates, int **commCells)
{
	int i;
	if (*commCells!=NULL)
	{
		delete[] (*commCells);
		(*commCells)=NULL;
	}
	if (jobs.size()!=0 && gates->galloc())
	{
		(*commCells)=new int[jobs.size()];
		for(i=0;i<(int)jobs.size();i++)
		{
			(*commCells)[i]=-1;
		}
		list<task>::iterator it=jobs.begin();
		for(i=0;i<(int)jobs.size();i++)
		{
			(*commCells)[i]=gates->findCell((*it).gavailImpl(),(*it).gnumOfAvailImpl(),(*it).gnumOfVMs(),(*it).greqPMNS()[0],(*it).greqPMNS()[1],(*it).greqPMNS()[2],(*it).greqPMNS()[3],(*it).gavAcc());
			it++;
		}
	}

}
