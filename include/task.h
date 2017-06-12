#ifndef TASK_H
#define TASK_H
#include <vector>
using namespace std;


class task 
{
    private:
		int type;
		int numOfAvailImpl;
		int *availImpl;
		double reqIns;
		int numOfVMs;
		double *reqPMNS;
		int *typeactPMN;
		double **minmaxactPMN;
		int *avAcc;
		double *rhoAcc;
		int alloc;
		//------------------------------
		//  IDs of nodes
		//  Executing the Task
		//------------------------------
		int *resourceIDs;
		//------------------------------
		//  Vector retaining current
		//  Utilization
		//------------------------------
		double *cUtilPMNr;
		        
    public:
	        task();
		task(const task &t);
		task & operator=(const task & t);
        	~task();

		//------------------------------
		//  Manual Constructor (for TCE)
		//------------------------------
		task(const int &L_type, const int &L_numOfAvailImpl,const int *L_availImpl,const double &L_reqIns, const int &L_numOfVMs, const double &L_reqP, const double &L_reqM, const double &L_reqN, const double &L_reqS, const int &L_typeactP, const int &L_typeactM, const int &L_typeactN, const double *L_minmaxactP, const double *L_minmaxactM, const double *L_minmaxactN, const int *L_avAcc, const double *L_rhoAcc);

		void reduceIns(const double & amount);
		int gtype() const;
		int gnumOfAvailImpl() const;
		int *gavailImpl() const;
		double greqIns() const;
		int gnumOfVMs() const;
		double *greqPMNS() const;
		int *gtypeactPMN() const;
		double **gminmaxactPMN() const;
		int *gavAcc() const;
		double *grhoAcc() const;
		int *gresourceIDs() const;
		double *gcUtilPMNr() const;
		int galloc() const;
		double getactP();
		double getactM();
		double getactN();
		void attachResources(const int *IDs);
		void detachResources();
		void compcUtilPMNr();
		void remapType(const int *type, const int &num);
		void reduceImpl(const int *type);
		
		//------------------------------
		//  Debugging
		//------------------------------
		void print() const;
};

#endif
