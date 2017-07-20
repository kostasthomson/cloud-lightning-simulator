#include <resource.h>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <list>
#define MY_MIN(a, b) ((a) <= (b) ? (a) : (b))
#define MY_MAX(a, b) ((a) >= (b) ? (a) : (b))
using namespace std;

resource::resource()
{
  alloc = 0;
  active = 0;
  movable = 0;
  type = -1;
  ID = -1;

  totalProc = 0.0;
  availProc = 0.0;
  utilProc = 0.0;

  totalMem = 0.0;
  availMem = 0.0;
  utilMem = 0.0;

  totalSto = 0.0;
  availSto = 0.0;
  utilSto = 0.0;

  physProc = 0.0;
  physMem = 0.0;
  physSto = 0.0;

  compCap = 0.0;

  accelerator = 0;
  totAcc = 0;
  availAcc = 0;
  utilAcc = 0;
  accCompCap = 0.0;

  overCommitProc = 1.0;
  overCommitMem = 1.0;

  autilProc = 0.0;
  autilMem = 0.0;
  arhoAcc = 0.0;

  numOfTasks = 0;
  cCompCapPerProc = 0.0;
  cCompCapPerAcc = 0.0;
}

resource::resource(const resinputs& setup, const int& _ID)
{
  alloc = 1;
  active = 0;
  movable = 1;
  type = setup.type;
  ID = _ID;

  physProc = setup.numOfProcUnits;
  physMem = setup.totMem;
  physSto = setup.totSto;

  overCommitProc = setup.overCommitProc;
  totalProc = ((double)setup.numOfProcUnits) * overCommitProc;
  availProc = totalProc;
  utilProc = 0.0;

  overCommitMem = setup.overCommitMem;
  totalMem = ((double)setup.totMem) * overCommitMem;
  availMem = totalMem;
  utilMem = 0.0;

  totalSto = ((double)setup.totSto);
  availSto = totalSto;
  utilSto = 0.0;

  compCap = setup.compCap;

  accelerator = setup.accelerator;
  accCompCap = setup.accCompCap;
  totAcc = setup.totAcc;
  availAcc = totAcc;
  utilAcc = 0;

  autilProc = 0.0;
  autilMem = 0.0;
  arhoAcc = 0.0;

  numOfTasks = 0;
  cCompCapPerProc = 0.0;
  cCompCapPerAcc = 0.0;
}

resource::resource(const resource& t)
{
  if (t.galloc()) {
    alloc = t.galloc();
    active = t.gactive();
    movable = t.gmovable();
    type = t.gtype();
    ID = t.gID();

    totalProc = t.gtotalProc();
    availProc = t.gavailProc();
    utilProc = t.gutilProc();

    totalMem = t.gtotalMem();
    availMem = t.gavailMem();
    utilMem = t.gutilMem();

    physProc = t.gphysProc();
    physMem = t.gphysMem();

    compCap = t.gcompCap();

    accelerator = t.gaccelerator();
    totAcc = t.gtotAcc();
    utilAcc = t.gutilAcc();
    availAcc = t.gavailAcc();
    accCompCap = t.gaccCompCap();

    overCommitProc = t.goverCommitProc();
    overCommitMem = t.goverCommitMem();

    autilProc = t.gautilProc();
    autilMem = t.gautilMem();
    arhoAcc = t.garhoAcc();

    numOfTasks = t.gnumOfTasks();
    cCompCapPerProc = t.gcCompCapPerProc();
    cCompCapPerAcc = t.gcCompCapPerAcc();
  }
}

