#include "TreeManager.h"
#include "SearchEngine.h"
#include "FileHandler.h"
#include "Relationship.h"
#include <iostream>
#include <limits>
#include <conio.h>
#include <windows.h>

// Định nghĩa mã màu ANSI


using namespace std;

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void printHeader() {
    cout << DUT_BLUE << BOLD;
    cout << "==========================================================\n";
    cout << "  ____ ___    _      ____  _   _    _      \n";
    cout << " / ___|_ _|  / \\    |  _ \\| | | |  / \\     \n";
    cout << "| |  _ | |  / _ \\   | |_) | |_| | / _ \\    \n";
    cout << "| |_| || | / ___ \\  |  __/|  _  |/ ___ \\   \n";
    cout << " \\____|___/_/   \\_\\ |_|   |_| |_/_/   \\_\\  \n";
    cout << "                                           \n";
    cout << "       PHAN MEM QUAN LY GIA PHA - PBL 1    \n";
    cout << "==========================================================\n";
    cout << RESET;
}

void showInteractiveMenu(int selectedIndex) {
    system("cls");
    printHeader();

    vector<string> options = {
        "1. Hien thi cay gia pha",
        "2. Tim kiem & Xem chi tiet",
        "3. Kiem tra quan he ho hang",
        "4. Tim nguoi theo quan he",
        "5. Luu du lieu xuong file",
        "6. Cap nhat thong tin chi tiet",
        "7. Nhap lieu hang loat tu file",
        "0. Thoat chuong trinh"
    };

    for (int i = 0; i < options.size(); ++i) {
        if (i == selectedIndex) {

            cout << HIGHLIGHT << "  > " << options[i] << "  " << RESET << endl;
        } else {
            cout << "    " << options[i] << endl;
        }
    }
    cout << "\n" << DUT_BLUE << "(Dung mui ten Len/Xuong va Enter de chon)" << RESET;
}

int getMenuChoice() {
    int selectedIndex = 0;
    int numOptions = 8;

    while (true) {
        showInteractiveMenu(selectedIndex);

        int key = _getch();
        if (key == 224) {
            key = _getch();
            if (key == 72) {
                selectedIndex = (selectedIndex - 1 + numOptions) % numOptions;
            } else if (key == 80) {
                selectedIndex = (selectedIndex + 1) % numOptions;
            }
        } else if (key == 13) {
        	Beep(3000, 400);
            if (selectedIndex == 7) return 0;
            return selectedIndex + 1;
        }
    }
}

