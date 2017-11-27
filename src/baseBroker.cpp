/* Copyright 2015 The CloudLightning Simulation Framework Authors. All
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

#include <baseBroker.h>
#include <cell.h>

baseBroker::baseBroker()
  : alloc(0),
    numberOfTypes(0),
    types(nullptr),
    numberOfResourcesPerType(nullptr),
    availableNetwork(0.0),
    totalNetwork(0.0)
{
}

baseBroker::~baseBroker() {}

void baseBroker::init(const cell* clCell)
{
  alloc = 1;
  numberOfTypes = clCell->getNumberOfTypes();
  types = new int[numberOfTypes];
  numberOfResourcesPerType = new int[numberOfTypes];

  for (int i = 0; i < numberOfTypes; i++) {
    types[i] = clCell->getTypes()[i];
    numberOfResourcesPerType[i] = clCell->getNumberOfResourcesPerType()[i];
  }
}

int baseBroker::galloc() const { return alloc; }
int baseBroker::getNumberOfTypes() const { return numberOfTypes; }
int* baseBroker::getTypes() const { return types; }
int* baseBroker::getNumberOfResourcesPerType() const { return numberOfResourcesPerType; }
double baseBroker::getTotalNetwork() const { return totalNetwork; }
double baseBroker::getAvailableNetwork() const { return availableNetwork; }
