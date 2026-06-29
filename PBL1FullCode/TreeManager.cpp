#include "TreeManager.h"
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

Person* root = nullptr;
int global_id_counter = 1;


Person::Person(string _name, string _gender, int _birthYear, string _birthday, Person* _parent) {
    id = global_id_counter++;
    name = _name;
    gender = _gender;
    birthYear = _birthYear;
    birthday = _birthday;
    parent = _parent;
    job = "Unknown";
    deathDay = "N/A";
    spouseName = "None";
    numChildren = 0;
    firstChild = nullptr;
    nextSibling = nullptr;
}


Person* CreateFamily(string name, string gender, string bday, int birthYear) {
    if (root != nullptr) return root;
    root = new Person(name, gender, birthYear, bday, nullptr);
    return root;
}

Person* AddChild(Person* parent, string name, string bday, string gender) {
    if (parent == nullptr || parent->gender == "Nu") return nullptr;

    int nSinh = Person::extractYear(bday);
    
    Person* newMember = new Person(name, gender, nSinh, bday, parent); 

    if (parent->firstChild == nullptr) {
        parent->firstChild = newMember;
    } 
    else if (newMember->birthYear < parent->firstChild->birthYear) {
        newMember->nextSibling = parent->firstChild;
        parent->firstChild = newMember;
    } 
    else {
        Person* curr = parent->firstChild;
        while (curr->nextSibling != nullptr && curr->nextSibling->birthYear <= newMember->birthYear) {
            curr = curr->nextSibling;
        }
        newMember->nextSibling = curr->nextSibling;
        curr->nextSibling = newMember;
    }
    return newMember;
}

void LinkChildSorted(Person* parent, Person* child) {
    if (parent == nullptr || child == nullptr) return;
    child->parent = parent;

    if (parent->firstChild == nullptr) {
        parent->firstChild = child;
    } 
    else if (child->birthYear < parent->firstChild->birthYear) {
        child->nextSibling = parent->firstChild;
        parent->firstChild = child;
    } 
    else {
        Person* curr = parent->firstChild;
        while (curr->nextSibling != nullptr && curr->nextSibling->birthYear <= child->birthYear) {
            curr = curr->nextSibling;
        }
        child->nextSibling = curr->nextSibling;
        curr->nextSibling = child;
    }
}

void DisplayTree(Person* current, int level) {
    if (current == nullptr) return;
    for (int i = 0; i < level; i++) cout << "    ";
    cout << "|-- [" << current->id << "] " << current->name << " (" << current->gender << ")" << endl;
    DisplayTree(current->firstChild, level + 1);
    DisplayTree(current->nextSibling, level);
}

void ShowDetail(SearchResult res) {
    if (res.node == nullptr) return;

    Person* p = res.node;
    cout << "\n" << DUT_BLUE << "========== THONG TIN CHI TIET ==========" << RESET << endl;

    if (res.isSpouse) {
        cout << BOLD << "Ho va ten: " << RESET << GREEN << p->spouseName << RESET << " (Phu nhan)" << endl;
        cout << BOLD << "Gioi tinh: " << RESET << "Nu" << endl;
        cout << BOLD << "Phu quan:  " << RESET << p->name << " (ID: " << p->id << ")" << endl;
        cout << BOLD << "Gia dinh:  " << RESET << "Nhanh cua " << (p->parent ? p->parent->name : "Ong To") << endl;
    } 
    else {
        cout << BOLD << "Ho va ten: " << RESET << GREEN << p->name << RESET << endl;
        cout << BOLD << "ID:        " << RESET << p->id << endl;
        cout << BOLD << "Gioi tinh: " << RESET << p->gender << endl;
        cout << BOLD << "Ngay sinh: " << RESET << p->birthday << endl;
        if (p->gender == "Nu") {
        	cout << BOLD << "Phu quan:  " << RESET << p->spouseName << endl;
		}
        else if (!p->spouseName.empty()) {
            cout << BOLD << "Phu nhan:  " << RESET << p->spouseName << endl;
        }
        
        if (p->parent != nullptr) {
            cout << BOLD << "Than sinh: " << RESET << p->parent->name << endl;
        }
    }
    cout << DUT_BLUE << "========================================" << RESET << endl;
}

void UpdatePersonInfo(Person* p) {
    if (p == nullptr) return;

    string temp;

    cout << "\n--- Cap nhat thong tin cho: [" << p->id << "] " << p->name << " ---\n";
    cout << "(Nhan Enter neu muon giu nguyen thong tin cu)\n";

    cout << "Ngay sinh hien tai [" << p->birthday << "]: ";
    getline(cin, temp);
    if (!temp.empty()) {
        p->birthday = temp;
        p->birthYear = Person::extractYear(temp); 
    }

    cout << "Nghe nghiep hien tai [" << p->job << "]: ";
    getline(cin, temp);
    if (!temp.empty()) p->job = temp;

    cout << "Ngay mat hien tai [" << p->deathDay << "]: ";
    getline(cin, temp);
    if (!temp.empty()) p->deathDay = temp;

    cout << "Ten vo/chong hien tai [" << p->spouseName << "]: ";
    getline(cin, temp);
    if (!temp.empty()) p->spouseName = temp;

    if (p->gender == "Nu") {
        cout << "So con hien tai [" << p->numChildren << "]: ";
        getline(cin, temp);
        if (!temp.empty()) {
            p->numChildren = stoi(temp);
        }
    }

    cout << "\n[OK] Cap nhat thanh cong!\n";
}

void FreeTree(Person* current) {
    if (current == nullptr) return;
    FreeTree(current->firstChild);
    FreeTree(current->nextSibling);
    delete current;
}

void ClearCurrentFamily() {
    if (root == nullptr) {
        cout << "[Thong bao] Gia pha hien dang trong, khong can xoa.\n";
        return;
    }

    FreeTree(root); 

    root = nullptr;
    global_id_counter = 1;

    cout << "[OK] Da xoa sach toan bo du lieu gia pha tren RAM!\n";
    cout << "ID he thong da duoc reset ve 1.\n";
}