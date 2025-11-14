# CloudLightning Simulator

The CloudLightning Simulator is a generalized and extensible simulation framework that enables the seamless simulation and experimentation of emerging Cloud computing infrastructures and HPC applications. The framework is inherently parallel; it is written using the C/C++ programming language and the MPI and OpenMP APIs and enables the exploitation of distributed and shared memory parallel techniques for the acceleration of Cloud simulation.

The project proposes to create a new way of provisioning heterogeneous cloud resources to deliver services, specified by the user, using a bespoke service description language.

Our goal is to address energy inefficiencies particularly in the use of resources and consequently to deliver savings to the cloud provider and the cloud consumer in terms of reduced power consumption and improved service delivery, with hyperscale systems particularly in mind.

The architecture of the CloudLightning Simulator is presented in the following articles, please cite them if you use the simulator:

- Christos K. Filelis-Papadopoulos, George A. Gravvanis, Panagiotis E. Kyziropoulos: A framework for simulating large scale cloud infrastructures, Future Generation Computer Systems, Volume 79, Part 2, 2018, Pages 703-714, ISSN 0167-739X, https://doi.org/10.1016/j.future.2017.06.017.

- Christos K. Filelis-Papadopoulos, Konstantinos M. Giannoutakis, George A. Gravvanis, Tzovaras Dimitrios: Large-scale simulation of a self-organizing self-management cloud computing framework, The Journal of Supercomputing, 2017, https://doi.org/10.1007/s11227-017-2143-2

The project documentation can be found online at:

     http://www.iti.gr/~kouzinopoulos/

For feedback, troubleshooting and technical inquiries, please contact cpapad@ee.duth.gr or kgiannou@iti.gr

## Getting started

Clone the project:

     https://bitbucket.org/cloudlightning/cloudlightning-simulator

## Prerequisites

- Required GCC version 4.8 or higher
- Required OpenMPI version 2.1 or higher
- Required CURL version 8.5.0 or higher

## Using WSL

1. Create (or open if exist) .wslconfig file in your C:\Users\\<your_username>
2. Add:

```
[wsl2]
networkingMode=mirrored
```

## Input and Output Data

Input JSON data files are located at the corresponding _input_ folder of the project. In the _input_ folder, there are three JSON files containing the appropriate parameters about
applications, brokers and cells. Each of these parameters can be configured by the user before running the simulation. The _CellData.json_ file includes some global information such
as the maximum time of the simulation, the update interval of the simulation, the number of the cells and the integration (or not) of the SOSM system.

The _outputCLsim.json_ file will be generated in the _output_ folder, after the execution of the simulator.

## Project Build

Navigate to the directory of the project, open the terminal and enter

     $ cmake .
     $ make

## Documentation Build (optional)

To build the project documentation locally, navigate to the directory of the project, open the terminal and enter

     $ make doc

The documentation is created in the doc/html/ directory.

## Run the CloudLightning Simulator

At the directory of the project, open the terminal and enter

     $ sh cl_sim.out

After running the simulation, an _outputCLsim.json_ file will be created in the _output_ folder. This file can be used as an input for the GUI implementation of the CloudLightning Simulator in order
to visualize the results of the simulator.

The Simulator Visualization tool can be found at the following url:

     https://bitbucket.org/cloudlightning/cl-simulatorvisualization

## Deliverables

The deliverables of the CloudLightning Project can be found at the following url:

     http://cloudlightning.eu/work-packages/public-deliverables/

## Acknowledgements

Cloudlightning was funded by the European Commission’s Horizon 2020 Programme for Research and Innovation under Grand Agreement No. 643946.
