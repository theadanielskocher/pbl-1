#include <iostream>
#include <string>
#include <windows.h> // Thu vien de bat mau sac tren Windows

using namespace std;

// --- Dinh nghia cac ma mau ANSI ---
#define RESET   "\x1b[0m"
#define BLUE    "\x1b[94m"
#define CYAN    "\x1b[96m"
#define GREEN   "\x1b[92m"
#define YELLOW  "\x1b[93m"
#define BOLD    "\x1b[1m"

void SetConsoleFont() {
    // 1. Thiet lap Code Page de ho tro hien thi UTF-8
    SetConsoleOutputCP(65001);

    // 2. Thong tin font chu mong muon
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    cfi.nFont = 0;
    cfi.dwFontSize.X = 0;                  // Chieu rong tu dong
    cfi.dwFontSize.Y = 18;                 // Do cao font (ban co the doi thanh 20 hoac 24)
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;

    // Chon font ho tro Unicode tot nhat
    wcscpy_s(cfi.FaceName, L"Consolas"); 

    // 3. Ap dung font cho cua so hien tai
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}


// Ham bat che do mau sac cho Windows Console
void EnableANSI() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}

void DrawHeader() {
    system("cls");
    cout << BLUE << "┌───────────────────────────────────────────┐" << RESET << endl;
    cout << BLUE << "│" << BOLD << CYAN << "        MAY TINH DON GIAN - PBL 1          " << RESET << BLUE << "│" << RESET << endl;
    cout << BLUE << "└───────────────────────────────────────────┘" << RESET << endl;
}

void DrawBox(string label, string value) {
    cout << CYAN << "  " << label << RESET << endl;
    cout << BLUE << "  ┌──────────────────────────────┐" << RESET << endl;
    cout << BLUE << "  │ " << YELLOW << value << RESET;
    // Tinh toan khoang cach de ve dau gach dung ben phai cho deu
    int spaces = 29 - value.length();
    for(int i = 0; i < spaces; i++) cout << " ";
    cout << BLUE << "│" << RESET << endl;
    cout << BLUE << "  └──────────────────────────────┘" << RESET << endl;
}

int main() {
	SetConsoleFont();
    EnableANSI(); // Kich hoat mau sac

    double a, b;
    
    DrawHeader();

    // Nhap so a
    cout << endl;
    cout << "  " << BOLD << "Moi ban nhap du lieu:" << RESET << endl << endl;
    cout << "  Nhap so a: ";
    cin >> a;

    // Nhap so b
    cout << "  Nhap so b: ";
    cin >> b;

    // Xu ly tinh toan
    double sum = a + b;
    string resultStr = to_string(a) + " + " + to_string(b) + " = " + to_string(sum);

    // Ve ket qua
    DrawHeader();
    cout << endl;
    DrawBox("KET QUA PHEP TINH:", to_string(sum));
    
    cout << endl << GREEN << "  [Phep tinh]: " << RESET << a << " + " << b << endl;
    cout << endl << YELLOW << "  Nhan phim bat ky de thoat..." << RESET << endl;
    
    system("pause > nul");
    return 0;
}