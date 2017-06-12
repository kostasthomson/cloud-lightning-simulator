#ifndef POWER_H
#define POWER_H
#include <inputs.h>
using namespace std;

class power
{
	private:
		int alloc;
		int typeCpu;
		int typeAcc;
		int accelerator;
		double cpuPmin,cpuPmax;
		double cpuC;
		int numOfPoints;
		double *cpubins;
		double *cpuP;
		double accPmin,accPmax;
		double accC;
		double *a,*b,*c,*d;
		
	public:
		power();
		~power();
		power(const powinputs &t);
		power(const power &t);
		power & operator=(const power & t);
		double modelCPU(double &u);
		double modelACC(double &rho, int &numAcc);
		double cpCons(double &u, double &rho, int &active, int &numAcc);
		int galloc() const;
		int gtypeCpu() const;
		int gtypeAcc() const;
		int gaccelerator() const;
		double gcpuPmin() const;
		double gcpuPmax() const;
		double gcpuC() const;
		int gnumOfPoints() const;
		double *gcpubins() const;
		double *gcpuP() const;
		double gaccPmin() const;
		double gaccPmax() const;
		double gaccC() const;
		double *ga() const;
		double *gb() const;
		double *gc() const;
		double *gd() const;
		void print() const;
};



#endif
