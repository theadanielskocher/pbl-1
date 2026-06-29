#ifndef SEARCHENGINE_H
#define SEARCHENGINE_H

#include "TreeManager.h"
#include <string>
#include <vector>

using namespace std;

extern Person* root;

void FindAllMatches(Person* current, string targetName, vector<SearchResult>& results);
SearchResult SelectPersonWithProxy(string targetName);
Person* FindFirstMatchForImport(Person* rootNode, string targetName);

#endif