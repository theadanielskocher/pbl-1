#ifndef TREE_MANAGER_H
#define TREE_MANAGER_H

#include "DataStructure.h"

Person* CreateFamily(string name, string gender, string bday, int birthYear);
Person* AddChild(Person* parent, string name, string bday, string gender);
void LinkChildSorted(Person* parent, Person* child);
void DisplayTree(Person* current, int level);
void ShowDetail(SearchResult p);
void UpdatePersonInfo(Person* p);

void FreeTree(Person* current);
void ClearCurrentFamily();

#endif