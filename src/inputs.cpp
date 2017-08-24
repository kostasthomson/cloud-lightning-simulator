#include <resource.h>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <jsoncons/json.hpp>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::setprecision;

brinputs::brinputs()
  : alloc(0),
    numberOfFunctions(0),
    Ws(NULL),
    initResPervRM(0),
    initvRMPerpSwitch(0),
    initpSwitchPerpRouter(0),
    pollIntervalCellM(0.0),
    pollIntervalpRouter(0.0),
    pollIntervalpSwitch(0.0),
    pollIntervalvRM(0.0),
    vRMdeploystrategy(0)
{
}

brinputs::brinputs(const brinputs& t)
{
  if (t.alloc) {
    alloc = t.alloc;
    numberOfFunctions = t.numberOfFunctions;
    Ws = new double[numberOfFunctions];

    for (int i = 0; i < numberOfFunctions; i++) {
      Ws[i] = t.Ws[i];
    }

    initResPervRM = t.initResPervRM;
    initvRMPerpSwitch = t.initvRMPerpSwitch;
    initpSwitchPerpRouter = t.initpSwitchPerpRouter;
    pollIntervalCellM = t.pollIntervalCellM;
    pollIntervalpRouter = t.pollIntervalpRouter;
    pollIntervalpSwitch = t.pollIntervalpSwitch;
    pollIntervalvRM = t.pollIntervalvRM;
    vRMdeploystrategy = t.vRMdeploystrategy;
  }
}

brinputs& brinputs::operator=(const brinputs& t)
{
  if (this != &t) {
    if (alloc) {
      alloc = 0;
      numberOfFunctions = 0;
      delete[] Ws;
      Ws = NULL;
      initResPervRM = 0;
      initvRMPerpSwitch = 0;
      initpSwitchPerpRouter = 0;
      pollIntervalCellM = 0.0;
      pollIntervalpRouter = 0.0;
      pollIntervalpSwitch = 0.0;
      pollIntervalvRM = 0.0;
      vRMdeploystrategy = 0;
    }
    alloc = t.alloc;
    if (alloc) {
      numberOfFunctions = t.numberOfFunctions;
      Ws = new double[numberOfFunctions];

      for (int i = 0; i < numberOfFunctions; i++) {
        Ws[i] = t.Ws[i];
      }

      initResPervRM = t.initResPervRM;
      initvRMPerpSwitch = t.initvRMPerpSwitch;
      initpSwitchPerpRouter = t.initpSwitchPerpRouter;
      pollIntervalCellM = t.pollIntervalCellM;
      pollIntervalpRouter = t.pollIntervalpRouter;
      pollIntervalpSwitch = t.pollIntervalpSwitch;
      pollIntervalvRM = t.pollIntervalvRM;
      vRMdeploystrategy = t.vRMdeploystrategy;
    }
  }
  return *this;
}

brinputs::~brinputs()
{
  if (alloc) {
    alloc = 0;
    numberOfFunctions = 0;
    delete[] Ws;
    Ws = NULL;
    initResPervRM = 0;
    initvRMPerpSwitch = 0;
    initpSwitchPerpRouter = 0;
    pollIntervalCellM = 0.0;
    pollIntervalpRouter = 0.0;
    pollIntervalpSwitch = 0.0;
    pollIntervalvRM = 0.0;
    vRMdeploystrategy = 0;
  }
}

void brinputs::print()
{
  if (alloc) {
    cout << "    =========== Broker ===========" << endl;
    cout << "        Number of Assessment Functions: " << numberOfFunctions << endl;
    cout << "        Weights: ";
    for (int i = 0; i < numberOfFunctions; i++) {
      cout << Ws[i] << " ";
    }
    cout << endl;
    cout << "        Initial number of Resources per vRM: " << initResPervRM << endl;
    cout << "        Initial number of vRMs per pSwitch: " << initvRMPerpSwitch << endl;
    cout << "        Initial number of pSwitches per pRouter: " << initpSwitchPerpRouter << endl;
    cout << "        Poll Interval for the Cell Manager: " << pollIntervalCellM << endl;
    cout << "        Poll Interval for the pRouteres: " << pollIntervalpRouter << endl;
    cout << "        Poll Interval for the pSwitches: " << pollIntervalpSwitch << endl;
    cout << "        Poll Interval for the vRMs: " << pollIntervalvRM << endl;
    cout << "        vRM deployment strategy: " << vRMdeploystrategy << endl;
    cout << endl;
  }
}

void brinputs::parse(const string& outname, int i)
{
  // Read the JSON text into a json value
  std::ifstream is(outname);
  jsoncons::json b;
  is >> b;

  int k = i;
  int j;
  alloc = 1;
  numberOfFunctions = b["Brokers"][k]["Number of functions"].as<int>();
  Ws=new double[numberOfFunctions];
  for(j=0;j<numberOfFunctions;j++){
    Ws[j] = b["Brokers"][k]["Weights of functions"][j].as<double>();
  }
  initResPervRM = b["Brokers"][k]["Number of Resources per vRM"].as<int>();
  initvRMPerpSwitch = b["Brokers"][k]["Number of vRMs per pSwitch"].as<int>();
  initpSwitchPerpRouter = b["Brokers"][k]["Number of pSwitch per pRouter"].as<int>();
  pollIntervalCellM = b["Brokers"][k]["Poll Interval Cell Manager"].as<int>();
  pollIntervalpRouter = b["Brokers"][k]["Poll Interval pRouter"].as<int>();
  pollIntervalpSwitch = b["Brokers"][k]["Poll Interval pSwitch"].as<int>();
  pollIntervalvRM = b["Brokers"][k]["Poll Interval vRM"].as<int>();
  vRMdeploystrategy = b["Brokers"][k]["vRM deployment strategy"].as<int>();

  /*
    outname>>numberOfFunctions;
    Ws=new double[numberOfFunctions];
    for(i=0;i<numberOfFunctions;i++)
      outname>>Ws[i];
    outname>>initResPervRM;
    outname>>initvRMPerpSwitch;
    outname>>initpSwitchPerpRouter;
    outname>>pollIntervalCellM;
    outname>>pollIntervalpRouter;
    outname>>pollIntervalpSwitch;
    outname>>pollIntervalvRM;
    outname>>vRMdeploystrategy;
  */
}

void brinputs::printfile(const string& outname, const ios::openmode& mode, int sosmIntegration)
{
  fstream file;
  int i;
  if (alloc) {
    file.open(outname.c_str(), mode);
    file << "    =========== Broker ===========" << endl;
    if (sosmIntegration){
      file << "        Number of Assessment Functions: " << numberOfFunctions << endl;
      file << "        Weights: ";
      for (i = 0; i < numberOfFunctions; i++) {
          file << Ws[i] << " ";
      }
      file << endl;
      file << "        Initial number of Resources per vRM: " << initResPervRM << endl;
      file << "        Initial number of vRMs per pSwitch: " << initvRMPerpSwitch << endl;
      file << "        Initial number of pSwitches per pRouter: " << initpSwitchPerpRouter << endl;
      file << "        Poll Interval for the Cell Manager: " << pollIntervalCellM << endl;
      file << "        Poll Interval for the pRouteres: " << pollIntervalpRouter << endl;
      file << "        Poll Interval for the pSwitches: " << pollIntervalpSwitch << endl;
      file << "        Poll Interval for the vRMs: " << pollIntervalvRM << endl;
      file << "        vRM deployment strategy: " << vRMdeploystrategy << endl;
    }
    else {
      file << "        Poll Interval for the Cell Manager: "<<pollIntervalCellM<<endl;
    }
    file << endl;
    file.close();
  }
}