resource& resource::operator=(const resource& t)
{
  if (this != &t) {
    if (alloc) {
      alloc = 0;
      active = 0;
      movable = 0;
      type = -1;
      ID = -1;

      totalProc = 0.0;
      availProc = 0.0;
      utilProc = 0.0;

      totalMem = 0.0;
      availMem = 0.0;
      utilMem = 0.0;

      totalSto = 0.0;
      availSto = 0.0;
      utilSto = 0.0;

      physProc = 0.0;
      physMem = 0.0;
      physSto = 0.0;

      compCap = 0.0;

      accelerator = 0;
      totAcc = 0;
      utilAcc = 0;
      availAcc = 0;
      accCompCap = 0.0;

      overCommitProc = 1.0;
      overCommitMem = 1.0;

      autilProc = 0.0;
      autilMem = 0.0;
      arhoAcc = 0.0;

      numOfTasks = 0;
      cCompCapPerProc = 0.0;
      cCompCapPerAcc = 0.0;
    }
    alloc = t.galloc();
    if (alloc) {
      active = t.gactive();
      movable = t.gmovable();
      type = t.gtype();
      ID = t.gID();

      totalProc = t.gtotalProc();
      availProc = t.gavailProc();
      utilProc = t.gutilProc();

      totalMem = t.gtotalMem();
      availMem = t.gavailMem();
      utilMem = t.gutilMem();

      totalSto = t.gtotalSto();
      availSto = t.gavailSto();
      utilSto = t.gutilSto();

      physProc = t.gphysProc();
      physMem = t.gphysMem();
      physSto = t.gphysSto();

      compCap = t.gcompCap();

      accelerator = t.gaccelerator();
      totAcc = t.gtotAcc();
      availAcc = t.gavailAcc();
      utilAcc = t.gutilAcc();
      accCompCap = t.gaccCompCap();

      overCommitProc = t.goverCommitProc();
      overCommitMem = t.goverCommitMem();

      autilProc = t.gautilProc();
      autilMem = t.gautilMem();
      arhoAcc = t.garhoAcc();

      numOfTasks = t.gnumOfTasks();
      cCompCapPerProc = t.gcCompCapPerProc();
      cCompCapPerAcc = t.gcCompCapPerAcc();
    }
  }
  return *this;
}

resource::~resource()
{
  if (alloc) {
    alloc = 0;
    active = 0;
    movable = 0;
    type = -1;
    ID = -1;

    totalProc = 0.0;
    availProc = 0.0;
    utilProc = 0.0;

    totalMem = 0.0;
    availMem = 0.0;
    utilMem = 0.0;

    totalSto = 0.0;
    availSto = 0.0;
    utilSto = 0.0;

    physProc = 0.0;
    physMem = 0.0;
    physSto = 0.0;

    compCap = 0.0;

    accelerator = 0;
    totAcc = 0;
    utilAcc = 0;
    availAcc = 0;
    accCompCap = 0.0;

    overCommitProc = 1.0;
    overCommitMem = 1.0;

    autilProc = 0.0;
    autilMem = 0.0;
    arhoAcc = 0.0;

    numOfTasks = 0;
    cCompCapPerProc = 0.0;
    cCompCapPerAcc = 0.0;
  }
}

//---------------------------------
// INITIALIZE RUNNING QUANTITIES
//---------------------------------
void resource::initRunQuan()
{
  if (alloc) {
    autilProc = 0.0;
    autilMem = 0.0;
    arhoAcc = 0.0;
  }
}

void resource::incrRunQuan(const double& uProc, const double& uMem, const double& rAcc)
{
  if (alloc) {
    autilProc += uProc;
    autilMem += uMem;
    arhoAcc += rAcc;
  }
}

void resource::compcCompCapPerProc()
{
  double ratio;
  if (alloc) {
    ratio = (MY_MAX(utilProc, 1.0) / physProc);
    cCompCapPerProc = (compCap / physProc) / ratio;
  }
  // cap = (processor MIPS/physical processors) / (util processors/physical processsors)
}

void resource::compcCompCapPerAcc()
{
  double ratio;
  if (alloc && accelerator) {
    ratio = ceil(MY_MAX(utilProc, 1.0) / physProc);
    cCompCapPerAcc = (accCompCap / ratio);
  }
}

//---------------------------------
//       PROBE - ADDJOB
//---------------------------------
void resource::deploy(const task* t)
{
  numOfTasks++;
  active = 1;
  availProc -= t->greqPMNS()[0];
  availMem -= t->greqPMNS()[1];
  availSto -= t->greqPMNS()[3];
  availAcc -= t->gavAcc()[0];
  utilMem = totalMem - availMem;
  utilProc = totalProc - availProc;
  utilSto = totalSto - availSto;
  utilAcc = totAcc - availAcc;

  // If the task requests more than 1 VMs, dont allow the resource to be moved
  if (t->gnumOfVMs() > 1) {
    movable = 0;
  }
}

