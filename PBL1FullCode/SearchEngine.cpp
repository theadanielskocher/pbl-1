#include "SearchEngine.h"  // 1. Luôn include file .h của chính nó
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstring>
#include <sstream>
#include <unordered_map>
#include <algorithm>
#include <cstdlib>
using namespace std;

void FindAllPersonsByName(Person* current, string targetName, vector<Person*>& results) {
    if (current == nullptr) return;
    if (current->name == targetName) results.push_back(current);
    FindAllPersonsByName(current->firstChild, targetName, results);
    FindAllPersonsByName(current->nextSibling, targetName, results);
}

Person* SelectPersonFromName(string targetName) {
    vector<Person*> results;
    FindAllPersonsByName(root, targetName, results);

    if (results.empty()) {
        cout << "\n[Loi] Khong tim thay ai co ten la '" << targetName << "'!\n";
        return nullptr;
    }

    if (results.size() == 1) return results[0]; 

    cout << "\n[Chu y] Tim thay " << results.size() << " nguoi cung ten '" << targetName << "':\n";
    cout << "------------------------------------------------------\n";
    for (size_t i = 0; i < results.size(); i++) {
        cout << i + 1 << ". ID: " << results[i]->id << " | Gioi: " << results[i]->gender;
        if (results[i]->parent != nullptr) cout << " | Con cua: " << results[i]->parent->name;
        else cout << " | (La Ong To)";
        cout << endl;
    }
    cout << "------------------------------------------------------\n";

    int choice = 0;
    while (true) {
        cout << "Nhap so thu tu (1 - " << results.size() << ") de chon: ";
        cin >> choice; cin.ignore(256, '\n'); 
        if (choice >= 1 && choice <= results.size()) return results[choice - 1];
        cout << "Lua chon khong hop le!\n";
    }
}

Person* FindFirstMatchForImport(Person* rootNode, string targetName) {
    vector<Person*> results;
    FindAllPersonsByName(rootNode, targetName, results);

    if (results.empty()) return nullptr;

    if (results.size() > 1) {
        // Cảnh báo nhưng không dừng chương trình
        cout << "[Canh bao Import] Tim thay " << results.size() 
             << " nguoi ten '" << targetName 
             << "'. Dang tu dong noi vao ID: " << results[0]->id << endl;
    }

    return results[0]; // Trả về người đầu tiên tìm thấy
}