appinputs::appinputs()
{
  alloc = 0;
  minmaxJobsPerSec = NULL;
  numOfApps = 0;
  numberOfAvailableImplementationsPerApp = NULL;
  availableImplementationsPerApp = NULL;
  minmaxInsPerApp = NULL;
  minmaxVMPerApp = NULL;
  minmaxProcPerVM = NULL;
  minmaxMemPerVM = NULL;
  minmaxStoPerVM = NULL;
  minmaxNetPerApp = NULL;
  typeOfActP = NULL;
  typeOfActM = NULL;
  typeOfActN = NULL;
  minmaxActP = NULL;
  minmaxActM = NULL;
  minmaxActN = NULL;
  accelerator = NULL;
  rhoAcc = NULL;
}

appinputs::appinputs(const appinputs& t)
{
  int i, j;
  if (t.alloc) {
    alloc = t.alloc;
    minmaxJobsPerSec = new double[2];
    for (i = 0; i < 2; i++) {
      minmaxJobsPerSec[i] = t.minmaxJobsPerSec[i];
    }
    numOfApps = t.numOfApps;
    numberOfAvailableImplementationsPerApp = new int[numOfApps];
    typeOfActP = new int[numOfApps];
    typeOfActM = new int[numOfApps];
    typeOfActN = new int[numOfApps];
    for (i = 0; i < numOfApps; i++) {
      numberOfAvailableImplementationsPerApp[i] = t.numberOfAvailableImplementationsPerApp[i];
      typeOfActP[i] = t.typeOfActP[i];
      typeOfActM[i] = t.typeOfActM[i];
      typeOfActN[i] = t.typeOfActN[i];
    }

    availableImplementationsPerApp = new int*[numOfApps];
    minmaxInsPerApp = new double*[numOfApps];
    minmaxVMPerApp = new int*[numOfApps];
    minmaxProcPerVM = new double*[numOfApps];
    minmaxMemPerVM = new double*[numOfApps];
    minmaxStoPerVM = new double*[numOfApps];
    minmaxNetPerApp = new double*[numOfApps];
    minmaxActP = new double*[numOfApps];
    minmaxActM = new double*[numOfApps];
    minmaxActN = new double*[numOfApps];
    accelerator = new int*[numOfApps];
    rhoAcc = new double*[numOfApps];

    for (i = 0; i < numOfApps; i++) {
      availableImplementationsPerApp[i] = new int[numberOfAvailableImplementationsPerApp[i]];
      accelerator[i] = new int[numberOfAvailableImplementationsPerApp[i]];
      rhoAcc[i] = new double[numberOfAvailableImplementationsPerApp[i]];
      for (j = 0; j < numberOfAvailableImplementationsPerApp[i]; j++) {
        availableImplementationsPerApp[i][j] = t.availableImplementationsPerApp[i][j];
        accelerator[i][j] = t.accelerator[i][j];
        rhoAcc[i][j] = t.rhoAcc[i][j];
      }
      minmaxInsPerApp[i] = new double[2];
      minmaxVMPerApp[i] = new int[2];
      minmaxProcPerVM[i] = new double[2];
      minmaxMemPerVM[i] = new double[2];
      minmaxStoPerVM[i] = new double[2];
      minmaxNetPerApp[i] = new double[2];
      minmaxActP[i] = new double[2];
      minmaxActM[i] = new double[2];
      minmaxActN[i] = new double[2];
      for (j = 0; j < 2; j++) {
        minmaxInsPerApp[i][j] = t.minmaxInsPerApp[i][j];
        minmaxVMPerApp[i][j] = t.minmaxVMPerApp[i][j];
        minmaxProcPerVM[i][j] = t.minmaxProcPerVM[i][j];
        minmaxMemPerVM[i][j] = t.minmaxMemPerVM[i][j];
        minmaxStoPerVM[i][j] = t.minmaxStoPerVM[i][j];
        minmaxNetPerApp[i][j] = t.minmaxNetPerApp[i][j];
        minmaxActP[i][j] = t.minmaxActP[i][j];
        minmaxActM[i][j] = t.minmaxActM[i][j];
        minmaxActN[i][j] = t.minmaxActN[i][j];
      }
    }
  }
}

appinputs& appinputs::operator=(const appinputs& t)
{
  int i, j;
  if (this != &t) {
    if (alloc) {
      alloc = 0;
      delete[] minmaxJobsPerSec;
      delete[] numberOfAvailableImplementationsPerApp;
      delete[] typeOfActP;
      delete[] typeOfActM;
      delete[] typeOfActN;
      for (i = 0; i < numOfApps; i++) {
        delete[] availableImplementationsPerApp[i];
        delete[] minmaxInsPerApp[i];
        delete[] minmaxVMPerApp[i];
        delete[] minmaxProcPerVM[i];
        delete[] minmaxMemPerVM[i];
        delete[] minmaxStoPerVM[i];
        delete[] minmaxNetPerApp[i];
        delete[] minmaxActP[i];
        delete[] minmaxActM[i];
        delete[] minmaxActN[i];
        delete[] rhoAcc[i];
        delete[] accelerator[i];
      }
      delete[] availableImplementationsPerApp;
      delete[] minmaxInsPerApp;
      delete[] minmaxVMPerApp;
      delete[] minmaxProcPerVM;
      delete[] minmaxMemPerVM;
      delete[] minmaxStoPerVM;
      delete[] minmaxNetPerApp;
      delete[] minmaxActP;
      delete[] minmaxActM;
      delete[] minmaxActN;
      delete[] accelerator;
      delete[] rhoAcc;
      numberOfAvailableImplementationsPerApp = NULL;
      availableImplementationsPerApp = NULL;
      minmaxInsPerApp = NULL;
      minmaxVMPerApp = NULL;
      minmaxProcPerVM = NULL;
      minmaxMemPerVM = NULL;
      minmaxStoPerVM = NULL;
      minmaxNetPerApp = NULL;
      typeOfActP = NULL;
      typeOfActM = NULL;
      typeOfActN = NULL;
      minmaxActP = NULL;
      minmaxActM = NULL;
      minmaxActN = NULL;
      accelerator = NULL;
      rhoAcc = NULL;
      numOfApps = 0;
    }
    alloc = t.alloc;
    if (alloc) {
      minmaxJobsPerSec = new double[2];
      for (i = 0; i < 2; i++)
        minmaxJobsPerSec[i] = t.minmaxJobsPerSec[i];
      numOfApps = t.numOfApps;
      numberOfAvailableImplementationsPerApp = new int[numOfApps];
      typeOfActP = new int[numOfApps];
      typeOfActM = new int[numOfApps];
      typeOfActN = new int[numOfApps];
      for (i = 0; i < numOfApps; i++) {
        numberOfAvailableImplementationsPerApp[i] = t.numberOfAvailableImplementationsPerApp[i];
        typeOfActP[i] = t.typeOfActP[i];
        typeOfActM[i] = t.typeOfActM[i];
        typeOfActN[i] = t.typeOfActN[i];
      }

      availableImplementationsPerApp = new int*[numOfApps];
      minmaxInsPerApp = new double*[numOfApps];
      minmaxVMPerApp = new int*[numOfApps];
      minmaxProcPerVM = new double*[numOfApps];
      minmaxMemPerVM = new double*[numOfApps];
      minmaxStoPerVM = new double*[numOfApps];
      minmaxNetPerApp = new double*[numOfApps];
      minmaxActP = new double*[numOfApps];
      minmaxActM = new double*[numOfApps];
      minmaxActN = new double*[numOfApps];
      accelerator = new int*[numOfApps];
      rhoAcc = new double*[numOfApps];

      for (i = 0; i < numOfApps; i++) {
        availableImplementationsPerApp[i] = new int[numberOfAvailableImplementationsPerApp[i]];
        accelerator[i] = new int[numberOfAvailableImplementationsPerApp[i]];
        rhoAcc[i] = new double[numberOfAvailableImplementationsPerApp[i]];
        for (j = 0; j < numberOfAvailableImplementationsPerApp[i]; j++) {
          availableImplementationsPerApp[i][j] = t.availableImplementationsPerApp[i][j];
          accelerator[i][j] = t.accelerator[i][j];
          rhoAcc[i][j] = t.rhoAcc[i][j];
        }
        minmaxInsPerApp[i] = new double[2];
        minmaxVMPerApp[i] = new int[2];
        minmaxProcPerVM[i] = new double[2];
        minmaxMemPerVM[i] = new double[2];
        minmaxStoPerVM[i] = new double[2];
        minmaxNetPerApp[i] = new double[2];
        minmaxActP[i] = new double[2];
        minmaxActM[i] = new double[2];
        minmaxActN[i] = new double[2];
        for (j = 0; j < 2; j++) {
          minmaxInsPerApp[i][j] = t.minmaxInsPerApp[i][j];
          minmaxVMPerApp[i][j] = t.minmaxVMPerApp[i][j];
          minmaxProcPerVM[i][j] = t.minmaxProcPerVM[i][j];
          minmaxMemPerVM[i][j] = t.minmaxMemPerVM[i][j];
          minmaxStoPerVM[i][j] = t.minmaxStoPerVM[i][j];
          minmaxNetPerApp[i][j] = t.minmaxNetPerApp[i][j];
          minmaxActP[i][j] = t.minmaxActP[i][j];
          minmaxActM[i][j] = t.minmaxActM[i][j];
          minmaxActN[i][j] = t.minmaxActN[i][j];
        }
      }
    }
  }
  return *this;
}

