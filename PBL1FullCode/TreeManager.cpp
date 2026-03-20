#include "TreeManager.h"  // 1. Luôn include file .h của chính nó
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

// Trong TreeManager.cpp
#include "TreeManager.h"

// Định nghĩa hàm khởi tạo của struct Person
Person::Person(string _name, string _gender, Person* _parent) {
    id = global_id_counter++;
    name = _name;
    gender = _gender;
    parent = _parent;
    birthday = "Unknown";
    job = "Unknown";
    deathDay = "N/A";
    spouseName = "None";
    numChildren = 0;
    firstChild = nullptr;
    nextSibling = nullptr;
}


Person* CreateFamily(string name, string gender) {
    if (root != nullptr) return root;
    root = new Person(name, gender);
    return root;
}

Person* AddChild(Person* parent, string name, string gender) {
    if (parent == nullptr || parent->gender == "Nu") return nullptr;
    Person* child = new Person(name, gender, parent);
    if (parent->firstChild == nullptr) {
        parent->firstChild = child;
    } else {
        Person* current = parent->firstChild;
        while (current->nextSibling != nullptr) {
            current = current->nextSibling;
        }
        current->nextSibling = child;
    }
    return child;
}

void DisplayTree(Person* current, int level) {
    if (current == nullptr) return;
    for (int i = 0; i < level; i++) cout << "    ";
    cout << "|-- [" << current->id << "] " << current->name << " (" << current->gender << ")" << endl;
    DisplayTree(current->firstChild, level + 1);
    DisplayTree(current->nextSibling, level);
}

void ShowDetail(Person* p) {
    if (p == nullptr) return;
    cout << "\n[ THONG TIN CHI TIET ]\n";
    cout << "ID: " << p->id << " | Ho ten: " << p->name << " (" << p->gender << ")\n";
    cout << "Ngay sinh: " << p->birthday << " | Nghe nghiep: " << p->job << endl;
    cout << "Tinh trang: " << p->deathDay << " | Vo/Chong: " << p->spouseName << endl;
}

void UpdatePersonInfo(Person* p) {
    if (p == nullptr) return;

    cout << "\n--- Cap nhat thong tin cho: [" << p->id << "] " << p->name << " ---\n";
    cout << "Nhap ngay sinh: "; getline(cin, p->birthday);
    cout << "Nhap nghe nghiep: "; getline(cin, p->job);
    cout << "Nhap ngay mat (N/A neu con song): "; getline(cin, p->deathDay);
    cout << "Nhap ten vo/chong: "; getline(cin, p->spouseName);

    if (p->gender == "Nu") {
        cout << "Nhap so con: "; cin >> p->numChildren;
        cin.ignore(256, '\n');
    }
    cout << "Cap nhat thanh cong!\n";
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

    // 1. Giai phong de quy tat ca cac nut tren RAM
    FreeTree(root); 

    // 2. Reset cac bien dieu khien ve trang thai ban dau
    root = nullptr;
    global_id_counter = 1;

    cout << "[OK] Da xoa sach toan bo du lieu gia pha tren RAM!\n";
    cout << "ID he thong da duoc reset ve 1.\n";
}