void resource::unload(const list<task>::iterator& t)
{
  numOfTasks--;
  if (numOfTasks == 0) {
    active = 0;
    availProc = totalProc;
    availMem = totalMem;
    availSto = totalSto;
    availAcc = totAcc;
    utilMem = 0.0;
    utilProc = 0.0;
    utilSto = 0.0;
    utilAcc = 0;
    autilProc = 0.0;
    autilMem = 0.0;
    arhoAcc = 0.0;
    cCompCapPerProc = 0.0;
    cCompCapPerAcc = 0.0;

    // If there are no remaining tasks assigned on a resource, allowed it to be moved to a different vRM during the
    // Self-Organization phase
    movable = 1;
  } else {
    availProc += t->greqPMNS()[0];
    availMem += t->greqPMNS()[1];
    availSto += t->greqPMNS()[3];
    availAcc += t->gavAcc()[0];
    utilMem = totalMem - availMem;
    utilProc = totalProc - availProc;
    utilSto = totalSto - availSto;
    utilAcc = totAcc - availAcc;
    autilProc -= t->gcUtilPMNr()[0];
    autilMem -= t->gcUtilPMNr()[1];
    arhoAcc -= t->gcUtilPMNr()[3];
  }
}

int resource::probe(const double& reqProc, const double& reqMem, const double& reqSto, const int& reqAcc)
{
  int choice = -1;
  if (reqProc <= availProc && reqMem <= availMem && reqSto <= availSto && reqAcc <= availAcc)
    choice = ID;
  return choice;
}

//---------------------------------
//        PRINT
//---------------------------------
void resource::print() const
{
  if (alloc) {
    cout << "ID: " << ID << endl;
    cout << "Active: " << active << endl;
    cout << "Movable: " << movable << endl;
    cout << "Type: " << type << endl;

    cout << "Total Proc: " << totalProc << endl;
    cout << "Avail Proc: " << availProc << endl;
    cout << "Util Proc: " << utilProc << endl;

    cout << "Total Memory: " << totalMem << endl;
    cout << "Avail Memory: " << availMem << endl;
    cout << "Util Memory: " << utilMem << endl;

    cout << "Total Storage: " << totalSto << endl;
    cout << "Avail Storage: " << availSto << endl;
    cout << "Util Storage: " << utilSto << endl;

    cout << "Physical Proc: " << physProc << endl;
    cout << "Physical Memory: " << physMem << endl;
    cout << "Physical Storage: " << physSto << endl;

    cout << "Proc Computational Capability: " << compCap << endl;

    cout << "Accelerator Availability: " << accelerator << endl;
    cout << "Total number of accelerators: " << totAcc << endl;
    cout << "Utilized accelerators: " << utilAcc << endl;
    cout << "Available accelerators: " << availAcc << endl;
    cout << "Accelerator Computational Capability: " << accCompCap << endl;

    cout << "Proc overcommitment ratio: " << overCommitProc << endl;
    cout << "Memory overcommitment ratio: " << overCommitMem << endl;

    cout << "Actual Processor Utilization: " << autilProc << endl;
    cout << "Actual Memory Utilization: " << autilMem << endl;
    cout << "Actual rho of Accelerators: " << arhoAcc << endl;

    cout << "Number of Tasks: " << numOfTasks << endl;
    cout << "Current Proc Computational Capability (per unit): " << cCompCapPerProc << endl;
    cout << "Current Acc Computational Capability (per unit): " << cCompCapPerAcc << endl;
  }
}

int resource::galloc() const { return alloc; }
int resource::gactive() const { return active; }
int resource::gmovable() const { return movable; }
int resource::gtype() const { return type; }
int resource::gID() const { return ID; }
double resource::gtotalProc() const { return totalProc; }
double resource::gavailProc() const { return availProc; }
double resource::gutilProc() const { return utilProc; }
double resource::gtotalMem() const { return totalMem; }
double resource::gavailMem() const { return availMem; }
double resource::gutilMem() const { return utilMem; }
double resource::gtotalSto() const { return totalSto; }
double resource::gavailSto() const { return availSto; }
double resource::gutilSto() const { return utilSto; }
double resource::gphysProc() const { return physProc; }
double resource::gphysMem() const { return physMem; }
double resource::gphysSto() const { return physSto; }
double resource::gcompCap() const { return compCap; }
int resource::gaccelerator() const { return accelerator; }
int resource::gtotAcc() const { return totAcc; }
int resource::gavailAcc() const { return availAcc; }
int resource::gutilAcc() const { return utilAcc; }
double resource::gaccCompCap() const { return accCompCap; }
double resource::goverCommitProc() const { return overCommitProc; }
double resource::goverCommitMem() const { return overCommitMem; }
double resource::gautilProc() const { return autilProc; }
double resource::gautilMem() const { return autilMem; }
double resource::garhoAcc() const { return arhoAcc; }
int resource::gnumOfTasks() const { return numOfTasks; }
double resource::gcCompCapPerProc() const { return cCompCapPerProc; }
double resource::gcCompCapPerAcc() const { return cCompCapPerAcc; }