void appinputs::parse(const string& fname)
{
  std::ifstream is(fname);
  jsoncons::json a;
  is >> a;

  int i, j;
  alloc = 1;
  numOfApps = a["Number of Applications"].as<int>();  //read number of applications and assign to variable
  minmaxJobsPerSec=new double[2];
  for(i=0;i<2;i++){
    minmaxJobsPerSec[i] = a["Minimum and Maximum Jobs Per Second"][i].as<double>();
  }

  // Initialization of application characteristics
  numberOfAvailableImplementationsPerApp = new int[numOfApps];
  availableImplementationsPerApp = new int*[numOfApps];
  minmaxInsPerApp = new double*[numOfApps];
  minmaxVMPerApp = new int*[numOfApps];
  minmaxProcPerVM = new double*[numOfApps];
  minmaxMemPerVM = new double*[numOfApps];
  minmaxStoPerVM = new double*[numOfApps];
  minmaxNetPerApp = new double*[numOfApps];
  minmaxActP = new double*[numOfApps];
  minmaxActM = new double*[numOfApps];
  minmaxActN = new double*[numOfApps];
  accelerator = new int*[numOfApps];
  rhoAcc = new double*[numOfApps];
  typeOfActP = new int[numOfApps];
  typeOfActM = new int[numOfApps];
  typeOfActN = new int[numOfApps];

  for(i=0;i<numOfApps;i++){

    numberOfAvailableImplementationsPerApp[i] = a["Applications"][i]["Number of Available Implementations"].as<int>();
    availableImplementationsPerApp[i]=new int[numberOfAvailableImplementationsPerApp[i]];
    for(j=0;j<numberOfAvailableImplementationsPerApp[i];j++){
        availableImplementationsPerApp[i][j] = a["Applications"][i]["Available Implementations"][j].as<int>() ;
      }
    minmaxInsPerApp[i]=new double[2];
    minmaxVMPerApp[i]=new int[2];
    minmaxProcPerVM[i]=new double[2];
    minmaxMemPerVM[i]=new double[2];
    minmaxStoPerVM[i]=new double[2];
    minmaxNetPerApp[i]=new double[2];

      for(j=0;j<2;j++){
        minmaxInsPerApp[i][j] = a["Applications"][i]["Minimum - Maximum Instructions Per App"][j].as<double>();
      }
      for(j=0;j<2;j++){
        minmaxVMPerApp[i][j] = a["Applications"][i]["Minimum - Maximum VMs Per App"][j].as<int>();
      }
      for(j=0;j<2;j++){
        minmaxProcPerVM[i][j] = a["Applications"][i]["Minimum - Maximum vCPUs Per VM"][j].as<int>();
      }
      for(j=0;j<2;j++){
        minmaxMemPerVM[i][j] = a["Applications"][i]["Minimum - Maximum Memory Per VM"][j].as<int>();
      }
      for(j=0;j<2;j++){
        minmaxStoPerVM[i][j] = a["Applications"][i]["Minimum - Maximum Storage Per VM"][j].as<double>();
      }
      for(j=0;j<2;j++){
        minmaxNetPerApp[i][j] = a["Applications"][i]["Minimum - Maximum Network Per VM"][j].as<double>();
      }

      typeOfActP[i] = a["Applications"][i]["Type of Actual Utilization (vCPU,Memory,Network)"][0].as<int>();
      typeOfActM[i] = a["Applications"][i]["Type of Actual Utilization (vCPU,Memory,Network)"][1].as<int>();
      typeOfActN[i] = a["Applications"][i]["Type of Actual Utilization (vCPU,Memory,Network)"][2].as<int>();

      minmaxActP[i]=new double[2];
      minmaxActM[i]=new double[2];
      minmaxActN[i]=new double[2];
      for(j=0;j<2;j++){
        minmaxActP[i][j] = a["Applications"][i]["Minimum - Maximum Actual vCPU Utilization"][j].as<double>();
      }
      for(j=0;j<2;j++){
        minmaxActM[i][j] = a["Applications"][i]["Minimum - Maximum Actual Memory Utilization"][j].as<double>();
      }
      for(j=0;j<2;j++){
        minmaxActN[i][j] = a["Applications"][i]["Minimum - Maximum Actual Network Utilization"][j].as<double>();
      }
      accelerator[i]=new int[numberOfAvailableImplementationsPerApp[i]];
      for(j=0;j<numberOfAvailableImplementationsPerApp[i];j++){
        accelerator[i][j] = a["Applications"][i]["Accelerator support"][j].as<int>();
      }
      rhoAcc[i]=new double[numberOfAvailableImplementationsPerApp[i]];
      for(j=0;j<numberOfAvailableImplementationsPerApp[i];j++){
        rhoAcc[i][j] = a["Applications"][i]["Rho for accelerator execution per Implementation"][j].as<double>();
      }
    }
  /*ifstream file;
  int i,j;
  file.open(fname.c_str());
  if (file)
  {
    alloc=1;
    minmaxJobsPerSec=new double[2];
    for(i=0;i<2;i++)
      file>>minmaxJobsPerSec[i];
    file>>numOfApps;
    numberOfAvailableImplementationsPerApp=new int[numOfApps];
    availableImplementationsPerApp=new int*[numOfApps];
    minmaxInsPerApp=new double*[numOfApps];
    minmaxVMPerApp=new int*[numOfApps];
    minmaxProcPerVM=new double*[numOfApps];
    minmaxMemPerVM=new double*[numOfApps];
    minmaxStoPerVM=new double*[numOfApps];
    minmaxNetPerApp=new double*[numOfApps];
    minmaxActP=new double*[numOfApps];
    minmaxActM=new double*[numOfApps];
    minmaxActN=new double*[numOfApps];
    accelerator=new int*[numOfApps];
    rhoAcc=new double*[numOfApps];
    typeOfActP=new int[numOfApps];
    typeOfActM=new int[numOfApps];
    typeOfActN=new int[numOfApps];
    for(i=0;i<numOfApps;i++)
    {

      file>>numberOfAvailableImplementationsPerApp[i];
      availableImplementationsPerApp[i]=new int[numberOfAvailableImplementationsPerApp[i]];
      for(j=0;j<numberOfAvailableImplementationsPerApp[i];j++)
      {
        file>>availableImplementationsPerApp[i][j];
      }

      minmaxInsPerApp[i]=new double[2];
      minmaxVMPerApp[i]=new int[2];
      minmaxProcPerVM[i]=new double[2];
      minmaxMemPerVM[i]=new double[2];
      minmaxStoPerVM[i]=new double[2];
      minmaxNetPerApp[i]=new double[2];

      for(j=0;j<2;j++)
      {
        file>>minmaxInsPerApp[i][j];
      }
      for(j=0;j<2;j++)
      {
        file>>minmaxVMPerApp[i][j];
      }
      for(j=0;j<2;j++)
      {
        file>>minmaxProcPerVM[i][j];
      }
      for(j=0;j<2;j++)
      {
        file>>minmaxMemPerVM[i][j];
      }
      for(j=0;j<2;j++)
      {
        file>>minmaxStoPerVM[i][j];
      }
      for(j=0;j<2;j++)
      {
        file>>minmaxNetPerApp[i][j];
      }

      file>>typeOfActP[i]>>typeOfActM[i]>>typeOfActN[i];

      minmaxActP[i]=new double[2];
      minmaxActM[i]=new double[2];
      minmaxActN[i]=new double[2];
      for(j=0;j<2;j++)
      {
        file>>minmaxActP[i][j];
      }
      for(j=0;j<2;j++)
      {
        file>>minmaxActM[i][j];
      }
      for(j=0;j<2;j++)
      {
        file>>minmaxActN[i][j];
      }
      accelerator[i]=new int[numberOfAvailableImplementationsPerApp[i]];
      for(j=0;j<numberOfAvailableImplementationsPerApp[i];j++)
      {
        file>>accelerator[i][j];
      }
      rhoAcc[i]=new double[numberOfAvailableImplementationsPerApp[i]];
      for(j=0;j<numberOfAvailableImplementationsPerApp[i];j++)
      {
        file>>rhoAcc[i][j];
      }
    }
  }	*/
}

