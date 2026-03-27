#include "SearchEngine.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;


// --- 2. THUẬT TOÁN DFS QUÉT TOÀN CÂY (Nâng cấp Proxy) ---
void FindAllMatches(Person* current, string targetName, vector<SearchResult>& results) {
    if (current == nullptr) return;

    // Kiểm tra tên chính (Nam giới/Con cái)
    if (current->name == targetName) {
        results.push_back({current, false});
    }

    // Kiểm tra tên vợ (Spouse) - Đây là chìa khóa để tìm Mẹ, Bà...
    if (current->spouseName == targetName) {
        results.push_back({current, true});
    }

    FindAllMatches(current->firstChild, targetName, results);
    FindAllMatches(current->nextSibling, targetName, results);
}

// --- 3. HÀM CHỌN NGƯỜI TƯƠNG TÁC (Nâng cấp giao diện chọn) ---
// Hàm này trả về SearchResult để main.cpp biết được đối tượng chọn là Spouse hay Node
SearchResult SelectPersonWithProxy(string targetName) {
    vector<SearchResult> results;
    FindAllMatches(root, targetName, results);

    if (results.empty()) {
        cout << "\n\033[1;31m[Loi] Khong tim thay ai co ten la '" << targetName << "'!\033[0m\n";
        return {nullptr, false};
    }

    // Nếu chỉ có 1 kết quả duy nhất
    if (results.size() == 1) return results[0];

    // Nếu trùng tên (Double name handling)
    cout << "\n\033[1;36m[Chu y] Tim thay " << results.size() << " ket qua cho ten '" << targetName << "':\033[0m\n";
    cout << "------------------------------------------------------\n";
    for (size_t i = 0; i < results.size(); i++) {
        cout << i + 1 << ". ";
        if (results[i].isSpouse) {
            cout << "[VO]  " << targetName << " (Vo cua " << results[i].node->name << ")";
        } else {
            cout << "[NUT] " << results[i].node->name << " (ID: " << results[i].node->id << ")";
        }
        
        if (results[i].node->parent != nullptr) 
            cout << " | Nhanh: " << results[i].node->parent->name;
        cout << endl;
    }
    cout << "------------------------------------------------------\n";

    int choice = 0;
    while (true) {
        cout << "Nhap so thu tu de chon: ";
        if (!(cin >> choice)) { cin.clear(); cin.ignore(1000, '\n'); continue; }
        if (choice >= 1 && choice <= (int)results.size()) return results[choice - 1];
        cout << "Lua chon khong hop le!\n";
    }
}

// --- 4. HÀM TÌM KIẾM NHANH CHO IMPORT ---
Person* FindFirstMatchForImport(Person* rootNode, string targetName) {
    // Với Import, ta ưu tiên tìm nút chính quy trước
    vector<SearchResult> results;
    FindAllMatches(rootNode, targetName, results);
    
    if (results.empty()) return nullptr;
    
    // Ưu tiên trả về Node chính, nếu không có mới trả về Proxy
    for(auto res : results) {
        if (!res.isSpouse) return res.node;
    }
    return results[0].node;
}