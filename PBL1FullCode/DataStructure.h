#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

#include <iostream>
#include <string>
#include <vector>

#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define RED     "\033[31m"      
#define GREEN   "\033[32m"      
#define DUT_BLUE  "\033[1;34m"
#define ORANGE  "\033[38;5;208m"
#define CYAN    "\033[36m"
#define HIGHLIGHT "\033[1;37;44m"

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

struct SearchResult {
    Person* node;    // Con trỏ tới nút tìm thấy (hoặc nút chồng nếu là vợ)
    bool isSpouse;   // true nếu tên khớp với spouseName, false nếu khớp với name
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