void appinputs::print()
{
  int i, j;
  if (alloc) {
    cout << "=========== App Catalog ===========" << endl;
    cout << "Number of Applications: " << numOfApps << endl;
    cout << "Minimum and Maximum Jobs Per Second: ";
    for (i = 0; i < 2; i++)
      cout << minmaxJobsPerSec[i] << " ";
    cout << endl;
    cout << "-----------------------------------" << endl;
    for (i = 0; i < numOfApps; i++) {
      cout << "     Application: " << i + 1 << endl;
      cout << "     Number of Available Implementations: " << numberOfAvailableImplementationsPerApp[i] << endl;
      cout << "     Available Implementations: ";
      for (j = 0; j < numberOfAvailableImplementationsPerApp[i]; j++)
        cout << availableImplementationsPerApp[i][j] << " ";
      cout << endl;
      cout << "     Minimum - Maximum Instructions Per App: ";
      for (j = 0; j < 2; j++) {
        cout << minmaxInsPerApp[i][j] << " ";
      }
      cout << endl;
      cout << "     Minimum - Maximum VMs Per App: ";
      for (j = 0; j < 2; j++) {
        cout << minmaxVMPerApp[i][j] << " ";
      }
      cout << endl;
      cout << "     Minimum - Maximum vCPUs Per VM: ";
      for (j = 0; j < 2; j++) {
        cout << minmaxProcPerVM[i][j] << " ";
      }
      cout << endl;
      cout << "     Minimum - Maximum Memory Per VM: ";
      for (j = 0; j < 2; j++) {
        cout << minmaxMemPerVM[i][j] << " ";
      }
      cout << "GBytes" << endl;
      cout << "     Minimum - Maximum Storage Per VM: ";
      for (j = 0; j < 2; j++) {
        cout << minmaxStoPerVM[i][j] << " ";
      }
      cout << "TBytes" << endl;
      cout << "     Minimum - Maximum Network Per VM: ";
      for (j = 0; j < 2; j++) {
        cout << minmaxNetPerApp[i][j] << " ";
      }
      cout << "Gbps" << endl;
      cout << "     Type of Actual Utilization (vCPU,Memory,Network): " << typeOfActP[i] << " " << typeOfActM[i] << " "
           << typeOfActN[i] << endl;
      cout << "     Minimum - Maximum Actual vCPU Utilization: ";
      for (j = 0; j < 2; j++) {
        cout << minmaxActP[i][j] << " ";
      }
      cout << endl;
      cout << "     Minimum - Maximum Actual Memory Utilization: ";
      for (j = 0; j < 2; j++) {
        cout << minmaxActM[i][j] << " ";
      }
      cout << endl;
      cout << "     Minimum - Maximum Actual Network Utilization: ";
      for (j = 0; j < 2; j++) {
        cout << minmaxActN[i][j] << " ";
      }
      cout << endl;
      cout << "     Accelerator support: ";
      for (j = 0; j < numberOfAvailableImplementationsPerApp[i]; j++) {
        cout << accelerator[i][j] << " ";
      }
      cout << endl;
      cout << "     Rho for accelerator execution per Implementation: ";
      for (j = 0; j < numberOfAvailableImplementationsPerApp[i]; j++) {
        cout << rhoAcc[i][j] << " ";
      }
      cout << endl;
      cout << "-----------------------------------" << endl;
    }
  }
}

void appinputs::printfile(const string& outname, const ios::openmode& mode)
{
  int i, j;
  fstream file;
  if (alloc) {
    file.open(outname.c_str(), mode);
    file << "=========== App Catalog ===========" << endl;
    file << "Number of Applications: " << numOfApps << endl;
    file << "Minimum and Maximum Jobs Per Second: ";
    for (i = 0; i < 2; i++)
      file << minmaxJobsPerSec[i] << " ";
    file << endl;
    file << "-----------------------------------" << endl;
    for (i = 0; i < numOfApps; i++) {
      file << "     Application: " << i + 1 << endl;
      file << "     Number of Available Implementations: " << numberOfAvailableImplementationsPerApp[i] << endl;
      file << "     Available Implementations: ";
      for (j = 0; j < numberOfAvailableImplementationsPerApp[i]; j++)
        file << availableImplementationsPerApp[i][j] << " ";
      file << endl;
      file << "     Minimum - Maximum Instructions Per App: ";
      for (j = 0; j < 2; j++) {
        file << minmaxInsPerApp[i][j] << " ";
      }
      file << endl;
      file << "     Minimum - Maximum VMs Per App: ";
      for (j = 0; j < 2; j++) {
        file << minmaxVMPerApp[i][j] << " ";
      }
      file << endl;
      file << "     Minimum - Maximum vCPUs Per VM: ";
      for (j = 0; j < 2; j++) {
        file << minmaxProcPerVM[i][j] << " ";
      }
      file << endl;
      file << "     Minimum - Maximum Memory Per VM: ";
      for (j = 0; j < 2; j++) {
        file << minmaxMemPerVM[i][j] << " ";
      }
      file << "GBytes" << endl;
      file << "     Minimum - Maximum Storage Per VM: ";
      for (j = 0; j < 2; j++) {
        file << minmaxStoPerVM[i][j] << " ";
      }
      file << "TBytes" << endl;
      file << "     Minimum - Maximum Network Per VM: ";
      for (j = 0; j < 2; j++) {
        file << minmaxNetPerApp[i][j] << " ";
      }
      file << "Gbps" << endl;
      file << "     Type of Actual Utilization (vCPU,Memory,Network): " << typeOfActP[i] << " " << typeOfActM[i] << " "
           << typeOfActN[i] << endl;
      file << "     Minimum - Maximum Actual vCPU Utilization: ";
      for (j = 0; j < 2; j++) {
        file << minmaxActP[i][j] << " ";
      }
      file << endl;
      file << "     Minimum - Maximum Actual Memory Utilization: ";
      for (j = 0; j < 2; j++) {
        file << minmaxActM[i][j] << " ";
      }
      file << endl;
      file << "     Minimum - Maximum Actual Network Utilization: ";
      for (j = 0; j < 2; j++) {
        file << minmaxActN[i][j] << " ";
      }
      file << endl;
      file << "     Accelerator support: ";
      for (j = 0; j < numberOfAvailableImplementationsPerApp[i]; j++) {
        file << accelerator[i][j] << " ";
      }
      file << endl;
      file << "     Rho for accelerator execution per Implementation: ";
      for (j = 0; j < numberOfAvailableImplementationsPerApp[i]; j++) {
        file << rhoAcc[i][j] << " ";
      }
      file << endl;
      file << "-----------------------------------" << endl;
    }
    file.close();
  }
}

