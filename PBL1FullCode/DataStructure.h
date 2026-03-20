#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

// ======================================================================
// 1. CẤU TRÚC DỮ LIỆU ĐỐI TƯỢNG (Dùng trên RAM)
// ======================================================================
struct Person {
    int id; 
    string name;
    string gender;          
    string birthday;        
    string job;             
    string deathDay;        
    string spouseName;      
    int numChildren;        

    Person* parent;
    Person* firstChild;
    Person* nextSibling;

    Person(string _name, string _gender, Person* _parent = nullptr);
};

// ======================================================================
// 2. CẤU TRÚC BẢN GHI TĨNH (Dùng để lưu/đọc file nhị phân .dat)
// ======================================================================
struct PersonRecord {
    int id;
    int parentId;
    char name[50], gender[10], birthday[20], job[50], deathDay[20], spouseName[50];
    int numChildren;
};

// ======================================================================
// 3. KHAI BÁO BIẾN TOÀN CỤC (Dùng từ khóa extern)
// ======================================================================
// extern báo cho compiler rằng các biến này được định nghĩa ở file .cpp khác
extern Person* root; 
extern int global_id_counter;

#endif