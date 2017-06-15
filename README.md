# CloudLightning Simulator

## Getting started

Clone project:

   https://gsikotidis@bitbucket.org/cloudlightning/cloudlightning-simulator.git

Install dependencies:

   $ Download OpenMPI from https://www.open-mpi.org/
   $ Navigate to the directory of the downloaded file
   $ ./configure --prefix=/where/to/install
   $ make all install
   $ sudo apt-get install libopenmpi-dev

##Project Build

Navigate to the directory of the project and open the terminal

   $ Enter "make clean"
   $ Enter "make all"

##Run the CL Simulator

Change directory at the bin folder

   $ Enter "sh cl_sim.out"