appinputs::~appinputs()
{
  int i;
  if (alloc) {
    alloc = 0;
    delete[] minmaxJobsPerSec;
    delete[] numberOfAvailableImplementationsPerApp;
    delete[] typeOfActP;
    delete[] typeOfActM;
    delete[] typeOfActN;
    for (i = 0; i < numOfApps; i++) {
      delete[] availableImplementationsPerApp[i];
      delete[] minmaxInsPerApp[i];
      delete[] minmaxVMPerApp[i];
      delete[] minmaxProcPerVM[i];
      delete[] minmaxMemPerVM[i];
      delete[] minmaxStoPerVM[i];
      delete[] minmaxNetPerApp[i];
      delete[] minmaxActP[i];
      delete[] minmaxActM[i];
      delete[] minmaxActN[i];
      delete[] accelerator[i];
      delete[] rhoAcc[i];
    }
    delete[] availableImplementationsPerApp;
    delete[] minmaxInsPerApp;
    delete[] minmaxVMPerApp;
    delete[] minmaxProcPerVM;
    delete[] minmaxMemPerVM;
    delete[] minmaxStoPerVM;
    delete[] minmaxNetPerApp;
    delete[] minmaxActP;
    delete[] minmaxActM;
    delete[] minmaxActN;
    delete[] accelerator;
    delete[] rhoAcc;
    numberOfAvailableImplementationsPerApp = NULL;
    availableImplementationsPerApp = NULL;
    minmaxInsPerApp = NULL;
    minmaxVMPerApp = NULL;
    minmaxProcPerVM = NULL;
    minmaxMemPerVM = NULL;
    minmaxStoPerVM = NULL;
    minmaxNetPerApp = NULL;
    typeOfActP = NULL;
    typeOfActM = NULL;
    typeOfActN = NULL;
    minmaxActP = NULL;
    minmaxActM = NULL;
    minmaxActN = NULL;
    accelerator = NULL;
    rhoAcc = NULL;
    numOfApps = 0;
  }
}

netinputs::netinputs()
{
  alloc = 0;
  netBW = 0.0;
  overCommitmentNetwork = 1.0;
}

netinputs::netinputs(const netinputs& t)
{
  if (t.alloc) {
    alloc = t.alloc;
    netBW = t.netBW;
    overCommitmentNetwork = t.overCommitmentNetwork;
  }
}

netinputs& netinputs::operator=(const netinputs& t)
{
  if (this != &t) {
    if (alloc) {
      alloc = 0;
      netBW = 0.0;
      overCommitmentNetwork = 1.0;
    }
    alloc = t.alloc;
    if (alloc) {
      netBW = t.netBW;
      overCommitmentNetwork = t.overCommitmentNetwork;
    }
  }
  return *this;
}

void netinputs::print()
{
  if (alloc) {
    cout << "=====Network Interconnection=====" << endl;
    cout << "     Interconnection Bandwidth: " << netBW << " Gbps" << endl;
    cout << "     Bandwidth Over Commitment Ration: " << overCommitmentNetwork << endl;
    cout << "=================================" << endl;
  }
}

netinputs::~netinputs()
{
  if (alloc) {
    alloc = 0;
    netBW = 0.0;
    overCommitmentNetwork = 1.0;
  }
}

powinputs::powinputs()
{
  alloc = 0;
  typeCpu = 0;
  typeAcc = 0;
  cpuPmin = 0.0;
  cpuPmax = 0.0;
  cpuC = 0.0;
  numOfPoints = 0;
  cpubins = NULL;
  cpuP = NULL;
  accelerator = 0;
  accPmin = 0.0;
  accPmax = 0.0;
  accC = 0.0;
}

powinputs::powinputs(const powinputs& t)
{
  int i;
  if (t.alloc) {
    alloc = t.alloc;
    typeCpu = t.typeCpu;
    typeAcc = t.typeAcc;
    cpuPmin = t.cpuPmin;
    cpuPmax = t.cpuPmax;
    cpuC = t.cpuC;
    numOfPoints = t.numOfPoints;
    if (numOfPoints > 0) {
      cpubins = new double[numOfPoints];
      cpuP = new double[numOfPoints];
      for (i = 0; i < numOfPoints; i++) {
        cpubins[i] = t.cpubins[i];
        cpuP[i] = t.cpuP[i];
      }
    } else {
      cpubins = NULL;
      cpuP = NULL;
    }
    accelerator = t.accelerator;
    accPmin = t.accPmin;
    accPmax = t.accPmax;
    accC = t.accC;
  }
}

powinputs& powinputs::operator=(const powinputs& t)
{
  int i;
  if (this != &t) {
    if (alloc) {
      alloc = 0;
      typeCpu = 0;
      typeAcc = 0;
      cpuPmin = 0.0;
      cpuPmax = 0.0;
      cpuC = 0.0;
      accelerator = 0;
      accPmin = 0.0;
      accPmax = 0.0;
      accC = 0.0;
      if (numOfPoints != 0) {
        numOfPoints = 0;
        delete[] cpubins;
        delete[] cpuP;
      }
    }
    alloc = t.alloc;
    if (alloc) {
      typeCpu = t.typeCpu;
      typeAcc = t.typeAcc;
      cpuPmin = t.cpuPmin;
      cpuPmax = t.cpuPmax;
      cpuC = t.cpuC;
      numOfPoints = t.numOfPoints;
      if (numOfPoints > 0) {
        cpubins = new double[numOfPoints];
        cpuP = new double[numOfPoints];
        for (i = 0; i < numOfPoints; i++) {
          cpubins[i] = t.cpubins[i];
          cpuP[i] = t.cpuP[i];
        }
      } else {
        cpubins = NULL;
        cpuP = NULL;
      }
      accelerator = t.accelerator;
      accPmin = t.accPmin;
      accPmax = t.accPmax;
      accC = t.accC;
    }
  }
  return *this;
}

powinputs::~powinputs()
{
  if (alloc) {
    alloc = 0;
    typeCpu = 0;
    typeAcc = 0;
    cpuPmin = 0.0;
    cpuPmax = 0.0;
    cpuC = 0.0;
    accelerator = 0;
    accPmin = 0.0;
    accPmax = 0.0;
    accC = 0.0;
    if (numOfPoints != 0) {
      numOfPoints = 0;
      delete[] cpubins;
      delete[] cpuP;
    }
  }
}

resinputs::resinputs()
{
  alloc = 0;
  numOfProcUnits = 0.0;
  totalMemory = 0.0;
  totalStorage = 0.0;
  overcommitmentProcessors = 0.0;
  overcommitmentMemory = 0.0;
  computeCapability = 0.0;
  accelerator = 0;
  acceleratorComputeCapability = 0.0;
  totalAccelerators = 0;
  type = 0;
}

resinputs::resinputs(const resinputs& t)
{
  if (t.alloc) {
    alloc = t.alloc;
    numOfProcUnits = t.numOfProcUnits;
    totalMemory = t.totalMemory;
    totalStorage = t.totalStorage;
    overcommitmentProcessors = t.overcommitmentProcessors;
    overcommitmentMemory = t.overcommitmentMemory;
    computeCapability = t.computeCapability;
    accelerator = t.accelerator;
    acceleratorComputeCapability = t.acceleratorComputeCapability;
    totalAccelerators = t.totalAccelerators;
    type = t.type;
  }
}

