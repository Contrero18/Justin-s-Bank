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

int numberOfAccounts(sqlite3 * dataBase, long long cnp){
    const char* sql = "SELECT COUNT(*) FROM accounts WHERE cnp = ?";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(dataBase, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(dataBase) << std::endl;
        sqlite3_close(dataBase);
        exit(1);
    }

    sqlite3_bind_int64(stmt, 1, cnp);

    if (sqlite3_step(stmt) != SQLITE_ROW) {
        std::cerr << "Failed to execute query: " << sqlite3_errmsg(dataBase) << std::endl;
    }

    int accountNumber = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);
    return accountNumber;
}

bool checkingCnp(sqlite3 * dataBase, long long cnp){
    std::string sql = "SELECT cnp FROM clients WHERE cnp = ?;";
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(dataBase, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(dataBase) << std::endl;
        sqlite3_close(dataBase);
        exit(1);
    }

    if (sqlite3_bind_int64(stmt, 1, cnp) != SQLITE_OK) {
        std::cerr << "Failed to bind parameter: " << sqlite3_errmsg(dataBase) << std::endl;
        sqlite3_finalize(stmt);
        sqlite3_close(dataBase);
        exit(1);
    }

    bool available = false;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        available = true;
    }

    sqlite3_finalize(stmt);
    return available;
}

std::string accountType(){
    char key;
    int colors[] = {11, 11};
    int position = 1;

    for(int i=8; i<=10; i++){ //delete previous menu options
        gotoxy(6, i);
        for(int j=0; j<25;j++){
            std::cout<<" ";
        }
    }

    while(1){

        if(position == 1){
            colors[0] = 5;
        }

        if(position == 2){
            colors[1] = 5;
        }

        gotoxy(6,8);
        color(colors[0]);
        std::cout<<"1. Checking";

        gotoxy(6,9);
        color(colors[1]);
        std::cout<<"2. Savings";

        key = _getch();

        if(key == 72 && (position ==2)){ // up arrow
            position--;
        }

        if(key == 80 && (position ==1)){ // down arrow
            position++;
        }

        if(key == '\r'){ // enter
            if(position == 1){
                return "Checking";
            }

            if(position == 2){
                return "Savings";
            }   

        }
        
        colors[0] = 11;
        colors[1] = 11;
    }
}

std::string date(){
    time_t now = time(0);
    
    tm* localTime = localtime(&now);

    return std::to_string(1900 + localTime->tm_year) + "-" + std::to_string(1 + localTime->tm_mon) + "-" + std::to_string(localTime->tm_mday);
}

bool card(){
    char key;
    int colors[] = {11, 11};
    int position = 1;

    for(int i=8; i<=10; i++){ //delete previous menu options
        gotoxy(6, i);
        for(int j=0; j<25;j++){
            std::cout<<" ";
        }
    }

    gotoxy(6,8);
    color(colors[0]);
    std::cout<<"Do you want a card?";

    while(1){

        if(position == 1){
            colors[0] = 5;
        }

        if(position == 2){
            colors[1] = 5;
        }

        gotoxy(6,10);
        color(colors[0]);
        std::cout<<"1. Yes";

        gotoxy(6,11);
        color(colors[1]);
        std::cout<<"2. No";

        key = _getch();

        if(key == 72 && (position ==2)){ // up arrow
            position--;
        }

        if(key == 80 && (position ==1)){ // down arrow
            position++;
        }

        if(key == '\r'){ // enter
            if(position == 1){
                return 1;
            }

            if(position == 2){
                return 0;
            }   

        }
        
        colors[0] = 11;
        colors[1] = 11;
    }
}

