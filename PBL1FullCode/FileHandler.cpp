#include "FileHandler.h"  // 1. Luôn include file .h của chính nó
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

// Ham xoa khoang trang thua o hai dau chuoi (Trim) - Rat quan trong cho file Text
string trim(const string& str) {
    size_t first = str.find_first_not_of(' ');
    if (string::npos == first) return str;
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

void FlattenTree(Person* current, vector<PersonRecord>& listRecords) {
    if (current == nullptr) return;
    PersonRecord rec;
    rec.id = current->id;
    rec.parentId = (current->parent != nullptr) ? current->parent->id : 0;
    
    strncpy(rec.name, current->name.c_str(), 49); rec.name[49] = '\0';
    strncpy(rec.gender, current->gender.c_str(), 9); rec.gender[9] = '\0';
    strncpy(rec.birthday, current->birthday.c_str(), 19); rec.birthday[19] = '\0';
    
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
        cout << "\n[OK] Da luu " << total << " nguoi vao " << filename << "!\n";
    }
}


// --- HAM XU LY FILE TEXT (NHAP LIEU HANG LOAT) ---
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
        ClearCurrentFamily(); // Su dung ham xoa vua tao o tren
    }

    string line;
    int count = 0;
    while (getline(inFile, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string tenCha, tenCon, gioiTinh;

        // Dinh dang mong muon: TenCha, TenCon, GioiTinh
        getline(ss, tenCha, ',');
        getline(ss, tenCon, ',');
        getline(ss, gioiTinh, ',');
        
        // FIX BUG: Xoa khoang trang thua de chong loi chuoi (Trim)
        tenCha = trim(tenCha);
        tenCon = trim(tenCon);
        gioiTinh = trim(gioiTinh);

        if (tenCha == "None" || tenCha == "") {
            if (root == nullptr) {
                CreateFamily(tenCon, gioiTinh);
                count++;
            } else {
                cout << "- Bo qua Ong To [" << tenCon << "] vi gia pha da co goc!\n";
            }
        } else {
            Person* cha = FindFirstMatchForImport(root, tenCha);
            if (cha != nullptr) {
                AddChild(cha, tenCon, gioiTinh);
                count++;
            } else {
                cout << "- [Canh bao] Khong tim thay cha: '" << tenCha << "' de them con '" << tenCon << "'\n";
            }
        }
    }

    inFile.close();
    cout << "\n[OK] Da doc va them " << count << " thanh vien tu file text!\n";
}