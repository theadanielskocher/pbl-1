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
#define YELLOW   "\033[38;5;226m"

using namespace std;

struct Person {
    int id; 
    string name;
    string gender;          
    string birthday;        
    string job;             
    string deathDay;        
    string spouseName;      
    int numChildren;
	int birthYear;      

    Person* parent = nullptr;
    Person* firstChild = nullptr;
    Person* nextSibling = nullptr;

    Person(string _name, string _gender, int _birthYear, string _birthday, Person* _parent = nullptr);
    
    static int extractYear(string bday) {
        if (bday.length() < 4) return 0;
        try {
            return stoi(bday.substr(bday.length() - 4));
        } catch (...) { return 0; }
    }
};

struct SearchResult {
    Person* node;
    bool isSpouse;
};

struct PersonRecord {
    int id;
    int parentId;
    char name[50], gender[10], birthday[20], job[50], deathDay[20], spouseName[50];
    int numChildren, birthYear;
};

extern Person* root; 
extern int global_id_counter;

#endif