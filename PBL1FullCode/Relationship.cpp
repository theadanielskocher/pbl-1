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