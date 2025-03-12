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

void typeCnp(sqlite3 * dataBase, std::string sentFrom);

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

std::string dateOfBirth(long long cnp){
    int day, month, year;
    long long aux = cnp;
    if(aux / 1000000000000 == 1 || aux / 1000000000000 == 2){
        year = 19;
    }else{
        year = 20;
    }
    aux = cnp;
    year = year * 100 + (aux / 10000000000) % 100;
    aux = cnp;
    month = (aux / 100000000) % 100;
    day = (cnp / 1000000 ) % 100;
    
    std::ostringstream dateStream;
    dateStream << year << "-" 
           << std::setw(2) << std::setfill('0') << month << "-"
           << std::setw(2) << std::setfill('0') << day;

    return dateStream.str();
}

bool rightData(){
    gotoxy(6,16);

    std::cout << "Is the data correct?";
    
    int colors[] = {11, 11};
    char key;
    int position = 1;

    while(1){

        if(position == 1){
            colors[0] = 5;
        }

        if(position == 2){
            colors[1] = 5;
        }
        

        gotoxy(6,18);
        color(colors[0]);
        std::cout<<"1. Yes";

        gotoxy(6,19);
        color(colors[1]);
        std::cout<<"2. No";

        key = _getch();

        if(key == 72 && (position == 2)){ // up arrow
            position--;
        }

        if(key == 80 && (position ==1)){ // down arrow
            position++;
        }

        if(key == '\r'){ // enter
            if(position == 1){
                return true;
            }

            if(position == 2){
                return false;
            }  
        }
        
        colors[0] = 11;
        colors[1] = 11;
    }
}

