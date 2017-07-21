#ifndef TCE_H
#define TCE_H
#include <gs.h>
#include <inputs.h>
#include <task.h>
#include <vector>

/// Creates one or more tasks based on AppData configuration
/// \param jobs List of jobs. The tasks created by the method are pushed back to the list
/// \param app Struct that holds information from the AppData configuration file
void taskCreationEngine(std::list<task>& jobs, const struct appinputs& app);

void taskImplSelect(std::list<task>& jobs);

/// Calls the gs::findCell method to retrieve the list of candidate cells for each task and select the most appropriate
/// \param jobs The list of tasks to be subsequently deployed
/// \param gates Pointer to the gateway object
/// \param commCells Integer array to store an index to the most appropriate cell for each task
void taskCellSelect(std::list<task>& jobs, const gs* gates, int** commCells);

#endif
