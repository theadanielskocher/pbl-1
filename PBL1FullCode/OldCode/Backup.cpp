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

// ======================================================================
// 1. CẤU TRÚC DỮ LIỆU & BIẾN TOÀN CỤC CHUẨN HÓA
// ======================================================================

int global_id_counter = 1;

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

// ======================================================================
// 2. CÁC HÀM QUẢN LÝ CÂY (TỪ POINTER.CPP)
// ======================================================================

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

void FreeTree(Person* current) {
    if (current == nullptr) return;
    FreeTree(current->firstChild);
    FreeTree(current->nextSibling);
    delete current;
}

// ======================================================================
// 3. HỆ THỐNG TÌM KIẾM & XỬ LÝ TRÙNG TÊN (TỪ POINTER.CPP)
// ======================================================================

void FindAllPersonsByName(Person* current, string targetName, vector<Person*>& results) {
    if (current == nullptr) return;
    if (current->name == targetName) results.push_back(current);
    FindAllPersonsByName(current->firstChild, targetName, results);
    FindAllPersonsByName(current->nextSibling, targetName, results);
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

Person* SelectPersonFromName(string targetName) {
    vector<Person*> results;
    FindAllPersonsByName(root, targetName, results);

    if (results.empty()) {
        cout << "\n[Loi] Khong tim thay ai co ten la '" << targetName << "'!\n";
        return nullptr;
    }

    if (results.size() == 1) return results[0]; 

    cout << "\n[Chu y] Tim thay " << results.size() << " nguoi cung ten '" << targetName << "':\n";
    cout << "------------------------------------------------------\n";
    for (size_t i = 0; i < results.size(); i++) {
        cout << i + 1 << ". ID: " << results[i]->id << " | Gioi: " << results[i]->gender;
        if (results[i]->parent != nullptr) cout << " | Con cua: " << results[i]->parent->name;
        else cout << " | (La Ong To)";
        cout << endl;
    }
    cout << "------------------------------------------------------\n";

    int choice = 0;
    while (true) {
        cout << "Nhap so thu tu (1 - " << results.size() << ") de chon: ";
        cin >> choice; cin.ignore(256, '\n'); 
        if (choice >= 1 && choice <= results.size()) return results[choice - 1];
        cout << "Lua chon khong hop le!\n";
    }
}

void ShowDetail(Person* p) {
    if (p == nullptr) return;
    cout << "\n[ THONG TIN CHI TIET ]\n";
    cout << "ID: " << p->id << " | Ho ten: " << p->name << " (" << p->gender << ")\n";
    cout << "Ngay sinh: " << p->birthday << " | Nghe nghiep: " << p->job << endl;
    cout << "Tinh trang: " << p->deathDay << " | Vo/Chong: " << p->spouseName << endl;
}

// ======================================================================
// 4. THUẬT TOÁN LCA & XÁC ĐỊNH QUAN HỆ (TỪ LCA.CPP)
// ======================================================================

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

// ======================================================================
// 4B. TÌM NGƯỜI THEO QUAN HỆ
// ======================================================================

// Pha 1: Leo ngược lên dA bậc thế hệ để tìm nút Tổ tiên tham chiếu
Person* GetAncestor(Person* start, int dA) {
    Person* curr = start;
    for (int i = 0; i < dA; i++) {
        if (curr != nullptr) curr = curr->parent;
    }
    return curr;
}

// Pha 2: Quét đệ quy xuống đúng dB bậc thế hệ để gom tất cả ứng viên
void CollectAtDepth(Person* root, int targetDepth, int currentDepth, vector<Person*>& results) {
    if (root == nullptr) return;
    
    // Nếu đã xuống đúng số đời cần tìm, đưa vào danh sách rồi dừng nhánh này
    if (currentDepth == targetDepth) {
        results.push_back(root);
        return; 
    }
    
    // Đi xuống đời con (tăng depth) và quét toàn bộ anh em ngang hàng
    Person* child = root->firstChild;
    while (child != nullptr) {
        CollectAtDepth(child, targetDepth, currentDepth + 1, results);
        child = child->nextSibling;
    }
}

// Pha 3: Hàm tìm kiếm chính kết hợp phân giải danh xưng và lọc giới tính/thứ tự
vector<Person*> FindRelativesByRelationship(Person* startPerson, string relationName) {
    vector<Person*> finalResults;
    if (startPerson == nullptr) return finalResults;

    int dA = -1, dB = -1;
    
    // BẢNG MÃ HÓA TỌA ĐỘ TỪ DANH XƯNG
    if (relationName == "Cha" || relationName == "Me") { dA = 1; dB = 0; }
    else if (relationName == "Ong" || relationName == "Ba") { dA = 2; dB = 0; }
    else if (relationName == "Anh trai" || relationName == "Chi gai" || 
             relationName == "Em trai" || relationName == "Em gai") { dA = 1; dB = 1; }
    else if (relationName == "Bac trai" || relationName == "Bac gai" || 
             relationName == "Chu" || relationName == "Co" || relationName == "Di") { dA = 2; dB = 1; }
    else if (relationName == "Anh/Chi/Em ho") { dA = 2; dB = 2; }
    else if (relationName == "Con") { dA = 0; dB = 1; }
    else if (relationName == "Chau") { dA = 0; dB = 2; }
    else {
        cout << "[Loi] Chua ho tro truy van danh xung: " << relationName << "\n";
        return finalResults;
    }

    // BƯỚC 1: TÌM TỔ TIÊN CHUNG
    Person* ancestor = GetAncestor(startPerson, dA);
    if (ancestor == nullptr) return finalResults; // Không đủ thế hệ (VD: Gốc tìm Cha)

    // BƯỚC 2: THU THẬP ỨNG VIÊN
    vector<Person*> candidates;
    if (dB == 0) {
        candidates.push_back(ancestor);
    } else {
        Person* child = ancestor->firstChild;
        while (child != nullptr) {
            CollectAtDepth(child, dB, 1, candidates);
            child = child->nextSibling;
        }
    }

    // BƯỚC 3: LỌC KẾT QUẢ DỰA TRÊN NGHIỆP VỤ (Giới tính, tuổi tác)
    for (Person* p : candidates) {
        if (p == startPerson) continue; // Loại bỏ chính mình
        bool match = false;
        
        // 3.1. Lọc trực hệ & Con cháu
        if (relationName == "Cha" || relationName == "Ong") { if (p->gender == "Nam") match = true; }
        else if (relationName == "Me" || relationName == "Ba") { if (p->gender == "Nu") match = true; }
        else if (relationName == "Con" || relationName == "Chau" || relationName == "Anh/Chi/Em ho") { match = true; }
        
        // 3.2. Lọc Anh/Chị/Em ruột (So sánh sinh trước/sau so với startPerson)
        else if (dA == 1 && dB == 1) {
            bool isOlder = false;
            Person* curr = ancestor->firstChild;
            while (curr != nullptr) {
                if (curr == p) { isOlder = true; break; }
                if (curr == startPerson) { isOlder = false; break; }
                curr = curr->nextSibling;
            }
            if (relationName == "Anh trai" && isOlder && p->gender == "Nam") match = true;
            if (relationName == "Chi gai" && isOlder && p->gender == "Nu") match = true;
            if (relationName == "Em trai" && !isOlder && p->gender == "Nam") match = true;
            if (relationName == "Em gai" && !isOlder && p->gender == "Nu") match = true;
        }
        
        // 3.3. Lọc Bác/Chú/Cô (So sánh sinh trước/sau so với Cha của startPerson)
        else if (dA == 2 && dB == 1) {
            Person* parentOfStart = startPerson->parent;
            if (p == parentOfStart) continue;
            
            bool isOlderThanParent = false;
            Person* curr = ancestor->firstChild;
            while (curr != nullptr) {
                if (curr == p) { isOlderThanParent = true; break; }
                if (curr == parentOfStart) { isOlderThanParent = false; break; }
                curr = curr->nextSibling;
            }
            if (relationName == "Bac trai" && isOlderThanParent && p->gender == "Nam") match = true;
            if (relationName == "Bac gai" && isOlderThanParent && p->gender == "Nu") match = true;
            if (relationName == "Chu" && !isOlderThanParent && p->gender == "Nam") match = true;
            if (relationName == "Co" && !isOlderThanParent && p->gender == "Nu") match = true;
            if (relationName == "Di" && !isOlderThanParent && p->gender == "Nu") match = true;
        }
        
        else if (relationName == "Anh/Chi/Em ho" && dA == 2 && dB == 2) {
            Person* parentOfStart = startPerson->parent;
            
            if (p->parent == parentOfStart) continue; 
            
            match = true;
        }

        if (match) finalResults.push_back(p);
    }

    return finalResults;
}


// ======================================================================
// 5. XỬ LÝ FILE NHỊ PHÂN (TỪ INOUT2.CPP)
// ======================================================================

struct PersonRecord {
    int id;
    int parentId;
    char name[50], gender[10], birthday[20], job[50], deathDay[20], spouseName[50];
    int numChildren;
};

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

// XOÁ SẠCH DỮ LIỆU GIA PHẢ

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

// Hàm dùng riêng cho việc Import file .txt
Person* FindFirstMatchForImport(Person* rootNode, string targetName) {
    vector<Person*> results;
    FindAllPersonsByName(rootNode, targetName, results);

    if (results.empty()) return nullptr;

    if (results.size() > 1) {
        // Cảnh báo nhưng không dừng chương trình
        cout << "[Canh bao Import] Tim thay " << results.size() 
             << " nguoi ten '" << targetName 
             << "'. Dang tu dong noi vao ID: " << results[0]->id << endl;
    }

    return results[0]; // Trả về người đầu tiên tìm thấy
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

// ======================================================================
// 6. KHỞI TẠO DỮ LIỆU CỐ ĐỊNH (HARDCODED DATA)
// ======================================================================

void InitFixedData() {
    cout << "[He thong] Dang khoi tao bo du lieu gia pha mau...\n";
    Person* to = CreateFamily("Nguyen Van To", "Nam");
    
    Person* hung = AddChild(to, "Nguyen Van Hung", "Nam");
    Person* lan = AddChild(to, "Nguyen Thi Lan", "Nu");
    Person* dung = AddChild(to, "Nguyen Van Dung", "Nam");
    
    AddChild(hung, "Nguyen Van Minh", "Nam");
    AddChild(hung, "Nguyen Thi Hoa", "Nu");
    
    AddChild(dung, "Nguyen Van Kien", "Nam");
    // Tạo 1 người trùng tên để test hàm SelectPersonFromName
    AddChild(dung, "Nguyen Van Minh", "Nam"); 
}

// ======================================================================
// 7. MENU ĐIỀU KHIỂN CHÍNH
// ======================================================================

int main() {
    InitFixedData(); // Khởi tạo dữ liệu cứng ngay khi chạy
    system("color F0"); 

    string binaryFile = "GiaPha_Test.dat";
    int luaChon;

    do {
        cout << "\n=========================================\n";
        cout << "   CHUONG TRINH QUAN LY GIA PHA PBL 1    \n";
        cout << "=========================================\n";
        cout << "1. Hien thi cay gia pha\n";
        cout << "2. Tim kiem & Xem chi tiet\n";
        cout << "3. Kiem tra quan he ho hang\n";
        cout << "4. Tim nguoi theo quan he\n";
        cout << "5. Luu du lieu xuong file\n";
        cout << "6. Cap nhat thong tin chi tiet\n";
        cout << "7. Nhap lieu hang loat tu file\n";
        cout << "0. Thoat chuong trinh\n";
        cout << "-----------------------------------------\n";
        cout << "Nhap lua chon cua ban: ";
        if (!(cin >> luaChon)) {      // NẾU NGƯỜI DÙNG NHẬP CHỮ VÀO MENU
            cin.clear();              // 1. Xóa cờ báo lỗi của cin
            cin.ignore(256, '\n');    // 2. Dọn sạch rác trong bộ đệm
            luaChon = -1;             // 3. Đưa về số âm để menu hiển thị lại
        } else {
            cin.ignore(256, '\n');    // Nếu nhập số đúng thì chỉ dọn phím Enter
        }
        switch (luaChon) {
            case 1:
                cout << "\n--- CAY GIA PHA HIEN TAI ---\n";
                DisplayTree(root, 0);
                break;
            case 2: {
                string ten;
                cout << "Nhap ten can xem chi tiet (Thu nhap 'Nguyen Van Minh' de test trung ten): ";
                getline(cin, ten);
                Person* p = SelectPersonFromName(ten);
                ShowDetail(p);
                break;
            }
            case 3: {
                string ten1, ten2;
                cout << "Nhap ten nguoi thu nhat: "; getline(cin, ten1);
                Person* p1 = SelectPersonFromName(ten1);
                if (!p1) break;
                
                cout << "Nhap ten nguoi thu hai: "; getline(cin, ten2);
                Person* p2 = SelectPersonFromName(ten2);
                if (!p2) break;

                cout << "\n=> KET QUA LCA: " << p1->name << " la " << DetermineRelationship(p1, p2) << " cua " << p2->name << endl;
                     
                break;
            }
            
            case 4: {
				string tenNguoi;
			    cout << "\n--- TIM NGUOI THEO QUAN HE ---\n";
			    cout << "Nhap ten nguoi lam moc: ";
			    getline(cin, tenNguoi);
			    
			    // Gọi hàm tìm kiếm chống trùng tên
			    Person* moc = SelectPersonFromName(tenNguoi); 
			    
			    if (moc == nullptr) break;
			
			    // 1. In Menu Danh xưng dạng bảng để dễ nhìn
			    cout << "\n================ CHON QUAN HE CAN TIM ================\n";
			    cout << "  1. Cha           2. Me          3. Ong         4. Ba\n";
			    cout << "  5. Anh trai      6. Chi gai     7. Em trai     8. Em gai\n";
			    cout << "  9. Bac trai     10. Bac gai    11. Chu        12. Co        13. Di\n";
			    cout << " 14. Anh/Chi/Em ho               15. Con        16. Chau\n";
			    cout << "======================================================\n";
			    
			    // 2. Vòng lặp bắt lỗi nhập liệu
			    int luaChon;
			    string quanHe = "";
			    
			    while (true) {
			        cout << "Nhap lua chon (1-16): ";
			        cin >> luaChon;
			        cin.ignore(256, '\n'); // Xóa bộ đệm
			        
			        // 3. Ánh xạ từ Số sang Chuỗi chuẩn xác 100%
			        switch (luaChon) {
			            case 1: quanHe = "Cha"; break;
			            case 2: quanHe = "Me"; break;
			            case 3: quanHe = "Ong"; break;
			            case 4: quanHe = "Ba"; break;
			            case 5: quanHe = "Anh trai"; break;
			            case 6: quanHe = "Chi gai"; break;
			            case 7: quanHe = "Em trai"; break;
			            case 8: quanHe = "Em gai"; break;
			            case 9: quanHe = "Bac trai"; break;
			            case 10: quanHe = "Bac gai"; break;
			            case 11: quanHe = "Chu"; break;
			            case 12: quanHe = "Co"; break;
			            case 13: quanHe = "Di"; break;
			            case 14: quanHe = "Anh/Chi/Em ho"; break;
			            case 15: quanHe = "Con"; break;
			            case 16: quanHe = "Chau"; break;
			            default: cout << "Lua chon khong hop le, vui long nhap lai!\n"; continue;
			        }
			        break; // Thoát vòng lặp nếu nhập đúng
			    }
			    
			    // 4. Gọi Backend xử lý
			    vector<Person*> ketQua = FindRelativesByRelationship(moc, quanHe);
			    
			    // 5. Hiển thị kết quả
			    if (ketQua.empty()) {
			        cout << "\n=> Khong tim thay ai la '" << quanHe << "' cua " << moc->name << " trong gia pha.\n";
			    } else {
			        cout << "\n=> Tim thay " << ketQua.size() << " nguoi thoa man la " << quanHe << " cua " << moc->name << ":\n";
			        cout << "--------------------------------------------------\n";
			        for (Person* p : ketQua) {
			            cout << " - ID: " << p->id << " | Ho ten: " << p->name << " (" << p->gender << ")";
			            if (p->deathDay != "N/A" && p->deathDay != "Unknown") {
			                cout << " [Da mat]";
			            }
			            cout << "\n";
			        }
			        cout << "--------------------------------------------------\n";
			    }
				break;
			}
            case 5:
                SaveTreeToFile(binaryFile);
                break;
            
            case 6: {
                string ten;
                cout << "Nhap ten nguoi can cap nhat: "; 
                getline(cin, ten);
                
                // Tận dụng hàm chống trùng tên để người dùng chọn đúng ID
                Person* p = SelectPersonFromName(ten);
                
                if (p != nullptr) {
                    UpdatePersonInfo(p);
                    cout << "\n--- THONG TIN SAU KHI CAP NHAT ---";
                    ShowDetail(p); // In ra để kiểm tra ngay
                }
                break;
            }
            
            case 7: {
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