resinputs& resinputs::operator=(const resinputs& t)
{
  if (this != &t) {
    if (alloc) {
      alloc = 0;
      numOfProcUnits = 0.0;
      totalMemory = 0.0;
      totalStorage = 0.0;
      overcommitmentProcessors = 0.0;
      computeCapability = 0.0;
      accelerator = 0;
      acceleratorComputeCapability = 0.0;
      totalAccelerators = 0;
      type = 0;
    }
    alloc = t.alloc;
    if (alloc) {
      alloc = t.alloc;
      numOfProcUnits = t.numOfProcUnits;
      totalMemory = t.totalMemory;
      totalStorage = t.totalStorage;
      overcommitmentProcessors = t.overcommitmentProcessors;
      overcommitmentMemory = t.overcommitmentMemory;
      computeCapability = t.computeCapability;
      accelerator = t.accelerator;
      acceleratorComputeCapability = t.acceleratorComputeCapability;
      totalAccelerators = t.totalAccelerators;
      type = t.type;
    }
  }
  return *this;
}

resinputs::~resinputs()
{
  if (alloc) {
    alloc = 0;
    numOfProcUnits = 0.0;
    totalMemory = 0.0;
    totalStorage = 0.0;
    overcommitmentProcessors = 0.0;
    computeCapability = 0.0;
    accelerator = 0;
    acceleratorComputeCapability = 0.0;
    totalAccelerators = 0;
    type = 0;
  }
}

cellinputs::cellinputs()
{
  alloc = 0;
  ID = 0;
  numberOfTypes = 0;
  types = NULL;
  numberOfResourcesPerType = NULL;
  rinp = NULL;
  pinp = NULL;
  ninp = NULL;
  binp = NULL;
}

cellinputs::cellinputs(const cellinputs& t)
{
  int i;
  if (t.alloc) {
    ID = t.ID;
    alloc = t.alloc;
    numberOfTypes = t.numberOfTypes;
    types = new int[numberOfTypes];
    numberOfResourcesPerType = new int[numberOfTypes];
    rinp = new resinputs[numberOfTypes];
    pinp = new powinputs[numberOfTypes];
    ninp = new netinputs[1];
    binp = new brinputs[1];
    ninp[0] = t.ninp[0];
    binp[0] = t.binp[0];
    for (i = 0; i < numberOfTypes; i++) {
      types[i] = t.types[i];
      numberOfResourcesPerType[i] = t.numberOfResourcesPerType[i];
      rinp[i] = t.rinp[i];
      pinp[i] = t.pinp[i];
    }
  }
}

cellinputs::~cellinputs()
{
  if (alloc) {
    alloc = 0;
    ID = 0;
    numberOfTypes = 0;
    delete[] types;
    delete[] numberOfResourcesPerType;
    delete[] rinp;
    delete[] pinp;
    delete[] ninp;
    delete[] binp;
  }
}

cellinputs& cellinputs::operator=(const cellinputs& t)
{
  int i;
  if (this != &t) {
    if (alloc) {
      alloc = 0;
      ID = 0;
      numberOfTypes = 0;
      delete[] types;
      delete[] numberOfResourcesPerType;
      delete[] rinp;
      delete[] pinp;
      delete[] ninp;
      delete[] binp;
    }
    alloc = t.alloc;
    if (alloc) {
      ID = t.ID;
      numberOfTypes = t.numberOfTypes;
      types = new int[numberOfTypes];
      numberOfResourcesPerType = new int[numberOfTypes];
      rinp = new resinputs[numberOfTypes];
      pinp = new powinputs[numberOfTypes];
      ninp = new netinputs[1];
      binp = new brinputs[1];
      ninp[0] = t.ninp[0];
      binp[0] = t.binp[0];
      for (i = 0; i < numberOfTypes; i++) {
        types[i] = t.types[i];
        numberOfResourcesPerType[i] = t.numberOfResourcesPerType[i];
        rinp[i] = t.rinp[i];
        pinp[i] = t.pinp[i];
      }
    }
  }
  return *this;
}

siminputs::siminputs()
{
  alloc = 0;
  numOfCells = 0;
  maxTime = 0.0;
	sosmIntegration = 0;
  updateInterval = 0.0;
  cinp = NULL;
}

siminputs::siminputs(const siminputs& t)
{
  int i;
  if (t.alloc) {
    alloc = t.alloc;
    numOfCells = t.numOfCells;
    maxTime = t.maxTime;
		sosmIntegration = t.sosmIntegration;
    updateInterval = t.updateInterval;
    cinp = new cellinputs[numOfCells];
    for (i = 0; i < numOfCells; i++)
      cinp[i] = t.cinp[i];
  }
}

siminputs::~siminputs()
{
  if (alloc) {
    alloc = 0;
    numOfCells = 0;
    maxTime = 0.0;
		sosmIntegration = 0;
    updateInterval = 0.0;
    delete[] cinp;
  }
}

siminputs& siminputs::operator=(const siminputs& t)
{
  int i;
  if (this != &t) {
    if (alloc) {
      alloc = 0;
      numOfCells = 0;
      maxTime = 0.0;
      sosmIntegration = 0;
      updateInterval = 0.0;
      delete[] cinp;
    }
    alloc = t.alloc;
    if (alloc) {
      numOfCells = t.numOfCells;
      maxTime = t.maxTime;
			sosmIntegration = t.sosmIntegration;
      updateInterval = t.updateInterval;
      cinp = new cellinputs[numOfCells];
      for (i = 0; i < numOfCells; i++)
        cinp[i] = t.cinp[i];
    }
  }
  return *this;
}

