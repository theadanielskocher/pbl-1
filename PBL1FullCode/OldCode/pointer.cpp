#include <iostream>
#include <string>
#include <vector>

using namespace std;

// --- BIẾN TOÀN CỤC (Đồng bộ với hệ thống chính) ---
int global_id_counter = 1;

// --- CẤU TRÚC DỮ LIỆU ---
struct Person {
    int id; // ID định danh duy nhất
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

// Con trỏ gốc của gia phả
Person* root = nullptr;

// --- CÁC HÀM QUẢN LÝ CÂY CƠ BẢN ---
Person* CreateFamily(string name, string gender) {
    if (root != nullptr) {
        cout << "Gia pha da ton tai!\n";
        return root;
    }
    root = new Person(name, gender);
    cout << "Da khoi tao gia pha voi ong to: [" << root->id << "] " << name << endl;
    return root;
}

Person* AddChild(Person* parent, string name, string gender) {
    if (parent == nullptr) {
        cout << "Loi: Cha khong ton tai!\n";
        return nullptr;
    }
    if (parent->gender == "Nu") {
        cout << "Loi: Khong the them con cho nguoi nu theo quy tac!\n";
        return nullptr;
    }
    if (gender != "Nam" && gender != "Nu") {
        cout << "Loi: Gioi tinh khong hop le!\n";
        return nullptr;
    }

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
    cout << "Da them [" << child->id << "] " << name << " lam con cua [" << parent->id << "] " << parent->name << endl;
    return child;
}

void DisplayTree(Person* current, int level) {
    if (current == nullptr) return;

    for (int i = 0; i < level; i++) cout << "    ";
    cout << "|-- [" << current->id << "] " << current->name << " (" << current->gender << ")" << endl;

    DisplayTree(current->firstChild, level + 1);
    DisplayTree(current->nextSibling, level);
}

void FreeTree(Person* current) {
    if (current == nullptr) return;
    FreeTree(current->firstChild);
    FreeTree(current->nextSibling);
    delete current;
}

// --- HỆ THỐNG TÌM KIẾM CHỐNG TRÙNG TÊN ---
void FindAllPersonsByName(Person* current, string targetName, vector<Person*>& results) {
    if (current == nullptr) return;
    
    if (current->name == targetName) {
        results.push_back(current);
    }
    
    FindAllPersonsByName(current->firstChild, targetName, results);
    FindAllPersonsByName(current->nextSibling, targetName, results);
}

Person* SelectPersonFromName(Person* rootNode, string targetName) {
    vector<Person*> results;
    FindAllPersonsByName(rootNode, targetName, results);

    if (results.empty()) {
        cout << "\n[Loi] Khong tim thay ai co ten la '" << targetName << "'!\n";
        return nullptr;
    }

    if (results.size() == 1) {
        return results[0]; 
    }

    cout << "\n[Chu y] Tim thay " << results.size() << " nguoi cung ten '" << targetName << "':\n";
    cout << "------------------------------------------------------\n";
    for (size_t i = 0; i < results.size(); i++) {
        cout << i + 1 << ". ID: " << results[i]->id 
             << " | Gioi: " << results[i]->gender;
        if (results[i]->parent != nullptr) {
            cout << " | Con cua: " << results[i]->parent->name;
        } else {
            cout << " | (La Cu To)";
        }
        cout << " | NS: " << results[i]->birthday << endl;
    }
    cout << "------------------------------------------------------\n";

    int choice = 0;
    while (true) {
        cout << "Nhap so thu tu (1 - " << results.size() << ") de chon: ";
        cin >> choice;
        cin.ignore(256, '\n'); 

        if (choice >= 1 && choice <= results.size()) {
            return results[choice - 1];
        } else {
            cout << "Lua chon khong hop le!\n";
        }
    }
}

// --- HÀM CẬP NHẬT VÀ HIỂN THỊ CHI TIẾT ---
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

void ShowDetail(Person* p) {
    if (p == nullptr) return;
    cout << "\n[ THONG TIN CHI TIET ]\n";
    cout << "ID: " << p->id << " | Ho ten: " << p->name << " (" << p->gender << ")\n";
    cout << "Ngay sinh: " << p->birthday << " | Nghe nghiep: " << p->job << endl;
    cout << "Tinh trang: " << p->deathDay << " | Vo/Chong: " << p->spouseName << endl;
    if (p->gender == "Nu") cout << "So con: " << p->numChildren << endl;
}

// --- CHƯƠNG TRÌNH CHÍNH (TEST) ---
int main() {
    CreateFamily("Nguyen Van To", "Nam");
    Person* hung = AddChild(root, "Nguyen Van Hung", "Nam");
    AddChild(root, "Nguyen Thi Lan", "Nu");
    
    // Cố tình tạo thêm một người trùng tên để test giao diện chọn lọc
    AddChild(hung, "Nguyen Van Minh", "Nam");
    AddChild(root, "Nguyen Van Minh", "Nam"); 

    cout << "\n--- TRUOC KHI CAP NHAT ---";
    cout << "\n";
    DisplayTree(root, 0);

    // Kịch bản test: Tìm người trùng tên và cập nhật
    string nameToUpdate = "Nguyen Van Hung";
    cout << "\nDang tim kiem de cap nhat: " << nameToUpdate << "...\n";
    
    Person* p = SelectPersonFromName(root, nameToUpdate);
    
    if (p != nullptr) {
        ShowDetail(p);
//       UpdatePersonInfo(p);
        
        cout << "\n--- SAU KHI CAP NHAT ---";
        ShowDetail(p);
    }

    FreeTree(root);
    root = nullptr;
    return 0;
}