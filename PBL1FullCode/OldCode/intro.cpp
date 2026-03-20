#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct Person {
    int id;             // Vị trí trong vector (0, 1, 2...)
    string name;
    string gender;      // "Nam" hoac "Nu"
    
    // Quản lý mối quan hệ bằng CHỈ SỐ (Index)
    // -1 nghĩa là không có (tương đương NULL)
    int parentID;       
    int firstChildID;   
    int nextSiblingID;  

    // Constructor để khởi tạo nhanh
    Person(int _id, string _name, string _gender, int _parentID) {
        id = _id;
        name = _name;
        gender = _gender;
        parentID = _parentID;
        firstChildID = -1;  // Mới sinh ra chưa có con
        nextSiblingID = -1; // Chưa có em
    }
};

// Biến toàn cục chứa cây gia phả
vector<Person> familyTree;

void CreateFamily(string name, string gender) {
    familyTree.clear(); // Xóa dữ liệu cũ nếu có
    // ID của ông tổ là 0, parentID là -1 (không có cha)
    Person root(0, name, gender, -1);
    familyTree.push_back(root);
    cout << "Da khoi tao gia pha voi ong to: " << name << endl;
}

void AddChild(int pID, string name, string gender) {
    // 1. Kiểm tra ID cha có hợp lệ không
    if (pID < 0 || pID >= familyTree.size()) {
        cout << "Loi: Khong tim thay nguoi cha co ID = " << pID << endl;
        return;
    }
    
    if (familyTree[pID].gender == "Nu") {
    	cout << "Loi: Khong the them con cho nguoi nu theo gia pha!" << endl;
    	return;
	}
	
    // 2. Tạo người con mới
    int newChildID = familyTree.size(); // ID mới bằng kích thước hiện tại
    Person child(newChildID, name, gender, pID);
    familyTree.push_back(child); // Thêm vào mảng

    // 3. Cập nhật mối liên kết cho CHA hoặc ANH/CHỊ
    // Lấy tham chiếu đến người cha (dùng & để cập nhật trực tiếp vào mảng)
    Person &parent = familyTree[pID]; 

    if (parent.firstChildID == -1) {
        // TRƯỜNG HỢP 1: Cha chưa có con nào
        // Đứa này là con cả -> Nối trực tiếp vào cha
        parent.firstChildID = newChildID;
    } else {
        // TRƯỜNG HỢP 2: Cha đã có con
        // Phải tìm đứa con út hiện tại để nối đứa mới vào sau
        int currentID = parent.firstChildID;
        
        // Duyệt đến khi tìm thấy người không còn em kế tiếp (nextSiblingID == -1)
        while (familyTree[currentID].nextSiblingID != -1) {
            currentID = familyTree[currentID].nextSiblingID;
        }
        
        // Đã tìm thấy đứa út (currentID), giờ nối đứa mới vào
        familyTree[currentID].nextSiblingID = newChildID;
    }
	
	if (gender != "Nam" && gender != "Nu") {
    	cout << "Loi: Gioi tinh khong hop le!" << endl;
    	return;
	}

    cout << "Da them " << name << " lam con cua " << parent.name << endl;
}
void PrintList() {
    cout << "\n--- DANH SACH THANH VIEN ---\n";
    for (const auto &p : familyTree) {
        cout << "ID: " << p.id 
             << " | Ten: " << p.name 
             << " | Cha ID: " << p.parentID 
             << " | Con Dau ID: " << p.firstChildID 
             << " | Em Ke ID: " << p.nextSiblingID << endl;
    }
}

// Hàm bổ trợ để in thụt lề
void DisplayTree(int currentID, int level) {
    if (currentID == -1) return;

    // 1. Tạo khoảng trắng thụt lề dựa trên cấp độ (level)
    for (int i = 0; i < level; i++) cout << "    ";
    
    // 2. In tên
    cout << "|-- " << familyTree[currentID].name << " (ID: " << currentID << ")" << endl;

    // 3. Đệ quy xuống con đầu tiên (Tăng level)
    DisplayTree(familyTree[currentID].firstChildID, level + 1);

    // 4. Đệ quy sang người em kế tiếp (Giữ nguyên level)
    DisplayTree(familyTree[currentID].nextSiblingID, level);
}

int main() {
    // Test thử kịch bản
    CreateFamily("Nguyen Van To", "Nam");   // ID 0
    
    AddChild(0, "Nguyen Van Hung", "Nam");   // ID = 1
	AddChild(0, "Nguyen Van Khai", "Nam");   // ID = 2
	AddChild(0, "Nguyen Thi Lan",  "Nu");    // ID = 3

    AddChild(1, "Nguyen Van Minh", "Nam");   // ID = 4
	AddChild(1, "Nguyen Thi Hoa",  "Nu");    // ID = 5
	AddChild(1, "Nguyen Van Long", "Nam");   // ID = 6
	
	AddChild(2, "Nguyen Van Tuan",  "Nam");  // ID = 7
	AddChild(2, "Nguyen Thi Huong", "Nu");   // ID = 8
	
	AddChild(4, "Nguyen Van Quan", "Nam");   // ID = 9
	AddChild(4, "Nguyen Thi Mai",  "Nu");    // ID = 10
	
	AddChild(6, "Nguyen Van Phuc", "Nam");   // ID = 11
	
	AddChild(7, "Nguyen Van Nam",  "Nam");   // ID = 12
	AddChild(7, "Nguyen Van Bac",  "Nam");   // ID = 13
	
	AddChild(9, "Nguyen Van An", "Nam");     // ID = 14
	
	AddChild(11, "Nguyen Thi Linh", "Nu");   // ID = 15
	AddChild(11, "Nguyen Van Duc",  "Nam");  // ID = 16

    DisplayTree(0, 1);
    PrintList();
    return 0;
}