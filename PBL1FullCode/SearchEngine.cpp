#include "SearchEngine.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;


void FindAllMatches(Person* current, string targetName, vector<SearchResult>& results) {
    if (current == nullptr) return;

    if (current->name == targetName) {
        results.push_back({current, false});
    }

    if (current->spouseName == targetName) {
        results.push_back({current, true});
    }

    FindAllMatches(current->firstChild, targetName, results);
    FindAllMatches(current->nextSibling, targetName, results);
}

SearchResult SelectPersonWithProxy(string targetName) {
    vector<SearchResult> results;
    FindAllMatches(root, targetName, results);

    if (results.empty()) {
        cout << "\n\033[1;31m[Loi] Khong tim thay ai co ten la '" << targetName << "'!\033[0m\n";
        return {nullptr, false};
    }

    if (results.size() == 1) return results[0];

    cout << "\n\033[1;36m[Chu y] Tim thay " << results.size() << " ket qua cho ten '" << targetName << "':\033[0m\n";
    cout << "------------------------------------------------------\n";
    for (size_t i = 0; i < results.size(); i++) {
        cout << i + 1 << ". ";
        if (results[i].isSpouse) {
            cout << "[VO]  " << targetName << " (Vo cua " << results[i].node->name << ")";
        } else {
            cout << "[NUT] " << results[i].node->name << " (ID: " << results[i].node->id << ")";
        }
        
        if (results[i].node->parent != nullptr) 
            cout << " | Nhanh: " << results[i].node->parent->name;
        cout << endl;
    }
    cout << "------------------------------------------------------\n";

    int choice = 0;
    while (true) {
        cout << "Nhap so thu tu de chon: ";
        if (!(cin >> choice)) { 
            cin.clear(); 
            cin.ignore(1000, '\n'); 
            continue; 
        }
        if (choice >= 1 && choice <= (int)results.size()) {
            cin.ignore(1000, '\n');
            return results[choice - 1];
        }
        cout << "Lua chon khong hop le!\n";
    }
}

Person* FindFirstMatchForImport(Person* rootNode, string targetName) {
    vector<SearchResult> results;
    FindAllMatches(rootNode, targetName, results);
    
    if (results.empty()) return nullptr;
    
    for(auto res : results) {
        if (!res.isSpouse) return res.node;
    }
    return results[0].node;
}