void siminputs::parse(const string& fname, const string& bname)
{
  int i, j, k;
  //double dummy;

  std::ifstream is(fname);
  jsoncons::json c;
  is >> c;

  sosmIntegration = c["SOSM integration"].as<int>();  //0 for traditional , 1 for SOSM
  maxTime = c["Maximum simulation time"].as<int>();   //read maximum simulation time and assign to variable
  updateInterval = c["Update interval"].as<int>();      //read update interval and assign to variable
  numOfCells = c["Number of Cells"].as<int>();      //read number of cells and assign to variable
  cinp=new cellinputs[numOfCells];
  alloc=1;

  //---------------------------------------------
  // For each cell and for each hardware type,
  // initialization of characteristics and reading
  // data from file
  //---------------------------------------------
  for(i=0;i<numOfCells;i++){
    cinp[i].binp=new brinputs[1];
    cinp[i].binp[0].alloc=1;
    cinp[i].binp[0].parse(bname,i);
    cinp[i].numberOfTypes = c["Cells"][i]["Number of hardware(HW) types"].as<int>();
    cinp[i].alloc=1;
    cinp[i].ID=i+1;
    cinp[i].types=new int[cinp[i].numberOfTypes];
    cinp[i].numberOfResourcesPerType=new int[cinp[i].numberOfTypes];
    cinp[i].rinp=new resinputs[cinp[i].numberOfTypes];
    cinp[i].pinp=new powinputs[cinp[i].numberOfTypes];
    cinp[i].ninp=new netinputs[1];
    cinp[i].ninp[0].alloc=1;
    cinp[i].ninp[0].netBW = c["Cells"][i]["Cell interconnection bandwidth"].as<int>();
    cinp[i].ninp[0].overCommitmentNetwork = c["Cells"][i]["Network bandwidth overcommitment ratio"].as<double>();

    for(j=0;j<cinp[i].numberOfTypes;j++)
    {
        cinp[i].types[j] = c["Cells"][i]["HW types"][j]["HW type ID"].as<int>();
        cinp[i].numberOfResourcesPerType[j] = c["Cells"][i]["HW types"][j]["Number of servers"].as<int>();
        cinp[i].rinp[j].alloc=1;
        cinp[i].rinp[j].type=cinp[i].types[j];
        cinp[i].rinp[j].numOfProcUnits = c["Cells"][i]["HW types"][j]["Number of CPUs per server"].as<int>();
        cinp[i].rinp[j].totalMemory = c["Cells"][i]["HW types"][j]["Memory per server"].as<int>();
        cinp[i].rinp[j].totalStorage = c["Cells"][i]["HW types"][j]["Storage per server"].as<int>();
        cinp[i].rinp[j].overcommitmentProcessors = c["Cells"][i]["HW types"][j]["Processors overcommitment ratio"].as<double>();
        cinp[i].rinp[j].overcommitmentMemory = c["Cells"][i]["HW types"][j]["Memory overcommitment ratio"].as<double>();
        cinp[i].rinp[j].computeCapability = c["Cells"][i]["HW types"][j]["Compute capability"].as<double>();
        cinp[i].rinp[j].accelerator = c["Cells"][i]["HW types"][j]["Accelerators"].as<int>();
        cinp[i].rinp[j].totalAccelerators = c["Cells"][i]["HW types"][j]["Number of accelerators per server"].as<int>();
        cinp[i].rinp[j].acceleratorComputeCapability = c["Cells"][i]["HW types"][j]["Accelerator compute capability"].as<double>();
        cinp[i].pinp[j].alloc=1;
        cinp[i].pinp[j].accelerator=cinp[i].rinp[j].accelerator;
        cinp[i].pinp[j].typeCpu = c["Cells"][i]["HW types"][j]["Type of CPU model"].as<int>();
        cinp[i].pinp[j].cpuPmin = c["Cells"][i]["HW types"][j]["CPU idle power consumption"].as<double>();
        cinp[i].pinp[j].cpuPmax = c["Cells"][i]["HW types"][j]["CPU max power consumption"].as<double>();
        cinp[i].pinp[j].numOfPoints = c["Cells"][i]["HW types"][j]["CPU number of points for interpolation"].as<int>();
        if(cinp[i].pinp[j].numOfPoints>0)
        {
          cinp[i].pinp[j].cpubins=new double[cinp[i].pinp[j].numOfPoints];
          for(k=0;k<cinp[i].pinp[j].numOfPoints;k++)
            cinp[i].pinp[j].cpubins[k] = c["Cells"][i]["HW types"][j]["CPU utilization bins"][k].as<double>();
          cinp[i].pinp[j].cpuP=new double[cinp[i].pinp[j].numOfPoints];
          for(k=0;k<cinp[i].pinp[j].numOfPoints;k++)
            cinp[i].pinp[j].cpuP[k] = c["Cells"][i]["HW types"][j]["CPU power consumption"][k].as<double>();
        }
        else
        {
          //dummy = 0.0;
          //dummy = 0.0;
        }
        cinp[i].pinp[j].cpuC = c["Cells"][i]["HW types"][j]["CPU sleep power consumption"].as<double>();
        cinp[i].pinp[j].typeAcc = c["Cells"][i]["HW types"][j]["Type of accelerator model"].as<int>();
        cinp[i].pinp[j].accPmin = c["Cells"][i]["HW types"][j]["Accelerator idle power consumption"].as<double>();
        cinp[i].pinp[j].accPmax = c["Cells"][i]["HW types"][j]["Accelerator max power consumption"].as<double>();
        cinp[i].pinp[j].accC = c["Cells"][i]["HW types"][j]["Accelerator sleep power consumption"].as<double>();
    }
  }

  /*
  if (file && file2)
  {
    file>>maxTime;
    file>>updateInterval;
    file>>numOfCells;

    cinp=new cellinputs[numOfCells];
    alloc=1;
    //Looping over all Cells from the serial file
    for(i=0;i<numOfCells;i++)
    {

      cinp[i].binp=new brinputs[1];
      cinp[i].binp[0].alloc=1;
      cinp[i].binp[0].parse(file2);
      file>>cinp[i].numberOfTypes;
      cinp[i].alloc=1;
      cinp[i].ID=i+1;
      cinp[i].types=new int[cinp[i].numberOfTypes];
      cinp[i].numberOfResourcesPerType=new int[cinp[i].numberOfTypes];
      cinp[i].rinp=new resinputs[cinp[i].numberOfTypes];
      cinp[i].pinp=new powinputs[cinp[i].numberOfTypes];
      cinp[i].ninp=new netinputs[1];
      cinp[i].ninp[0].alloc=1;
      file>>cinp[i].ninp[0].netBW;
      file>>cinp[i].ninp[0].overCommitmentNetwork;
      for(j=0;j<cinp[i].numberOfTypes;j++)
      {
        file>>cinp[i].types[j];
        file>>cinp[i].numberOfResourcesPerType[j];
        cinp[i].rinp[j].alloc=1;
        cinp[i].rinp[j].type=cinp[i].types[j];
        file>>cinp[i].rinp[j].numOfProcUnits;
        file>>cinp[i].rinp[j].totalMemory;
        file>>cinp[i].rinp[j].totalStorage;
        file>>cinp[i].rinp[j].overcommitmentProcessors;
        file>>cinp[i].rinp[j].overcommitmentMemory;
        file>>cinp[i].rinp[j].computeCapability;
        file>>cinp[i].rinp[j].accelerator;
        file>>cinp[i].rinp[j].totalAccelerators;
        file>>cinp[i].rinp[j].acceleratorComputeCapability;
        cinp[i].pinp[j].alloc=1;
        cinp[i].pinp[j].accelerator=cinp[i].rinp[j].accelerator;
        file>>cinp[i].pinp[j].typeCpu;
        file>>cinp[i].pinp[j].cpuPmin;
        file>>cinp[i].pinp[j].cpuPmax;
        file>>cinp[i].pinp[j].numOfPoints;
        if(cinp[i].pinp[j].numOfPoints>0)
        {
          cinp[i].pinp[j].cpubins=new double[cinp[i].pinp[j].numOfPoints];
          for(k=0;k<cinp[i].pinp[j].numOfPoints;k++)
            file>>cinp[i].pinp[j].cpubins[k];
          cinp[i].pinp[j].cpuP=new double[cinp[i].pinp[j].numOfPoints];
          for(k=0;k<cinp[i].pinp[j].numOfPoints;k++)
            file>>cinp[i].pinp[j].cpuP[k];
        }
        else
        {
          file>>dummy;
          file>>dummy;
        }
        file>>cinp[i].pinp[j].cpuC;
        file>>cinp[i].pinp[j].typeAcc;
        file>>cinp[i].pinp[j].accPmin;
        file>>cinp[i].pinp[j].accPmax;
        file>>cinp[i].pinp[j].accC;
      }
    }
    file.close();
    file2.close();
  }
  else
  {
    cout<<endl<<"Catastrophic Error: File(s) not found"<<endl;
    exit(0);
  }
  */
}

