#include "Relationship.h"  // 1. Luôn include file .h của chính nó
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

vector<Person*> GetPathToRoot(Person* p) {
    vector<Person*> path;
    while (p != nullptr) {
        path.push_back(p);
        p = p->parent;
    }
    reverse(path.begin(), path.end());
    return path;
}

// Thêm 2 tham số bool để xác định đối tượng có phải là "Vợ" của nút đang xét không
string DetermineRelationship(Person* pA, bool isSpouseA, Person* pB, bool isSpouseB) {
    if (pA == nullptr || pB == nullptr) return "Loi: Nguoi khong ton tai!";
    
    // Nếu A và B trỏ cùng 1 nút và cả hai đều là vợ/chồng hoặc cả hai là chính nút đó
    if (pA == pB && isSpouseA == isSpouseB) return "Chinh la mot nguoi!";
    if (pA == pB && isSpouseA != isSpouseB) return "Quan he: Vo chong";

    vector<Person*> pathA = GetPathToRoot(pA);
    vector<Person*> pathB = GetPathToRoot(pB);

    int lcaIndex = -1;
    int minLen = min((int)pathA.size(), (int)pathB.size());
    for (int i = 0; i < minLen; i++) {
        if (pathA[i] == pathB[i]) lcaIndex = i; 
        else break;
    }

    if (lcaIndex == -1) return "Khong co quan he ho hang";

    int dA = (pathA.size() - 1) - lcaIndex;
    int dB = (pathB.size() - 1) - lcaIndex;
    Person* lca = pathA[lcaIndex];

    // --- XỬ LÝ DANH XƯNG TỔNG QUÁT (Dựa trên Proxy) ---
    
    // 1. A là bề trên của B (Trực hệ ngược)
    if (dB == 0) {
        if (dA == 1) return isSpouseA ? "Me" : "Cha";
        if (dA == 2) return isSpouseA ? "Ba noi" : "Ong noi";
        if (dA == 3) return isSpouseA ? "Ba co" : "Ong co";
        return isSpouseA ? "To tien (Nu)" : "To tien (Nam)";
    }

    // 2. B là bề trên của A (Trực hệ thuận)
    if (dA == 0) {
        if (isSpouseA) return "Nang dau"; // Vợ của nút A (A=B) là con dâu của B
        if (dB == 1) return "Con";
        if (dB == 2) return "Chau";
        return "Hau due";
    }

    // 3. Quan hệ ngang hàng (Anh chị em)
    if (dA == 1 && dB == 1) {
        bool aIsOlder = false;
        Person* curr = lca->firstChild;
        while (curr != nullptr) {
            if (curr == pA) { aIsOlder = true; break; }
            if (curr == pB) { aIsOlder = false; break; }
            curr = curr->nextSibling;
        }
        
        if (isSpouseA) {
            if (aIsOlder) return (pA->gender == "Nam") ? "Chi dau" : "Anh re";
            else return (pA->gender == "Nam") ? "Em dau" : "Em re";
        }

        if (aIsOlder) return (pA->gender == "Nam") ? "Anh trai" : "Chi gai";
        return (pA->gender == "Nam") ? "Em trai" : "Em gai";
    }

    // 4. Quan hệ bàng hệ (Cô, Chú, Bác, Dì...)
    if (dA == 1 && dB == 2) {
        Person* aNode = pathA[lcaIndex + 1];
        Person* parentOfB = pathB[lcaIndex + 1];
        
        bool aIsOlder = false;
        Person* curr = lca->firstChild;
        while (curr != nullptr) {
            if (curr == aNode) { aIsOlder = true; break; }
            if (curr == parentOfB) { aIsOlder = false; break; }
            curr = curr->nextSibling;
        }

        if (isSpouseA) { // Vợ của bác/chú/cô
            if (aIsOlder) return "Bac gai (Vo bac)";
            return (pA->gender == "Nam") ? "Thim (Vo chu)" : "Duong (Chong co)";
        }
        
        if (aIsOlder) return (pA->gender == "Nam") ? "Bac trai" : "Bac gai";
        return (pA->gender == "Nam") ? "Chu" : "Co";
    }

    return "Ho hang xa (Cach " + to_string(dA + dB) + " doi)";
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
    else if (relationName == "Hau due cua Anh/Chi/Em") { dA = 1; dB = -2;}
    else {
        cout << "[Loi] Chua ho tro truy van danh xung: " << relationName << "\n";
        return finalResults;
    }

    // BƯỚC 1: TÌM TỔ TIÊN CHUNG
    Person* ancestor = GetAncestor(startPerson, dA);
    if (ancestor == nullptr) return finalResults; // Không đủ thế hệ (VD: Gốc tìm Cha)

    // BƯỚC 2: THU THẬP ỨNG VIÊN
    vector<Person*> candidates;
    if (dB == -2) {
    // Thu thập từ đời cháu (2), chắt (3), chút (4)...
    Person* siblingOfStart = ancestor->firstChild;
    while (siblingOfStart != nullptr) {
        if (siblingOfStart != startPerson) { // Không quét nhánh của chính mình
            for (int i = 2; i <= 5; i++) { // Quét sâu xuống 5 đời
                CollectAtDepth(siblingOfStart, i, 1, candidates);
            }
        }
        siblingOfStart = siblingOfStart->nextSibling;
    }
	} else if (dB == 0) { candidates.push_back(ancestor); }
 	else {
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
        else if (relationName == "Hau due cua Anh/Chi/Em") { match = true;}  // Chấp nhận tất cả vì đã lọc nhánh ở Bước 2

        if (match) finalResults.push_back(p);
    }

    return finalResults;
}

vector<string> FindRelativesNames(Person* startPerson, string relationName) {
    vector<string> names;
    if (startPerson == nullptr) return names;

    // Các trường hợp mẫu hệ trực tiếp (Tìm vợ của các nút tổ tiên)
    if (relationName == "Me") {
        if (startPerson->parent != nullptr && !startPerson->parent->spouseName.empty())
            names.push_back(startPerson->parent->spouseName + " (Vo cua " + startPerson->parent->name + ")");
        return names;
    }
    
    if (relationName == "Ba") {
        Person* dad = startPerson->parent;
        if (dad != nullptr && dad->parent != nullptr && !dad->parent->spouseName.empty())
            names.push_back(dad->parent->spouseName + " (Ba noi - Vo cua " + dad->parent->name + ")");
        return names;
    }

    vector<Person*> results = FindRelativesByRelationship(startPerson, relationName);
    for (Person* p : results) {
        names.push_back(p->name);
    }
    
    return names;
}