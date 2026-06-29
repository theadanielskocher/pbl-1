#include "TreeManager.h"
#include "SearchEngine.h"
#include "FileHandler.h"
#include "Relationship.h"
#include <iostream>
#include <limits>
#include <conio.h>
#include <fstream>
#include <windows.h>

using namespace std;

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

struct FamilyStats {
    int totalMembers = 0;
    int totalSpouses = 0;
    int maxGen = 0;
    int maleCount = 0;
    int femaleCount = 0;
    int deceasedCount = 0;
    int genDistribution[20] = {0};
};

void CalculateStats(Person* p, int level, FamilyStats &s) {
    if (p == nullptr) return;

    s.totalMembers++;
    if (p->gender == "Nam") s.maleCount++; 
    else if (p->gender == "Nu") s.femaleCount++;

    if (!p->spouseName.empty() && p->spouseName != "None" && p->spouseName != "N/A") {
        s.totalSpouses++;
    }

    if (p->deathDay != "N/A" && p->deathDay != "Unknown" && !p->deathDay.empty()) {
        s.deceasedCount++;
    }

    if (level < 20) s.genDistribution[level]++;
    if (level > s.maxGen) s.maxGen = level;

    CalculateStats(p->firstChild, level + 1, s);
    CalculateStats(p->nextSibling, level, s);
}

void printHeader() {
    system("cls"); 
    
    const int width = 78; 

    cout << DUT_BLUE << BOLD;

    cout << (char)201; 
    for(int i = 0; i < width; i++) cout << (char)205;
    cout << (char)187 << endl;

    cout << (char)186 << R"(     ____                      _               _               _            )" << "  " << (char)186 << endl;
    cout << (char)186 << R"(    / __ \                    | |             (_)             | |           )" << "  " << (char)186 << endl;
    cout << (char)186 << R"(   | |  | |_   _  __ _ _ __   | |_   _    __ _ _  __ _   _ __ | |__   __ _  )" << "  " << (char)186 << endl;
    cout << (char)186 << R"(   | |  | | | | |/ _` | '_ \  | | | | |  / _` | |/ _` | | '_ \| '_ \ / _` | )" << "  " << (char)186 << endl;
    cout << (char)186 << R"(   | |__| | |_| | (_| | | | | | | |_| | | (_| | | (_| | | |_) | | | | (_| | )" << "  " << (char)186 << endl;
    cout << (char)186 << R"(    \___\_\\__,_|\__,_|_| |_| |_|\__, |  \__, |_|\__,_| | .__/|_| |_|\__,_| )" << "  " << (char)186 << endl;
    cout << (char)186 << R"(                                  __/ |   __/ |         | |                 )" << "  " << (char)186 << endl;
    cout << (char)186 << R"(                                 |___/   |___/          |_|                 )" << "  " << (char)186 << endl;

    cout << (char)199;
    for(int i = 0; i < width; i++) cout << (char)196;
    cout << (char)182 << endl;

    string subTitle = "PBL 1 - Do an lap trinh tinh toan";
    string subTitle2 = "SVTH: Nguyen Dinh Hai Dang va Le Ba Son - Lop 25T_KHDL";
    int padding = (width - subTitle.length()) / 2;
    int padding2 = (width - subTitle2.length()) / 2;
    
    cout << (char)186;
    for(int i = 0; i < padding; i++) cout << " ";
    cout << YELLOW << subTitle << DUT_BLUE;
    for(int i = 0; i < width - subTitle.length() - padding; i++) cout << " ";
    cout << (char)186 << endl;

	cout << (char)186;
	for (int i = 0; i < padding2; i++) cout << " ";
	cout << YELLOW << subTitle2 << DUT_BLUE;
	for (int i = 0; i < width - subTitle2.length() - padding2; i++) cout << " ";
	cout << (char)186 << endl;

    cout << (char)200; 
    for(int i = 0; i < width; i++) cout << (char)205;
    cout << (char)188 << endl;

    cout << RESET;
}