// Print Simulation Inputs (Mainly for Debugging)
void siminputs::print()
{
  int i, j, k;
  if (alloc) {
    cout << "=========== Global Parameters ===========" << endl;
    cout << "Maximum Simulation Time: " << maxTime << " seconds" << endl;
    if (sosmIntegration){
      cout<<"Resource Allocation Mechanism: SOSM"<<endl;
    }
    else{
      cout<<"Resource Allocation Mechanism: Traditional"<<endl;
    }
    cout << "Update Interval: " << updateInterval << " seconds" << endl;
    cout << endl;
    cout << "=========== Cell Parameters ===========" << endl;
    cout << "Number of Cells: " << numOfCells << endl;
    cout << "---------------------------------------" << endl;
    // Looping over all Cells from the serial file
    for (i = 0; i < numOfCells; i++) {
      if (cinp[i].alloc) {
        cout << "    Cell: " << cinp[i].ID << endl;
        cout << "         Number of HW Types: " << cinp[i].numberOfTypes << endl;
        if (cinp[i].ninp[0].alloc) {
          cout << "         Cell Interconnection Bandwidth: " << cinp[i].ninp[0].netBW << " Gbps" << endl;
          cout << "         Network Bandwidth Overcommitment ratio: " << cinp[i].ninp[0].overCommitmentNetwork << endl;
        }
        for (j = 0; j < cinp[i].numberOfTypes; j++) {
          if (cinp[i].rinp[j].alloc) {
            cout << "              HW Type: " << cinp[i].types[j] << endl;
            cout << "                   Number of Servers: " << cinp[i].numberOfResourcesPerType[j] << endl;
            cout << "                   Number of Proc. Units per Server:" << cinp[i].rinp[j].numOfProcUnits << endl;
            cout << "                   Memory per Server: " << cinp[i].rinp[j].totalMemory << " GBytes" << endl;
            cout << "                   Storage per Server: " << cinp[i].rinp[j].totalStorage << " TBytes" << endl;

            cout << "                   Proc. Overcommitment ratio: " << cinp[i].rinp[j].overcommitmentProcessors << endl;
            cout << "                   Memory Overcommitment ratio: " << cinp[i].rinp[j].overcommitmentMemory << endl;
            cout << setprecision(10) << "                   Compute Capability: " << cinp[i].rinp[j].computeCapability << " MIPS"
                 << endl;
            cout << "                   Accelerators: " << cinp[i].rinp[j].accelerator << endl;
            cout << "                   Total Number of Accelerators per Server: " << cinp[i].rinp[j].totalAccelerators << endl;
            cout << "                   Accelerator Compute Capability: " << cinp[i].rinp[j].acceleratorComputeCapability << " MIPS"
                 << endl;
          }
          if (cinp[i].pinp[j].alloc) {
            cout << "                   Type of CPU model: " << cinp[i].pinp[j].typeCpu << endl;
            if (cinp[i].pinp[j].typeCpu < 0) {
              cout << "                   CPU Idle Power Consumption: " << cinp[i].pinp[j].cpuPmin << " Watts" << endl;
              cout << "                   CPU Max Power Consumption: " << cinp[i].pinp[j].cpuPmax << " Watts" << endl;
            } else if (cinp[i].pinp[j].typeCpu > 0) {
              cout << "                   CPU Number of Points for Interpolation: " << cinp[i].pinp[j].numOfPoints
                   << endl;
              cout << "                   CPU Utilization Bins: ";
              for (k = 0; k < cinp[i].pinp[j].numOfPoints; k++)
                cout << cinp[i].pinp[j].cpubins[k] << " ";
              cout << endl;
              cout << "                   CPU Power Consuption: ";
              for (k = 0; k < cinp[i].pinp[j].numOfPoints; k++)
                cout << cinp[i].pinp[j].cpuP[k] << " ";
              cout << endl;
            }
            cout << "                   CPU Sleep Power Consumption: " << cinp[i].pinp[j].cpuC << " Watts" << endl;
            if (cinp[i].pinp[j].accelerator) {
              cout << "                   Type of Accelerator Model: " << cinp[i].pinp[j].typeAcc << endl;
              cout << "                   Accelerator Idle Power Consumption: " << cinp[i].pinp[j].accPmin << " Watts"
                   << endl;
              cout << "                   Accelerator Max Power Consumption: " << cinp[i].pinp[j].accPmax << " Watts"
                   << endl;
              cout << "                   Accelerator Sleep Power Consumption: " << cinp[i].pinp[j].accC << " Watts"
                   << endl;
            }
          }
        }
        if (cinp[i].binp[0].alloc) {
          cinp[i].binp[0].print();
        }
      }
    }
    cout << "---------------------------------------" << endl;
    cout << endl;
  }
}

void siminputs::printfile(const string& outname, const ios::openmode& mode)
{
  int i, j, k;
  fstream file;
  if (alloc) {
    file.open(outname.c_str(), mode);

    file << "=========== Global Parameters ===========" << endl;
    file << "Maximum Simulation Time: " << maxTime << " seconds" << endl;
    if (sosmIntegration){
      file << "Resource Allocation Mechanism: SOSM" << endl;
    }
    else{
      file << "Resource Allocation Mechanism: Traditional" << endl;
    }
    file << "Update Interval: " << updateInterval << " seconds" << endl;
    file << endl;
    file << "=========== Cell Parameters ===========" << endl;
    file << "Number of Cells: " << numOfCells << endl;
    file << "---------------------------------------" << endl;
    // Looping over all Cells from the serial file
    for (i = 0; i < numOfCells; i++) {
      if (cinp[i].alloc) {
        file << "    Cell: " << cinp[i].ID << endl;
        file << "         Number of HW Types: " << cinp[i].numberOfTypes << endl;
        if (cinp[i].ninp[0].alloc) {
          file << "         Cell Interconnection Bandwidth: " << cinp[i].ninp[0].netBW << " Gbps" << endl;
          file << "         Network Bandwidth Overcommitment ratio: " << cinp[i].ninp[0].overCommitmentNetwork << endl;
        }
        for (j = 0; j < cinp[i].numberOfTypes; j++) {
          if (cinp[i].rinp[j].alloc) {
            file << "              HW Type: " << cinp[i].types[j] << endl;
            file << "                   Number of Servers: " << cinp[i].numberOfResourcesPerType[j] << endl;
            file << "                   Number of Proc. Units per Server:" << cinp[i].rinp[j].numOfProcUnits << endl;
            file << "                   Memory per Server: " << cinp[i].rinp[j].totalMemory << " GBytes" << endl;
            file << "                   Storage per Server: " << cinp[i].rinp[j].totalStorage << " TBytes" << endl;

            file << "                   Proc. Overcommitment ratio: " << cinp[i].rinp[j].overcommitmentProcessors << endl;
            file << "                   Memory Overcommitment ratio: " << cinp[i].rinp[j].overcommitmentMemory << endl;
            file << setprecision(10) << "                   Compute Capability: " << cinp[i].rinp[j].computeCapability << " MIPS"
                 << endl;
            file << "                   Accelerators: " << cinp[i].rinp[j].accelerator << endl;
            file << "                   Total Number of Accelerators per Server: " << cinp[i].rinp[j].totalAccelerators << endl;
            file << "                   Accelerator Compute Capability: " << cinp[i].rinp[j].acceleratorComputeCapability << " MIPS"
                 << endl;
          }
          if (cinp[i].pinp[j].alloc) {
            file << "                   Type of CPU model: " << cinp[i].pinp[j].typeCpu << endl;
            if (cinp[i].pinp[j].typeCpu < 0) {
              file << "                   CPU Idle Power Consumption: " << cinp[i].pinp[j].cpuPmin << " Watts" << endl;
              file << "                   CPU Max Power Consumption: " << cinp[i].pinp[j].cpuPmax << " Watts" << endl;
            } else if (cinp[i].pinp[j].typeCpu > 0) {
              file << "                   CPU Number of Points for Interpolation: " << cinp[i].pinp[j].numOfPoints
                   << endl;
              file << "                   CPU Utilization Bins: ";
              for (k = 0; k < cinp[i].pinp[j].numOfPoints; k++)
                file << cinp[i].pinp[j].cpubins[k] << " ";
              file << endl;
              file << "                   CPU Power Consuption: ";
              for (k = 0; k < cinp[i].pinp[j].numOfPoints; k++)
                file << cinp[i].pinp[j].cpuP[k] << " ";
              file << endl;
            }
            file << "                   CPU Sleep Power Consumption: " << cinp[i].pinp[j].cpuC << " Watts" << endl;
            if (cinp[i].pinp[j].accelerator) {
              file << "                   Type of Accelerator Model: " << cinp[i].pinp[j].typeAcc << endl;
              file << "                   Accelerator Idle Power Consumption: " << cinp[i].pinp[j].accPmin << " Watts"
                   << endl;
              file << "                   Accelerator Max Power Consumption: " << cinp[i].pinp[j].accPmax << " Watts"
                   << endl;
              file << "                   Accelerator Sleep Power Consumption: " << cinp[i].pinp[j].accC << " Watts"
                   << endl;
            }
          }
        }
        file.close();
        if (cinp[i].binp[0].alloc) {
          cinp[i].binp[0].printfile(outname, ios::out | ios::app, sosmIntegration);
        }
        file.open(outname.c_str(), ios::out | ios::app);
      }
    }
    file << "---------------------------------------" << endl;
    file << endl;
    file.close();
  }
}
