#ifndef CELL_H
#define CELL_H
#include <inputs.h>
#include <resource.h>
#include <task.h>
#include <power.h>
#include <netw.h>
#include <stat.h>
#include <brokers.h>
using namespace std;


class cell 
{
    private:
	int ID;
	int alloc;
	int numOfTypes;
	int *types;
	int *numOfResourcesPerType;
	power *powerComp;
	netw *network;
	broker *brok;
        resource **resources;
	stat *stats;
    public:

        //---------------------------------
        //        CONSTRUCTORS
        //---------------------------------
	cell();
	cell(const cellinputs & setup);
 	cell(const cell & t);

        //---------------------------------
        //        ASSIGNMENT OPERATOR
        //---------------------------------
	cell & operator=(const cell & t);
  

        //---------------------------------
        //        DECONSTRUCTOR
        //---------------------------------        
    	~cell();

        //---------------------------------
        //        TIMESTEPPING
        //---------------------------------
	void timestep(const double &tstep);
 

        //---------------------------------
        //        GATEWAYS
        //---------------------------------
	int gID() const;
	int galloc() const;
	int gnumOfTypes() const;
	int *gtypes() const;
	int *gnumOfResourcesPerType() const;
 	resource **gresources() const;
	broker *gbrok() const;
	power *gpowerComp() const;
	netw *gnetwork() const;
	stat *gstats() const;

        //---------------------------------
        //           DEPLOY
        //---------------------------------
	void deploy(list<task> *jobs);

        //---------------------------------
        //        UPDATE STATS
        //---------------------------------
	void updateStats(const double &tstep);	

        //---------------------------------
        //        PRINT
        //---------------------------------
	void print();
};

#endif
