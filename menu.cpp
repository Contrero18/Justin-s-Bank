#include "menu.h"

void color(int color){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void gotoxy(int x, int y){
    COORD c;
    c.X=x;
    c.Y=y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void showCursor(bool visible) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = visible;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

void menu:: bankName(){
    system("cls");
    showCursor(false);
    gotoxy(3, 1);
    color(5);
    std::cout<<"      _           _   _         _         ____              _    \n";
    gotoxy(3, 2);
    std::cout<<"     | |_   _ ___| |_(_)_ __   ( )___    | __ )  __ _ _ __ | | __\n";
    gotoxy(3, 3);
    std::cout<<"  _  | | | | / __| __| | '_ \\  |// __|   |  _ \\ / _` | '_ \\| |/ /\n";
    gotoxy(3, 4);
    std::cout<<" | |_| | |_| \\__ \\ |_| | | | |   \\__ \\   | |_) | (_| | | | |   < \n";
    gotoxy(3, 5);
    std::cout<<"  \\___/ \\__,_|___/\\__|_|_| |_|   |___/   |____/ \\__,_|_| |_|_|\\_\\\n";
}

void menu:: firstMenu(){
    char key;
    int colors[] = {11, 11, 11};
    int position = 1;
    while(1){

        if(position == 1){
            colors[0] = 5;
        }

        if(position == 2){
            colors[1] = 5;
        }

        if(position == 3){
            colors[2] = 5;
        }

        gotoxy(6,8);
        color(colors[0]);
        std::cout<<"1. Create New Account";

        gotoxy(6,9);
        color(colors[1]);
        std::cout<<"2. Accounts";

        gotoxy(6,10);
        color(colors[2]);
        std::cout<<"3. Exit";

        key = _getch();

        if(key == 72 && (position >=2 && position <=3)){ // up arrow
            position--;
        }

        if(key == 80 && (position >=1 && position <=2)){ // down arrow
            position++;
        }

        if(key == '\r'){ // enter
            if(position == 1){
                std::cout<<"Nih=gga1";
            }

            if(position == 2){
                std::cout<<"Nih=gga2";
            }   

            if(position == 3){
                exit(0);
            }   
        }
        
        colors[0] = 11;
        colors[1] = 11;
        colors[2] = 11; 
    }
}