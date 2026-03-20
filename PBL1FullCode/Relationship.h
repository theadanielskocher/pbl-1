#ifndef RELATIONSHIP_H
#define RELATIONSHIP_H

#include "DataStructure.h"

vector<Person*> GetPathToRoot(Person* p);
string DetermineRelationship(Person* pA, Person* pB);

Person* GetAncestor(Person* start, int dA);
void CollectAtDepth(Person* root, int targetDepth, int currentDepth, vector<Person*>& results);
vector<Person*> FindRelativesByRelationship(Person* startPerson, string relationName);

#endif