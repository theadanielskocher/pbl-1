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