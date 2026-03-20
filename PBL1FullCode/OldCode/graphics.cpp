#include <iostream>
#include <conio.h>
#include <windows.h>

using namespace std;

string menuItems[] = {
    "Add Student",
    "Show Student List",
    "Search Student",
    "Exit"
};

const int MENU_SIZE = 4;

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void drawMenu(int selected) {

    system("cls");

    cout << "==============================\n";
    cout << "      STUDENT MANAGEMENT      \n";
    cout << "==============================\n\n";

    for(int i = 0; i < MENU_SIZE; i++) {

        if(i == selected) {
            setColor(240); // highlight
            cout << " > " << menuItems[i] << endl;
            setColor(15);
        }
        else {
            cout << "   " << menuItems[i] << endl;
        }

    }

    cout << "\nUse UP/DOWN and ENTER\n";
}

int main() {

    int selected = 0;

    while(true) {

        drawMenu(selected);

        int key = _getch();

        if(key == 224) { // special key
            key = _getch();

            if(key == 72) { // UP
                selected--;
                if(selected < 0) selected = MENU_SIZE - 1;
            }

            if(key == 80) { // DOWN
                selected++;
                if(selected >= MENU_SIZE) selected = 0;
            }
        }

        else if(key == 13) { // ENTER

            system("cls");

            cout << "You selected: " << menuItems[selected] << endl;

            if(selected == 3) break;

            system("pause");
        }

    }

    return 0;
}