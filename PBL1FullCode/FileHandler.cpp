#include "FileHandler.h" 
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

string trim(const string& str) {
    size_t first = str.find_first_not_of(' ');
    if (string::npos == first) return str;
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

int safeStoi(string s) {
    try {
        return stoi(trim(s));
    } catch (...) {
        return 0;
    }
}

void FlattenTree(Person* current, vector<PersonRecord>& listRecords) {
    if (current == nullptr) return;
    PersonRecord rec;
    rec.id = current->id;
    rec.parentId = (current->parent != nullptr) ? current->parent->id : 0;
    rec.birthYear = current->birthYear;
    rec.numChildren = current->numChildren;

    strncpy(rec.name, current->name.c_str(), 49); rec.name[49] = '\0';
    strncpy(rec.gender, current->gender.c_str(), 9); rec.gender[9] = '\0';
    strncpy(rec.birthday, current->birthday.c_str(), 19); rec.birthday[19] = '\0';
    
    strncpy(rec.job, current->job.c_str(), 49); rec.job[49] = '\0';
    strncpy(rec.deathDay, current->deathDay.c_str(), 19); rec.deathDay[19] = '\0';
    strncpy(rec.spouseName, current->spouseName.c_str(), 49); rec.spouseName[49] = '\0';
    
    listRecords.push_back(rec);
    FlattenTree(current->firstChild, listRecords);
    FlattenTree(current->nextSibling, listRecords);
}

void SaveTreeToFile(string filename) {
    if (root == nullptr) return;
    vector<PersonRecord> listRecords;
    FlattenTree(root, listRecords);

    ofstream outFile(filename, ios::binary);
    if (outFile.is_open()) {
        int total = listRecords.size();
        outFile.write((char*)&total, sizeof(int));
        outFile.write((char*)listRecords.data(), total * sizeof(PersonRecord));
        outFile.close();
        cout << "\n[OK] Da luu " << total << " nguoi vao " << filename << " (bao gom nam sinh)!\n";
    }
}

void ImportFromTextFile(string filename) {
    ifstream inFile(filename);
    if (!inFile.is_open()) {
        cout << "[Loi] Khong the mo file " << filename << ". Hay kiem tra lai ten file!\n";
        return;
    }
    
    if (root != nullptr) {
        char confirm;
        cout << "[Canh bao] Gia pha hien tai se bi xoa sach de nap du lieu moi.\n";
        cout << "Ban co chac chan muon tiep tuc? (y/n): ";
        cin >> confirm;
        if (confirm != 'y' && confirm != 'Y') {
            cout << "Huy bo thao tac nap file.\n";
            return;
        }
        ClearCurrentFamily();
    }

    string line;
    int count = 0;
    while (getline(inFile, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string tenCha, tenCon, gioiTinh, ngaySinh;
        int namSinh;

        getline(ss, tenCha, ',');
        getline(ss, tenCon, ',');
        getline(ss, gioiTinh, ',');
        getline(ss, ngaySinh, ',');
        
        tenCha = trim(tenCha);
        tenCon = trim(tenCon);
        gioiTinh = trim(gioiTinh);
        ngaySinh = trim(ngaySinh);
        namSinh = Person::extractYear(ngaySinh);

        if (tenCha == "None" || tenCha == "") {
            if (root == nullptr) {
                CreateFamily(tenCon, gioiTinh, ngaySinh, namSinh);
                count++;
            } else {
                cout << "- Bo qua Ong To [" << tenCon << "] vi gia pha da co goc!\n";
            }
        } else {
            Person* cha = FindFirstMatchForImport(root, tenCha);
            if (cha != nullptr) {
                if (cha->gender == "Nam") {
		            AddChild(cha, tenCon, ngaySinh, gioiTinh);
		            count++;
	        	} else {
		            cout << "- [Loi] Khong the them con cho Nu: '" << tenCha 
		                 << "'. Thao tac bi tu choi theo quy tac gia pha.\n";
	        	}
            } else {
                cout << "- [Canh bao] Khong tim thay cha: '" << tenCha << "' de them con '" << tenCon << "'\n";
            }
        }
    }

    inFile.close();
    cout << "\n[OK] Da doc va sap xep " << count << " thanh vien tu file text theo thu tu nam sinh!\n";
}

void LoadTreeFromFile(string filename) {
    ifstream inFile(filename, ios::binary);
    if (!inFile.is_open()) return;

    int total;
    inFile.read((char*)&total, sizeof(int));
    
    vector<PersonRecord> listRecords(total);
    inFile.read((char*)listRecords.data(), total * sizeof(PersonRecord));
    inFile.close();

    ClearCurrentFamily();
    unordered_map<int, Person*> idMap;
    int maxId = 0;

    for (const auto& rec : listRecords) {
        Person* p = new Person(rec.name, rec.gender, rec.birthYear, rec.birthday);
        p->id = rec.id;
        p->job = rec.job;
        p->deathDay = rec.deathDay;
        p->spouseName = rec.spouseName;
        p->numChildren = rec.numChildren;

        idMap[p->id] = p; 
        if (p->id > maxId) maxId = p->id;
    }

    for (const auto& rec : listRecords) {
        Person* child = idMap[rec.id];
        if (rec.parentId == 0) {
            root = child; 
        } else {
            if (idMap.count(rec.parentId)) {
                Person* pParent = idMap[rec.parentId];
                LinkChildSorted(pParent, child);
            }
        }
    }

    global_id_counter = maxId + 1; 
    cout << "\n[OK] Da phuc hoi hoan toan " << total << " thanh vien!\n";
}