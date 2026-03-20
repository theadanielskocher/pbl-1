#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstring>
#include <sstream>
#include <unordered_map> // Thu vien cho Banh bam (Hash Map) O(1)

using namespace std;

// Bien toan cuc de tu dong sinh ID duy nhat cho moi thanh vien
int global_id_counter = 1;

// --- CAC HAM HO TRO (UTILITIES) ---
// Ham xoa khoang trang thua o hai dau chuoi (Trim) - Rat quan trong cho file Text
string trim(const string& str) {
    size_t first = str.find_first_not_of(' ');
    if (string::npos == first) return str;
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

// --- CAU TRUC DU LIEU TREN RAM ---
struct Person {
    int id; // ID dinh danh duy nhat
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

    Person(string _name, string _gender, Person* _parent = nullptr) {
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
};

Person* root = nullptr;

// Tao thuy to, tra ve con tro
Person* CreateFamily(string name, string gender) {
    if (root != nullptr) return root;
    root = new Person(name, gender);
    return root;
}

// Them con, tra ve con tro cua con vua tao de gán du lieu ngay lap tuc
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

// In gia pha hien thi them ID
void DisplayTree(Person* current, int level) {
    if (current == nullptr) return;
    for (int i = 0; i < level; i++) cout << "    ";
    cout << "|-- [" << current->id << "] " << current->name << " (" << current->gender << ")" << endl;
    DisplayTree(current->firstChild, level + 1);
    DisplayTree(current->nextSibling, level);
}

// Giai phong bo nho
void FreeTree(Person* current) {
    if (current == nullptr) return;
    FreeTree(current->firstChild);
    FreeTree(current->nextSibling);
    delete current;
}

// Tim kiem theo ten (Dung de ho tro import tu file Text)
Person* FindPersonByName(Person* current, string targetName) {
    if (current == nullptr) return nullptr;
    if (current->name == targetName) return current;
    Person* found = FindPersonByName(current->firstChild, targetName);
    if (found != nullptr) return found;
    return FindPersonByName(current->nextSibling, targetName);
}

// --- CAU TRUC DU LIEU LUU FILE NHI PHAN ---
struct PersonRecord {
    int id;
    int parentId; // Dung ID de chong trung ten thay vi dung char parentName[50]
    char name[50];
    char gender[10];
    char birthday[20];
    char job[50];
    char deathDay[20];
    char spouseName[50];
    int numChildren;
};

// --- CAC HAM XU LY FILE NHI PHAN (.DAT) ---
void FlattenTree(Person* current, vector<PersonRecord>& listRecords) {
    if (current == nullptr) return;

    PersonRecord rec;
    rec.id = current->id;
    rec.parentId = (current->parent != nullptr) ? current->parent->id : 0;
    
    strncpy(rec.name, current->name.c_str(), sizeof(rec.name) - 1); rec.name[sizeof(rec.name) - 1] = '\0';
    strncpy(rec.gender, current->gender.c_str(), sizeof(rec.gender) - 1); rec.gender[sizeof(rec.gender) - 1] = '\0';
    strncpy(rec.birthday, current->birthday.c_str(), sizeof(rec.birthday) - 1); rec.birthday[sizeof(rec.birthday) - 1] = '\0';
    strncpy(rec.job, current->job.c_str(), sizeof(rec.job) - 1); rec.job[sizeof(rec.job) - 1] = '\0';
    strncpy(rec.deathDay, current->deathDay.c_str(), sizeof(rec.deathDay) - 1); rec.deathDay[sizeof(rec.deathDay) - 1] = '\0';
    strncpy(rec.spouseName, current->spouseName.c_str(), sizeof(rec.spouseName) - 1); rec.spouseName[sizeof(rec.spouseName) - 1] = '\0';
    rec.numChildren = current->numChildren;

    listRecords.push_back(rec);
    FlattenTree(current->firstChild, listRecords);
    FlattenTree(current->nextSibling, listRecords);
}

void SaveTreeToFile(Person* rootNode, string filename) {
    if (rootNode == nullptr) {
        cout << "Gia pha trong, khong co gi de luu!\n";
        return;
    }
    vector<PersonRecord> listRecords;
    FlattenTree(rootNode, listRecords);

    ofstream outFile(filename, ios::binary);
    if (outFile.is_open()) {
        int totalNodes = listRecords.size();
        outFile.write((char*)&totalNodes, sizeof(int));
        outFile.write((char*)listRecords.data(), totalNodes * sizeof(PersonRecord));
        outFile.close();
        cout << "\n[OK] Da luu " << totalNodes << " nguoi vao file " << filename << "!\n";
    }
}

// Toi uu toc do Load file voi do phuc tap O(N) dung Hash Map
void LoadTreeFromFile(string filename) {
    ifstream inFile(filename, ios::binary);
    if (!inFile.is_open()) {
        cout << "\n[Loi] Khong tim thay file de doc!\n";
        return;
    }

    if (root != nullptr) {
        FreeTree(root);
        root = nullptr;
    }

    int totalNodes = 0;
    inFile.read((char*)&totalNodes, sizeof(int));
    if (totalNodes <= 0) return;

    PersonRecord* listRecords = new PersonRecord[totalNodes];
    inFile.read((char*)listRecords, totalNodes * sizeof(PersonRecord));
    inFile.close();

    unordered_map<int, Person*> nodeMap;
    int maxIdInFile = 0;

    for (int i = 0; i < totalNodes; i++) {
        Person* newlyAdded = nullptr;

        if (listRecords[i].parentId == 0) {
            newlyAdded = CreateFamily(listRecords[i].name, listRecords[i].gender);
        } else {
            Person* parentNode = nodeMap[listRecords[i].parentId];
            if (parentNode != nullptr) {
                newlyAdded = AddChild(parentNode, listRecords[i].name, listRecords[i].gender);
            }
        }

        if (newlyAdded != nullptr) {
            newlyAdded->id = listRecords[i].id;
            if (newlyAdded->id > maxIdInFile) maxIdInFile = newlyAdded->id;

            newlyAdded->birthday = listRecords[i].birthday;
            newlyAdded->job = listRecords[i].job;
            newlyAdded->deathDay = listRecords[i].deathDay;
            newlyAdded->spouseName = listRecords[i].spouseName;
            newlyAdded->numChildren = listRecords[i].numChildren;

            nodeMap[newlyAdded->id] = newlyAdded;
        }
    }
    global_id_counter = maxIdInFile + 1;
    delete[] listRecords;
    cout << "\n[OK] Da tai thanh cong " << totalNodes << " nguoi tu file nhi phan!\n";
}

// --- HAM XU LY FILE TEXT (NHAP LIEU HANG LOAT) ---
void ImportFromTextFile(string filename) {
    ifstream inFile(filename);
    if (!inFile.is_open()) {
        cout << "[Loi] Khong the mo file " << filename << ". Hay kiem tra lai ten file!\n";
        return;
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
            Person* cha = FindPersonByName(root, tenCha);
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

// --- CHUONG TRINH CHINH ---
int main() {
    string binaryFile = "GiaPha.dat";
    int luaChon;

    do {
        cout << "\n=========================================\n";
        cout << "   CHUONG TRINH QUAN LY GIA PHA PBL 1    \n";
        cout << "=========================================\n";
        cout << "1. Hien thi cay gia pha\n";
        cout << "2. Luu gia pha xuong file (.dat)\n";
        cout << "3. Mo gia pha tu file (.dat)\n";
        cout << "4. Nhap lieu hang loat tu file (.txt)\n";
        cout << "0. Thoat chuong trinh\n";
        cout << "-----------------------------------------\n";
        cout << "Nhap lua chon cua ban: ";
        cin >> luaChon;
        cin.ignore(256, '\n'); 

        switch (luaChon) {
            case 1:
                cout << "\n--- CAY GIA PHA HIEN TAI ---\n";
                if (root == nullptr) cout << "Gia pha dang trong!\n";
                else DisplayTree(root, 0);
                break;
            case 2:
                SaveTreeToFile(root, binaryFile);
                break;
            case 3:
                LoadTreeFromFile(binaryFile);
                break;
            case 4: {
                string textFile;
                cout << "Nhap ten file text (Dinh dang: TenCha, TenCon, GioiTinh)\n";
                cout << "VD: None, Nguyen Van A, Nam\n";
                cout << "Ten file (VD: NhapLieu.txt): ";
                getline(cin, textFile);
                ImportFromTextFile(textFile);
                break;
            }
            case 0:
                cout << "Dang don dep bo nho... Tam biet!\n";
                FreeTree(root);
                break;
            default:
                cout << "Lua chon khong hop le!\n";
                break;
        }
    } while (luaChon != 0);

    return 0;
}