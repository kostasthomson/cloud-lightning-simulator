#ifndef TCE_H
#define TCE_H
#include <vector>
#include <task.h>
#include <inputs.h>
#include <gs.h>
using namespace std;


void taskCreationEngine(list<task> &jobs, const struct appinputs &app);
void taskImplSelect(list<task> &jobs);
void taskCellSelect(list<task> &jobs, const gs *gates, int **commCells);

#endif
