#include <iostream>
#include <string>
#include <vector>
#include <conio.h> // Để dùng _getch()
#include <windows.h> // Để dùng màu sắc và ẩn con trỏ

using namespace std;

// --- TIỆN ÍCH GIAO DIỆN ---
void SetColor(int text, int background) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (background << 4) | text);
}

void HideCursor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

// --- MENU CHÍNH ---
void DrawMenu(int selected, vector<string> options) {
    system("cls");
    cout << "====================================================\n";
    cout << "||        HE THONG QUAN LY GIA PHA - PBL          ||\n";
    cout << "||      (Dung phim MUI TEN de di chuyen)          ||\n";
    cout << "====================================================\n\n";

    for (int i = 0; i < options.size(); ++i) {
        if (i == selected) {
            SetColor(0, 11); // Chữ đen, nền xanh cyan nhạt (Highlight)
            cout << "  >> " << options[i] << " <<  " << endl;
            SetColor(7, 0);  // Trả về mặc định
        } else {
            cout << "     " << options[i] << endl;
        }
    }
    cout << "\n----------------------------------------------------\n";
    cout << " [Enter]: Chon | [Esc]: Thoat";
}

int main() {
    HideCursor();
    vector<string> options = {
        "1. Nhap du lieu tu file (.txt)",
        "2. Them thanh vien moi",
        "3. Tim kiem & Cap nhat thong tin",
        "4. Xem so do gia pha (Tree View)",
        "5. Xac dinh quan he ho hang (LCA)",
        "6. Luu du lieu (.bin)",
        "0. Thoat chuong trinh"
    };

    int selected = 0;
    bool running = true;

    while (running) {
        DrawMenu(selected, options);

        // Doc phim tu ban phim
        int key = _getch();

        if (key == 224) { // Phím mũi tên trả về 224 trước, sau đó là mã phím
            key = _getch();
            if (key == 72) { // Mũi tên Lên
                selected = (selected - 1 + options.size()) % options.size();
            } else if (key == 80) { // Mũi tên Xuống
                selected = (selected + 1) % options.size();
            }
        } else if (key == 13) { // Phím Enter
            system("cls");
            SetColor(10, 0); // Chữ xanh lá
            cout << "\n[BAN DA CHON]: " << options[selected] << endl;
            SetColor(7, 0);
            
            cout << "\n(Day la mo phong logic... Bam phim bat ky de quay lai menu)";
            _getch();
            
            if (selected == options.size() - 1) running = false;
        } else if (key == 27) { // Phím Esc
            running = false;
        }
    }

    return 0;
}