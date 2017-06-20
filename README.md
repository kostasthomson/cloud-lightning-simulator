# CloudLightning Simulator

## Getting started

Clone the project

     $ git clone https://gsikotidis@bitbucket.org/cloudlightning/cloudlightning-simulator.git

## Prerequisites

* Required gcc versions 4.9-6.0 (https://gcc.gnu.org/)
* Required OpenMPI version 2.1 (https://www.open-mpi.org)

Install OpenMPI

     $ ./configure --prefix=/where/to/install
     $ make all install
     $ sudo apt-get install libopenmpi-dev

## Input and Output Data

Input and output JSON data files are located at the corresponding *input* and *output* folders of the project

##Project Build

Navigate to the directory of the project, open the terminal and enter

     $ cmake .
     $ make
     $ make install

##Run the CL Simulator

Change directory to the *bin* folder and enter

     $ sh cl_sim.out

##Deliverable

The deliverable 7.1.1 of the CloudLightning Project can be found on the following url:
http://cloudlightning.eu/work-packages/public-deliverables/