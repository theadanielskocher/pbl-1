#include <iostream>
#include <windows.h>
#include <conio.h>
#include <string>
#include <vector>

using namespace std;

const int WHITE = 15;
const int YELLOW = 14;
const int GREEN = 10;
const int RED = 12;

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void drawMenu(const vector<string>& options, int selected) {
    system("cls");
    setColor(YELLOW);
    cout << "\n   ===== QUAN LY GIA PHA =====\n\n";

    for (int i = 0; i < options.size(); i++) {
        if (i == selected) {
            setColor(GREEN);
            cout << "  > " << options[i] << " <\n";
        } else {
            setColor(WHITE);
            cout << "    " << options[i] << "\n";
        }
    }

    setColor(RED);
    cout << "\n   Su dung mui ten len/xuong - Enter chon - ESC thoat\n";
}

int main() {
    vector<string> options = {
        "1. Them nguoi moi (to tien)",
        "2. Them con cho mot nguoi",
        "3. Hien thi cay gia pha",
        "4. Tim kiem theo ten",
        "5. Luu vao file",
        "6. Doc tu file",
        "0. Thoat"
    };

    int selected = 0;
    bool running = true;

    while (running) {
        drawMenu(options, selected);

        int key = _getch();
        if (key == 0 || key == 224) {  // phím đặc biệt
            key = _getch();
            if (key == 72 && selected > 0) selected--;           // lên
            if (key == 80 && selected < options.size()-1) selected++; // xuống
        }
        else if (key == 13) {  // Enter
            system("cls");
            cout << "Ban da chon: " << options[selected] << endl;
            cout << "Xu ly chuc nang o day...\n";
            cout << "\nNhan phim bat ky de quay lai menu...";
            _getch();
        }
        else if (key == 27) {  // ESC
            running = false;
        }
    }

    setColor(WHITE);
    cout << "\nTam biet!\n";
    return 0;
}