void tryAgain(sqlite3 * dataBase){
    gotoxy(6,16);

    std::cout << "Do you want to try again?";
    
    int colors[] = {11, 11};
    char key;
    int position = 1;

    while(1){

        if(position == 1){
            colors[0] = 5;
        }

        if(position == 2){
            colors[1] = 5;
        }
        

        gotoxy(6,18);
        color(colors[0]);
        std::cout<<"1. Yes";

        gotoxy(6,19);
        color(colors[1]);
        std::cout<<"2. No";

        key = _getch();

        if(key == 72 && (position == 2)){ // up arrow
            position--;
        }

        if(key == 80 && (position ==1)){ // down arrow
            position++;
        }

        if(key == '\r'){ // enter
            if(position == 1){
                for(int i=8; i<=20; i++){ //delete previous menu options
                    gotoxy(0, i);
                    for(int j=0; j<63;j++){
                        std::cout<<" ";
                    }
                }
                typeCnp(dataBase, "newAccount");
            }

            if(position == 2){
                for(int i=8; i<=20; i++){ //delete previous menu options
                    gotoxy(0, i);
                    for(int j=0; j<63;j++){
                        std::cout<<" ";
                    }
                }
                menu:: firstMenu(dataBase);
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

void addClient(sqlite3 * dataBase, Client *& client){
    const char* sql = "INSERT INTO clients (cnp, first_name, last_name, date_of_birth, address, phone, email) "
                      "VALUES (?, ?, ?, ?, ?, ?, ?)";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(dataBase, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(dataBase) << std::endl;
        sqlite3_close(dataBase);
        exit(1);
    }

    sqlite3_bind_int64(stmt, 1, client->getCnp());// cnp
    sqlite3_bind_text(stmt, 2, client->getFirst_name().c_str(), -1, SQLITE_STATIC);// first_name
    sqlite3_bind_text(stmt, 3, client->getFirst_name().c_str(), -1, SQLITE_STATIC);// last_name
    sqlite3_bind_text(stmt, 4, client->getDate().c_str(), -1, SQLITE_STATIC);// date_of_birth
    sqlite3_bind_text(stmt, 5, client->getAddress().c_str(), -1, SQLITE_STATIC); // address
    sqlite3_bind_text(stmt, 6, client->getPhone().c_str(), -1, SQLITE_STATIC);// phone
    sqlite3_bind_text(stmt, 7, client->getEmail().c_str(), -1, SQLITE_STATIC); // email

    for(int i=8; i<=20; i++){ //delete previous menu options
        gotoxy(0, i);
        for(int j=0; j<63;j++){
            std::cout<<" ";
        }
    }

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Execution failed: " << sqlite3_errmsg(dataBase) << std::endl;
        exit(1);
    } else {
        gotoxy(6, 8);
        std::cout << "Client added successfully." << std::endl;
        gotoxy(6, 9);
        Sleep(700);
        std::cout << "Return to main menu in 3";
        Sleep(700);
        gotoxy(6, 9);
        std::cout << "Return to main menu in 2";
        Sleep(700);
        gotoxy(6, 9);
        std::cout << "Return to main menu in 1";
        Sleep(300);
        menu:: firstMenu(dataBase);
    }

    sqlite3_finalize(stmt);
}

void newClient(sqlite3 * dataBase, long long cnp){
    for(int i=8; i<=13; i++){ //delete previous menu options
        gotoxy(0, i);
        for(int j=0; j<25;j++){
            std::cout<<" ";
        }
    }
    gotoxy(6,8);
    std::cout << "Would you like to proceed with registering a new client?";
    
    int colors[] = {11, 11};
    char key;
    int position = 1;

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

        if(key == 72 && (position == 2)){ // up arrow
            position--;
        }

        if(key == 80 && (position ==1)){ // down arrow
            position++;
        }

        if(key == '\r'){ // enter
            if(position == 1){
                break;
            }

            if(position == 2){
                menu:: firstMenu(dataBase);
            }  
        }
        
        colors[0] = 11;
        colors[1] = 11;
    }

    for(int i=8; i<=13; i++){ //delete previous menu options
        gotoxy(0, i);
        for(int j=0; j<63;j++){
            std::cout<<" ";
        }
    }

    std::string firstName, lastName, address, phone, email;

    showCursor(true);
    gotoxy(6,8);
    std::cout<<"Cnp: "<<cnp;

    gotoxy(6,9);
    std::cout<<"First name: "; std::cin>>firstName;

    gotoxy(6,10);
    std::cout<<"Last name: "; std::cin>>lastName;

    gotoxy(6,11);
    std::string date = dateOfBirth(cnp);
    std::cout<<"Date of birth: " << date;

    gotoxy(6,12);
    std::cin.ignore();
    std::cout<<"Address: "; std::getline(std::cin, address);

    gotoxy(6,13);
    std::cout<<"Phone: "; std::cin>>phone;

    gotoxy(6,14);
    std::cout<<"Email: "; std::cin>>email;

    showCursor(false);

    bool ok = rightData();

    if(ok == false){
        tryAgain(dataBase);
    }else{
        Client *client = new Client(cnp, firstName, lastName, date, address, phone, email);
        addClient(dataBase, client);
        delete client;
    }
    
}

void wrongCnp(sqlite3 * dataBase, std::string sentFrom, long long cnp){
    showCursor(false);
    for(int i=8; i<=13; i++){ //delete previous menu options
        gotoxy(0, i);
        for(int j=0; j<25;j++){
            std::cout<<" ";
        }
    }
    gotoxy(6,8);
    std::cout << "Invalid CNP format! Please enter a valid 13-digit numeric CNP. ";
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
                typeCnp(dataBase, sentFrom);
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
    for(int i=8; i<=13; i++){ //delete previous menu options
        gotoxy(6, i);
        for(int j=0; j<63;j++){
            std::cout<<" ";
        }
    }
    gotoxy(6,8);
    std::cout << "Enter CNP: ";
    showCursor(true);
    long long int cnp;
    if(!(std::cin>>cnp)){   //checking if cnp is valid
        wrongCnp(dataBase, sentFrom, cnp);
    }
    long long aux=cnp;
    int nr = 0;
    while(aux){
        aux=aux/10;
        nr++;
    }
    if(nr != 13){
        wrongCnp(dataBase, sentFrom, cnp);
    }
    showCursor(false);

    bool ok=checkingCnp(dataBase, cnp);

    if(ok == 0){
        newClient(dataBase, cnp);
    }

    if(ok == 1 && sentFrom == "newAccount")
    {
        newAccount(dataBase, cnp);
    }
    
}

void accountsOption(sqlite3 * dataBase){
    int colors[] = {11, 11, 11};
    char key;
    int position = 1;

    for(int i=8; i<=13; i++){ //delete previous menu options
        gotoxy(6, i);
        for(int j=0; j<63;j++){
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

    for(int i=8; i<=13; i++){ //delete previous menu options
        gotoxy(6, i);
        for(int j=0; j<63;j++){
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