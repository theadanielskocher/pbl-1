#ifndef SEARCH_ENGINE_H
#define SEARCH_ENGINE_H

#include "DataStructure.h"

void FindAllPersonsByName(Person* current, string targetName, vector<Person*>& results);
Person* SelectPersonFromName(string targetName);
Person* FindFirstMatchForImport(Person* rootNode, string targetName);

#endif