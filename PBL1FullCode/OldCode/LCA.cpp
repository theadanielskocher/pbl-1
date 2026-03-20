#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// --- 1. CẤU TRÚC DỮ LIỆU ---
struct Person {
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

// --- 2. CÁC HÀM QUẢN LÝ CÂY CƠ BẢN ---

Person* FindPerson(Person* current, string targetName) {
    if (current == nullptr) return nullptr;
    if (current->name == targetName) return current;
    Person* found = FindPerson(current->firstChild, targetName);
    if (found != nullptr) return found;
    return FindPerson(current->nextSibling, targetName);
}

void AddChild(Person*& root, string parentName, string childName, string gender) {
    if (root == nullptr) {
        root = new Person(childName, gender);
        return;
    }
    Person* p = (parentName == "") ? root : FindPerson(root, parentName);
    if (!p) return;

    if (p->gender == "Nu") {
        cout << "Loi: Khong the them con cho nguoi nu theo gia pha!\n";
        return;
    }

    Person* child = new Person(childName, gender, p);
    if (p->firstChild == nullptr) {
        p->firstChild = child;
    } else {
        Person* current = p->firstChild;
        while (current->nextSibling != nullptr) {
            current = current->nextSibling;
        }
        current->nextSibling = child;
    }
}

// --- 3. THUẬT TOÁN LCA & XÁC ĐỊNH QUAN HỆ ---

vector<Person*> GetPathToRoot(Person* p) {
    vector<Person*> path;
    while (p != nullptr) {
        path.push_back(p);
        p = p->parent;
    }
    reverse(path.begin(), path.end());
    return path;
}

string DetermineRelationship(Person* pA, Person* pB) {
    if (pA == nullptr || pB == nullptr) return "Loi: Nguoi khong ton tai!";
    if (pA == pB) return "Chinh la mot nguoi!";

    // Lấy đường đi từ gốc đến A và B
    vector<Person*> pathA = GetPathToRoot(pA);
    vector<Person*> pathB = GetPathToRoot(pB);

    int lcaIndex = -1;
    int minLen = min((int)pathA.size(), (int)pathB.size());
    for (int i = 0; i < minLen; i++) {
        if (pathA[i] == pathB[i]) {
            lcaIndex = i; 
        } else break;
    }

    if (lcaIndex == -1) return "Khong co quan he ho hang (khac cay)";

    int dA = (pathA.size() - 1) - lcaIndex;
    int dB = (pathB.size() - 1) - lcaIndex;
    Person* lca = pathA[lcaIndex];

    // --- 1. Cùng thế hệ ---
    if (dA == dB) {
        if (dA == 1) { // Anh / Chị / Em ruột
            bool aIsOlder = false;
            Person* curr = lca->firstChild;
            while (curr != nullptr) {
                if (curr == pA) { aIsOlder = true; break; }
                if (curr == pB) { aIsOlder = false; break; }
                curr = curr->nextSibling;
            }
            if (aIsOlder) return (pA->gender == "Nam") ? "Anh trai" : "Chi gai";
            else return (pA->gender == "Nam") ? "Em trai" : "Em gai";
        }
        return "Anh/Chi/Em ho (" + to_string(dA - 1) + " doi)";
    }

    // --- 2. Quan hệ Trực hệ ---
    if (dA == 0) { // A là tổ tiên của B
        if (dB == 1) return (pA->gender == "Nam") ? "Cha" : "Me";
        if (dB == 2) return (pA->gender == "Nam") ? "Ong" : "Ba";
        return "To tien (doi thu " + to_string(dB) + ")";
    }
    if (dB == 0) { // B là tổ tiên của A
        if (dA == 1) return "Con";
        if (dA == 2) return "Chau";
        return "Hau due (doi thu " + to_string(dA) + ")";
    }

    // --- 3. Quan hệ Bàng hệ (Cô, Chú, Bác...) ---
    if (dA == 1 && dB == 2) {
        // A là anh/chị/em của Cha/Mẹ B
        Person* aNode = pathA[lcaIndex + 1];     // Chính là A
        Person* parentOfB = pathB[lcaIndex + 1]; // Cha/mẹ của B
        
        bool aIsOlder = false;
        Person* curr = lca->firstChild;
        while (curr != nullptr) {
            if (curr == aNode) { aIsOlder = true; break; }
            if (curr == parentOfB) { aIsOlder = false; break; }
            curr = curr->nextSibling;
        }

        if (aIsOlder) {
            return (pA->gender == "Nam") ? "Bac trai" : "Bac gai";
        } else {
            // Theo gia phả nội tộc, B là con của cha (Nam), nên em của cha gọi là Chú/Cô
            return (pA->gender == "Nam") ? "Chu" : "Co"; 
        }
    }

    if (dA == 2 && dB == 1) {
        // B là anh/chị/em của Cha/Mẹ A (A là cháu)
        Person* parentOfA = pathA[lcaIndex + 1]; // Cha/mẹ của A
        Person* bNode = pathB[lcaIndex + 1];     // Chính là B
        
        bool bIsOlder = false;
        Person* curr = lca->firstChild;
        while (curr != nullptr) {
            if (curr == bNode) { bIsOlder = true; break; }
            if (curr == parentOfA) { bIsOlder = false; break; }
            curr = curr->nextSibling;
        }

        if (bIsOlder) {
            return "Chau (goi bang Bac)";
        } else {
            return (pB->gender == "Nam") ? "Chau (goi bang Chu)" : "Chau (goi bang Co)";
        }
    }

    // --- 4. Các trường hợp khác ---
    return "Ho hang xa (Cach nhau " + to_string(dA + dB) + " bac)";
}

// --- 4. HÀM HIỂN THỊ & GIẢI PHÓNG ---

void DisplayTree(Person* current, int level) {
    if (current == nullptr) return;
    for (int i = 0; i < level; i++) cout << "    ";
    cout << "|-- " << current->name << " (" << current->gender << ")" << endl;
    DisplayTree(current->firstChild, level + 1);
    DisplayTree(current->nextSibling, level);
}

void FreeTree(Person* current) {
    if (current == nullptr) return;
    FreeTree(current->firstChild);
    FreeTree(current->nextSibling);
    delete current;
}

// --- 5. HÀM MAIN (KIỂM THỬ) ---

int main() {
    Person* root = nullptr;

    // Khởi tạo gia phả mẫu
    AddChild(root, "", "Nguyen Van To", "Nam"); // Ong To
    AddChild(root, "Nguyen Van To", "Nguyen Van Hung", "Nam");
    AddChild(root, "Nguyen Van To", "Nguyen Thi Lan", "Nu");
    AddChild(root, "Nguyen Van Hung", "Nguyen Van Minh", "Nam"); // Chau noi
    AddChild(root, "Nguyen Thi Lan", "Le Thi Hoa", "Nu"); // Chau ngoai (Luu y logic AddChild file pointer.cpp xet gioi tinh cha)

    cout << "--- SO DO GIA PHA ---\n";
    DisplayTree(root, 0);

    // Test quan hệ
    cout << "\n--- KIEM TRA QUAN HE ---\n";
    Person* p1 = FindPerson(root, "Nguyen Van To");
    Person* p2 = FindPerson(root, "Nguyen Van Minh");
    Person* p3 = FindPerson(root, "Nguyen Thi Lan");
    Person* p4 = FindPerson(root, "Nguyen Van Hung");

    if (p1 && p2) cout << p1->name << " va " << p2->name << ": " << DetermineRelationship(p1, p2) << endl;
    if (p3 && p4) cout << p3->name << " va " << p4->name << ": " << DetermineRelationship(p3, p4) << endl;
    if (p4 && p2) cout << p4->name << " va " << p2->name << ": " << DetermineRelationship(p4, p2) << endl;

    FreeTree(root);
    return 0;
}