void newAccount(sqlite3 * dataBase, long long cnp){
    int accountsNumber = numberOfAccounts(dataBase, cnp);

    const char* sql = "INSERT INTO accounts (cnp, account_number, account_type, balance, opened_date, has_card) "
                      "VALUES (?, ?, ?, ?, ?, ?)";
    
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(dataBase, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(dataBase) << std::endl;
        sqlite3_close(dataBase);
        exit(1);
    }

    sqlite3_bind_int64(stmt, 1, cnp); // cnp
    sqlite3_bind_text(stmt, 2, ("ACCT_" + std::to_string(accountsNumber + 1)).c_str(), -1, SQLITE_STATIC); // account_number

    sqlite3_bind_text(stmt, 3, accountType().c_str() , -1, SQLITE_STATIC); // account_type

    sqlite3_bind_double(stmt, 4, 0); // balance

    sqlite3_bind_text(stmt, 5, date().c_str() , -1, SQLITE_STATIC); // opened_date
    
    sqlite3_bind_int(stmt, 6, card()); // has_card

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Execution failed: " << sqlite3_errmsg(dataBase) << std::endl;
    }
    
    sqlite3_finalize(stmt);
}

void newClient(sqlite3 * dataBase){

}

void typeCnp(sqlite3 * dataBase, std::string sentFrom);



void wrongCnp(sqlite3 * dataBase, std::string sentFrom){
    showCursor(false);
    for(int i=8; i<=10; i++){ //delete previous menu options
        gotoxy(6, i);
        for(int j=0; j<25;j++){
            std::cout<<" ";
        }
    }
    gotoxy(6,8);
    std::cout << "CNP INVALID ";
    //menu fro invalid
    int colors[] = {11, 11, 11, 11};
    char key;
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

        if(position == 4){
            colors[3] = 5;
        }
        

        gotoxy(6,10);
        color(colors[0]);
        std::cout<<"1. Type Cnp Again";

        gotoxy(6,11);
        color(colors[1]);
        std::cout<<"2. Add new client";

        gotoxy(6,12);
        color(colors[2]);
        std::cout<<"3. Go Back";

        gotoxy(6,13);
        color(colors[3]);
        std::cout<<"4. Exit";

        key = _getch();

        if(key == 72 && (position >=2 && position <=4)){ // up arrow
            position--;
        }

        if(key == 80 && (position >=1 && position <=3)){ // down arrow
            position++;
        }

        if(key == '\r'){ // enter
            if(position == 1){
                typeCnp(dataBase, sentFrom);
            }

            if(position == 2){
                newClient(dataBase);
            }  

            if(position == 3){
                menu:: firstMenu(dataBase);
            }   

            if(position == 4){
                exit(0);
            }   
        }
        
        colors[0] = 11;
        colors[1] = 11;
        colors[2] = 11;
        colors[3] = 11;
    }
}

void typeCnp(sqlite3 * dataBase, std::string sentFrom){
    for(int i=8; i<=12; i++){ //delete previous menu options
        gotoxy(6, i);
        for(int j=0; j<25;j++){
            std::cout<<" ";
        }
    }
    gotoxy(6,8);
    std::cout << "Enter CNP: ";
    showCursor(true);
    long long int cnp;
    if(!(std::cin>>cnp)){   //checking if cnp is valid
        wrongCnp(dataBase, sentFrom);
    }
    long long aux=cnp;
    int nr = 0;
    while(aux){
        aux=aux/10;
        nr++;
    }
    if(nr != 13){
        wrongCnp(dataBase, sentFrom);
    }
    showCursor(false);

    bool ok=checkingCnp(dataBase, cnp);

    if(ok == 1 && sentFrom == "newAccount")
    {
        newAccount(dataBase, cnp);
    }
    
}

void accountsOption(sqlite3 * dataBase){
    int colors[] = {11, 11, 11};
    char key;
    int position = 1;

    for(int i=8; i<=10; i++){ //delete previous menu options
        gotoxy(6, i);
        for(int j=0; j<25;j++){
            std::cout<<" ";
        }
    }

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
        std::cout<<"1. Type Cnp";

        gotoxy(6,9);
        color(colors[1]);
        std::cout<<"2. Go Back";

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
                typeCnp(dataBase, "accounts");
            }

            if(position == 2){
                menu:: firstMenu(dataBase);
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

void menu:: firstMenu(sqlite3 * dataBase){
    char key;
    int colors[] = {11, 11, 11};
    int position = 1;

    for(int i=8; i<=10; i++){ //delete previous menu options
        gotoxy(6, i);
        for(int j=0; j<25;j++){
            std::cout<<" ";
        }
    }

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
                typeCnp(dataBase, "newAccount");
            }

            if(position == 2){
                accountsOption(dataBase);
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