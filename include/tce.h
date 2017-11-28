/* Copyright 2017 The CloudLightning Simulation Framework Authors. All
Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#ifndef TCE_H
#define TCE_H
#include <list>

class inputs;
class gs;
class task;

using std::list;

/// Creates one or more tasks based on AppData configuration
/// \param jobs List of jobs. The tasks created by the method are pushed back to
/// the list
/// \param app Struct that holds information from the AppData configuration file
void taskCreationEngine(std::list<task>& jobs, const class appinputs& app);

void taskImplSelect(std::list<task>& jobs);

/// Calls the gs::findCell method to retrieve the list of candidate cells for
/// each task and select the most appropriate
/// \param jobs The list of tasks to be subsequently deployed
/// \param gates Pointer to the gateway object
/// \param commCells Integer array to store an index to the most appropriate
/// cell for each task
void taskCellSelect(std::list<task>& jobs, const gs* gates, int** commCells);

#endif
