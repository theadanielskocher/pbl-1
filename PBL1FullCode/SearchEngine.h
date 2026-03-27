#ifndef SEARCHENGINE_H
#define SEARCHENGINE_H

#include "TreeManager.h" // Chứa định nghĩa cấu trúc Person
#include <string>
#include <vector>

using namespace std;

extern Person* root; 

// --- 3. CÁC HÀM TÌM KIẾM ---

void FindAllMatches(Person* current, string targetName, vector<SearchResult>& results);

SearchResult SelectPersonWithProxy(string targetName);
Person* FindFirstMatchForImport(Person* rootNode, string targetName);

#endif // SEARCHENGINE_H