void pressAnyKey() {
    cout << "\n------------------------------------------";
    cout << "\nAn [Enter] de quay lai menu chính...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

bool askToContinue(string functionName) {
    cout << "\n" << DUT_BLUE << "------------------------------------------" << RESET;
    cout << "\n" << ORANGE << "Ban co muon tiep tuc " << BOLD << functionName << RESET << ORANGE << " khong?" << RESET;
    cout << "\n" << GREEN << "[Y]: Tiep tuc" << RESET << " | " << RED << "[N]: Ve Menu" << RESET;
    
    while (true) {
        char ch = _getch();
        if (ch == 'y' || ch == 'Y') {
            Beep(1000, 100); // Tiếng bíp cao cho "Đồng ý"
            return true;
        }
        if (ch == 'n' || ch == 'N' || ch == 27) { // 27 là phím ESC
            Beep(750, 150); // Tiếng bíp thấp cho "Thoát"
            return false;
        }
    }
}

void InitFixedData() {
    cout << "[He thong] Dang khoi tao bo du lieu gia pha mau...\n";
    Person* to = CreateFamily("Nguyen Van To", "Nam");
    
    Person* hung = AddChild(to, "Nguyen Van Hung", "Nam");
    Person* lan = AddChild(to, "Nguyen Thi Lan", "Nu");
    Person* dung = AddChild(to, "Nguyen Van Dung", "Nam");
    
    AddChild(hung, "Nguyen Van Minh", "Nam");
    AddChild(hung, "Nguyen Thi Hoa", "Nu");
    
    AddChild(dung, "Nguyen Van Kien", "Nam");

    AddChild(dung, "Nguyen Van Minh", "Nam"); 
}

// ======================================================================
// 7. MENU ĐIỀU KHIỂN CHÍNH
// ======================================================================

int main() {
    InitFixedData(); // Khởi tạo dữ liệu cứng ngay khi chạy 

    string binaryFile = "GiaPha_Test.dat";
    int luaChon;
    
    do {
    	clearScreen();
    	luaChon = getMenuChoice();
        switch (luaChon) {
            case 1:
            	clearScreen();
                cout << "\n--- CAY GIA PHA HIEN TAI ---\n";
                DisplayTree(root, 0);
                pressAnyKey();
                break;
            case 2: {
            	do {
            		clearScreen();
	                string ten;
	                cout << "Nhap ten can xem chi tiet (viet dung chinh ta ten cua nguoi do):";
	                getline(cin, ten);
	                SearchResult res = SelectPersonWithProxy(ten);
	                if (res.node != nullptr) {
				        ShowDetail(res);
				    }
//	                pressAnyKey();
//	                break;
				} while (askToContinue("tim kiem va xem chi tiet thanh vien"));
            	break;
            }
            case 3: {
            	do {
            		clearScreen();
	                string ten1, ten2;
	                cout << "Nhap ten nguoi thu nhat (viet dung chinh ta ten cua nguoi do): "; getline(cin, ten1);
	                SearchResult resA = SelectPersonWithProxy(ten1);
	                cout << "Nhap ten nguoi thu hai (viet dung chinh ta ten cua nguoi do): "; getline(cin, ten2);
	                SearchResult resB = SelectPersonWithProxy(ten2);
	                
					
					
					if (resA.node && resB.node) {
					    // Gọi hàm Universal mà chúng ta đã thống nhất
					    string quanHe = DetermineRelationship(resA.node, resA.isSpouse, resB.node, resB.isSpouse);
					    cout << "Ket qua: " << quanHe << endl;
					}
	
				} while (askToContinue("kiem tra quan he ho hang"));
            	break;
            }
            
            case 4: {
            	do {
            		clearScreen();
					string tenNguoi;
				    cout << "\n--- TIM NGUOI THEO QUAN HE ---\n";
				    cout << "Nhap ten nguoi lam moc (viet dung chinh ta ten cua nguoi do): ";
				    getline(cin, tenNguoi);
				    
				    // Gọi hàm tìm kiếm chống trùng tên
				    SearchResult res = SelectPersonWithProxy(tenNguoi); // 1. Lấy "vỏ bọc" kết quả
					if (res.node == nullptr) break;            // Kiểm tra xem có thấy ai không
					
					Person* moc = res.node;
				
				    // 1. In Menu Danh xưng dạng bảng để dễ nhìn
				    cout << "\n================ CHON QUAN HE CAN TIM ================\n";
				    cout << "  1. Cha           2. Me          3. Ong         4. Ba\n";
				    cout << "  5. Anh trai      6. Chi gai     7. Em trai     8. Em gai\n";
				    cout << "  9. Bac trai     10. Bac gai    11. Chu        12. Co        13. Di\n";
				    cout << " 14. Anh/Chi/Em ho               15. Con        16. Chau\n";
				    cout << " 17. Chau/Chat/... (Hau due cua Anh/Chi/Em)\n";
				    cout << "======================================================\n";
				    
				    // 2. Vòng lặp bắt lỗi nhập liệu
				    int luaChon;
				    string quanHe = "";
				    
				    while (true) {
				        cout << "Nhap lua chon (1-17): ";
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
				            case 17: quanHe = "Hau due cua Anh/Chi/Em"; break;
				            default: cout << "Lua chon khong hop le, vui long nhap lai!\n"; continue;
				        }
				        break; // Thoát vòng lặp nếu nhập đúng
				    }
				    
				    // 4. Gọi Backend xử lý
				    // 4. Gọi Backend xử lý (Hàm này trả về vector<string>)
						// Sửa dòng này:
						// 4. Gọi Backend xử lý - Dùng hàm trả về String để lấy được tên Vợ
						vector<string> ketQua = FindRelativesNames(moc, quanHe);
						
						// 5. Hiển thị kết quả
						if (ketQua.empty()) {
						    cout << "\n" << RED << "=> Khong tim thay ai la '" << quanHe << "' cua " << moc->name << RESET << endl;
						} else {
						    cout << "\n" << GREEN << "=> Tim thay " << ketQua.size() << " " << quanHe << " cua " << moc->name << ":" << RESET << endl;
						    cout << "--------------------------------------------------\n";
						    for (const string& ten : ketQua) {
						        // Nếu tìm Mẹ, nó sẽ in ra: "Tên Vợ (Vo cua Hùng)"
						        cout << " - " << BOLD << ten << RESET << endl;
						    }
						    cout << "--------------------------------------------------\n";
						}
				} while (askToContinue("tim nguoi theo quan he"));
				break;
		}
            case 5:
            	clearScreen();
                SaveTreeToFile(binaryFile);
                pressAnyKey();
                break;
            
            case 6: {
	            	do {
	            		clearScreen();
		                string ten;
		                cout << "Nhap ten nguoi can cap nhat (viet dung chinh ta ten cua nguoi do): "; 
		                getline(cin, ten);
	                
	                	// Tận dụng hàm chống trùng tên để người dùng chọn đúng ID
	                	
	                	// 1. Lấy "vỏ bọc" kết quả tìm kiếm
						SearchResult res = SelectPersonWithProxy(ten); 
						
						// 2. Kiểm tra xem có tìm thấy ai không (Kiểm tra cái lõi .node)
						if (res.node != nullptr) {
						    
						    // 3. Cập nhật thông tin: 
						    // Nếu là Vợ (isSpouse), ta cần một logic cập nhật riêng cho trường spouseName
						    if (res.isSpouse) {
						        cout << "Dang cap nhat thong tin cho Phu nhan: " << res.node->spouseName << endl;
						        cout << "Nhap ten moi: ";
						        getline(cin, res.node->spouseName); // Cập nhật trực tiếp vào trường spouseName của chồng
						    } 
						    else {
						        // Nếu là Nút chính quy, gọi hàm cập nhật Person* như cũ của ông
						        UpdatePersonInfo(res.node); 
						    }
						
						    cout << "\n--- THONG TIN SAU KHI CAP NHAT ---";
						    
						    // 4. In ra kiểm tra (Dùng hàm ShowDetail đã Overload cho SearchResult)
						    ShowDetail(res); 
						} 
						else {
						    cout << RED << "Khong tim thay nguoi nay trong gia pha!" << RESET << endl;
						}
	            } while (askToContinue("cap nhat thong tin"));
	            break;
	        }
            
            case 7: {
            	clearScreen();
                string textFile;
                cout << "Nhap ten file text (Dinh dang: TenCha, TenCon, GioiTinh)\n";
                cout << "VD: None, Nguyen Van A, Nam\n";
                cout << "Ten file (VD: NhapLieu.txt): ";
                getline(cin, textFile);
                ImportFromTextFile(textFile);
                pressAnyKey();
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