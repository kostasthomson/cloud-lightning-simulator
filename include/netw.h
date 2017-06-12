#ifndef NETW_H
#define NETW_H
using namespace std;
#include <task.h>
#include <list>
#include <iterator>

class netw {
	private:
		int alloc;
		double phyNetw;
		double totalNetw;
		double utilNetw;
		double availNetw;
		double autilNetw;
		double overCommitNet;
		int numTasks;
	public:
       	 	//---------------------------------
        	//        CONSTRUCTORS
	        //---------------------------------
		
		netw();
		netw(const netinputs & setup);
		netw(const netw & t);

       	 	//---------------------------------
        	//        DECONSTRUCTORS
	        //---------------------------------
		
		~netw();
		
		//---------------------------------
		//        ASSIGNMENT OPERATOR
		//---------------------------------

		netw & operator=(const netw & t);		
		
		int galloc() const;
		double gphyNetw() const;
		double gtotalNetw() const;
		double gutilNetw() const;
		double gavailNetw() const;
		double gautilNetw() const;
		int gnumTasks() const;
		double goverCommitNet() const;
		void initRunQuan();
		void incrRunQuan(const double &uNetw);
		void sutilNetw(const double &L_utilNetw);
		void print() const;
		void deploy(const task * t);
		void unload(list<task>::iterator &t);
		void unload(const double &L_availNetw,const double &L_autilNetw,const int &L_numtasks);
		int probe(const double &reqNetw) const;

};


#endif