void showInteractiveMenu(int selectedIndex) {
    system("cls");
    printHeader();

    vector<string> options = {
    	"0. Thoat chuong trinh",
        "1. Hien thi cay gia pha",
        "2. Tim kiem & Xem chi tiet",
        "3. Kiem tra quan he ho hang",
        "4. Tim nguoi theo quan he",
        "5. Luu du lieu xuong file",
        "6. Cap nhat thong tin chi tiet",
        "7. Nhap lieu hang loat tu file",
        "8. Thong ke"
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
    int selectedIndex = 1;
    int numOptions = 8;

    while (true) {
        showInteractiveMenu(selectedIndex);

        int key = _getch();
        if (key == 224) {
            key = _getch();
            if (key == 72) { 
                selectedIndex--;
                if (selectedIndex < 0) selectedIndex = numOptions;
            } 
            else if (key == 80) {
                selectedIndex++;
                if (selectedIndex > numOptions) selectedIndex = 0;
            }
        } 
        else if (key == 13) {
            Beep(3000, 400); 
            return selectedIndex; 
        }
        else if (key == '0') {
            Beep(3000, 400);
            return 0;
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
            Beep(1000, 100);
            return true;
        }
        if (ch == 'n' || ch == 'N' || ch == 27) {
            Beep(750, 150);
            return false;
        }
    }
}

void InitFixedData() {
    cout << "[He thong] Dang khoi tao bo du lieu gia pha mau...\n";
    sleep(2);
    Person* to = CreateFamily("Nguyen Van To", "Nam", "19/09/1928", 1928);
    Person* hung = AddChild(to, "Nguyen Van Long", "29/03/1949", "Nam");
    Person* vanh = AddChild(to, "Nguyen Van Vanh", "01/01/1946", "Nam");
}


int main() {
    InitFixedData();

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
	                cout << "Nhap ten can xem chi tiet (viet dung chinh ta ten cua nguoi do):\n";
	                getline(cin, ten);
	                SearchResult res = SelectPersonWithProxy(ten);
	                if (res.node != nullptr) {
				        ShowDetail(res);
				    }
				} while (askToContinue("tim kiem va xem chi tiet thanh vien"));
            	break;
            }
            case 3: {
            	do {
            		clearScreen();
	                string ten1, ten2;
	                cout << "Nhap ten nguoi thu nhat (viet dung chinh ta ten cua nguoi do):\n"; getline(cin, ten1);
	                SearchResult resA = SelectPersonWithProxy(ten1);
	                
	                if (resA.node == nullptr) { 
			            cout << "Vui long kiem tra lai ten nguoi thu nhat!" << endl;
			            system("pause");
			            continue;
			        }
        
	                cout << "Nhap ten nguoi thu hai (viet dung chinh ta ten cua nguoi do):\n"; getline(cin, ten2);
	                SearchResult resB = SelectPersonWithProxy(ten2);
	                
					if (resB.node == nullptr) {
			            cout << "Vui long kiem tra lai ten nguoi thu hai!" << endl;
			            system("pause");
			            continue;
			        }
					
					if (resA.node && resB.node) {
					    string quanHe = DetermineRelationship(resA.node, resA.isSpouse, resB.node, resB.isSpouse);
					    
					    if (ten1 == ten2) {
					    	cout << BOLD << quanHe << endl;
						}
						else {
							cout << "\n" << GREEN << BOLD << "==> KET QUA: " << RESET;
						    cout << BOLD << ten1 << RESET << " la " << RED << BOLD << quanHe << RESET;
						    cout << " cua " << BOLD << ten2 << RESET << endl;
						}
					    
					}
	
				} while (askToContinue("kiem tra quan he ho hang"));
            	break;
            }
            
            case 4: {
            	do {
            		clearScreen();
					string tenNguoi;
				    cout << "\n--- TIM NGUOI THEO QUAN HE ---\n";
				    cout << "Nhap ten nguoi lam moc (viet dung chinh ta ten cua nguoi do):\n";
				    getline(cin, tenNguoi);
				    
				    SearchResult res = SelectPersonWithProxy(tenNguoi);
					if (res.node == nullptr) break; 
					
					Person* moc = res.node;
				
				    cout << "\n================ CHON QUAN HE CAN TIM ================\n";
				    cout << "  1. Cha           2. Me          3. Ong         4. Ba\n";
				    cout << "  5. Anh trai      6. Chi gai     7. Em trai     8. Em gai\n";
				    cout << "  9. Bac trai     10. Bac gai    11. Chu        12. Co        13. Di\n";
				    cout << " 14. Anh/Chi/Em ho               15. Con        16. Chau\n";
				    cout << " 17. Chau/Chat/... (Hau due cua Anh/Chi/Em)\n";
				    cout << "======================================================\n";
				    
				    int luaChon;
				    string quanHe = "";
				    
				    while (true) {
				        cout << "Nhap lua chon (1-17): ";
				        cin >> luaChon;
				        cin.ignore(256, '\n'); 
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
				        break;
				    }
				    
						vector<string> ketQua = FindRelativesNames(moc, quanHe);
						
						if (ketQua.empty()) {
						    cout << "\n" << RED << "=> Khong tim thay ai la '" << quanHe << "' cua " << moc->name << RESET << endl;
						} else {
						    cout << "\n" << GREEN << "=> Tim thay " << ketQua.size() << " " << quanHe << " cua " << moc->name << ":" << RESET << endl;
						    cout << "--------------------------------------------------\n";
						    for (const string& ten : ketQua) {
						        cout << " - " << BOLD << ten << RESET << endl;
						    }
						    cout << "--------------------------------------------------\n";
						}
				} while (askToContinue("tim nguoi theo quan he"));
				break;
		}
			case 5: {
                clearScreen();
                if (root == nullptr) {
                    cout << RED << "Loi: Gia pha hien tai dang trong, khong co du lieu de luu!" << RESET << endl;
                    pressAnyKey();
                    break;
                }

                string userFileName;
                bool isDuplicate = true;

                while (isDuplicate) {
                    cout << YELLOW << "--- LUU DU LIEU GIA PHA ---" << RESET << endl;
                    cout << "Nhap ten file muon luu (VD: GiaPhaV2.dat): ";
                    getline(cin, userFileName);

                    if (userFileName.length() < 4 || userFileName.substr(userFileName.length() - 4) != ".dat") {
                        userFileName += ".dat";
                    }

                    ifstream checkFile(userFileName);
                    if (checkFile.is_open()) {
                        cout << RED << "[Canh bao] File '" << userFileName << "' da ton tai!" << RESET << endl;
                        cout << "Vui long dat mot ten khac de tranh ghi de du lieu cu.\n" << endl;
                        checkFile.close();
                    } else {
                        isDuplicate = false;
                    }
                }

                SaveTreeToFile(userFileName);
                pressAnyKey();
                break;
            }
            
            case 6: {
	            	do {
	            		clearScreen();
		                string ten;
		                cout << "Nhap ten nguoi can cap nhat (viet dung chinh ta ten cua nguoi do):\n"; 
		                getline(cin, ten);
	                
						SearchResult res = SelectPersonWithProxy(ten); 
						
						if (res.node != nullptr) {

						    if (res.isSpouse) {
						        cout << "Dang cap nhat thong tin cho Phu nhan: " << res.node->spouseName << endl;
						        cout << "Nhap ten moi: ";
						        getline(cin, res.node->spouseName);
						    } 
						    else {
						        UpdatePersonInfo(res.node); 
						    }
						
						    cout << "\n--- THONG TIN SAU KHI CAP NHAT ---";
						    
						    ShowDetail(res); 
						} 
						else {
						    cout << RED << "Khong tim thay nguoi nay trong gia pha!" << RESET << endl;
						}
	            } while (askToContinue("cap nhat thong tin"));
	            break;
	        }
            
            case 7: {
                do {
                    clearScreen();
                    cout << YELLOW << BOLD << "================= NAP DU LIEU GIA PHA =================" << RESET << endl;
                    cout << "  1. Nap tu file nhi phan (.dat) - Phuc hoi trang thai cu\n";
                    cout << "  2. Nap tu file van ban (.txt)  - Nhap moi hang loat\n";
                    cout << "  0. Quay lai Menu chinh\n";
                    cout << YELLOW << "=======================================================" << RESET << endl;
                    
                    int subChoice;
                    cout << "Nhap lua chon cua ban: ";
                    cin >> subChoice;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    if (subChoice == 0) break;

                    string fileName;
                    if (subChoice == 1) {
                        cout << "\n[Luu y] Nap file .dat se khoi phuc toan bo cau truc va ID da luu.\n";
                        cout << "Nhap ten file nhi phan (VD: GiaPha_Test.dat): ";
                        getline(cin, fileName);
                        if (!fileName.empty()) {
                            LoadTreeFromFile(fileName);
                        }
                    } 
                    else if (subChoice == 2) {
                        cout << "\n[Luu y] File .txt phai dung dinh dang: Cha, Con, Gioi Tinh, Nam Sinh, Ngay Sinh\n";
                        cout << "Nhap ten file van ban (VD: NhapLieu.txt): ";
                        getline(cin, fileName);
                        if (!fileName.empty()) {
                            ImportFromTextFile(fileName);
                        }
                    } 
                    else {
                        cout << RED << "Lua chon khong hop le!" << RESET << endl;
                    }

                    cout << "\nAn [Enter] de tiep tuc...";
                    cin.get();
                } while (askToContinue("thao tac nap file"));
                break;
            }
            
            case 8: {
			    clearScreen();
			    FamilyStats s;
			    CalculateStats(root, 1, s); 
			
			    cout << YELLOW << BOLD << "================= THONG KE GIA PHA =================" << RESET << endl;
			    cout << " 1. Quy mo dong ho:  " << BOLD << s.totalMembers + s.totalSpouses << " nguoi" << RESET << endl;
			    cout << "    - Huyet thong:   " << s.totalMembers << endl;
			    cout << "    - Phu nhan:      " << s.totalSpouses << endl;
			    cout << " 2. So doi (Generations): " << BOLD << s.maxGen << RESET << endl;
			    cout << " 3. Co cau gioi tinh: " << DUT_BLUE << "Nam: " << s.maleCount 
			         << RESET << " | " << RED << "Nu: " << s.femaleCount << RESET << endl;
			    cout << " 4. Tinh trang:      " << "Con song: " << (s.totalMembers - s.deceasedCount) 
			         << " | Da khuat: " << s.deceasedCount << endl;
			    
			    cout << "\n" << BOLD << "--- bieu do phan bo thanh vien theo doi ---" << RESET << endl;
			    for (int i = 1; i <= s.maxGen; i++) {
			        cout << " Doi " << (i < 10 ? "0" : "") << i << ": ";
			        for (int j = 0; j < s.genDistribution[i]; j++) cout << "*"; 
			        cout << " (" << s.genDistribution[i] << ")" << endl;
			    }
			    cout << YELLOW << "====================================================" << RESET << endl;
			    
			    cout << "\nNhan Enter de quay lai menu...";
			    cin.ignore(); cin.get();
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