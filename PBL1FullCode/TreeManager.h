#ifndef TREE_MANAGER_H
#define TREE_MANAGER_H

#include "DataStructure.h" // 2. Luôn include cấu trúc dữ liệu cơ sở

Person* CreateFamily(string name, string gender);
Person* AddChild(Person* parent, string name, string gender);
void DisplayTree(Person* current, int level);
void ShowDetail(Person* p);
void UpdatePersonInfo(Person* p);

void FreeTree(Person* current);
void ClearCurrentFamily();

#endif // Kết thúc Include Guard