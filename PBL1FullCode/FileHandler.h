#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include "DataStructure.h"
#include "TreeManager.h"
#include "SearchEngine.h"
#include "Relationship.h"

string trim(const string& str);
void FlattenTree(Person* current, vector<PersonRecord>& listRecords);
void SaveTreeToFile(string filename);
void ImportFromTextFile(string filename);
void LoadTreeFromFile(string filename);

#endif