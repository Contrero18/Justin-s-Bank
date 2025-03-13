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
void transactionMenu(sqlite3 * dataBase, int id, double balance);
void newTransaction(sqlite3 * dataBase, int id, double balance);

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

std::string accountType(sqlite3 * dataBase){
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
        std::cout<<"1. Checking";

        gotoxy(6,9);
        color(colors[1]);
        std::cout<<"2. Savings";

        gotoxy(6,10);
        color(colors[2]);
        std::cout<<"3. Go To Main Menu";

        key = _getch();

        if(key == 72 && (position >=2 && position <=3)){ // up arrow
            position--;
        }

        if(key == 80 && (position >=1 && position <=2)){ // down arrow
            position++;
        }

        if(key == '\r'){ // enter
            if(position == 1){
                return "Checking";
            }

            if(position == 2){
                return "Savings";
            }   
            if(position == 3){
                menu:: firstMenu(dataBase);
            }
        }
        
        colors[0] = 11;
        colors[1] = 11;
        colors[2] = 11;
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
                for(int i=10; i<=11; i++){ //delete previous menu options
                    gotoxy(6, i);
                    for(int j=0; j<25;j++){
                        std::cout<<" ";
                    }
                }
                return 1;
            }

            if(position == 2){
                for(int i=10; i<=11; i++){ //delete previous menu options
                    gotoxy(6, i);
                    for(int j=0; j<25;j++){
                        std::cout<<" ";
                    }
                }
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

    sqlite3_bind_text(stmt, 3, accountType(dataBase).c_str() , -1, SQLITE_STATIC); // account_type

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
    delete client;
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
    }
    
}

void cardVerification(sqlite3 * dataBase, int id, int balance){
    sqlite3_stmt* stmt;
    std::string sql = "SELECT has_card FROM accounts WHERE account_id = ?;";
    if ( sqlite3_prepare_v2(dataBase, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Preparation failed: " << sqlite3_errmsg(dataBase) << std::endl;
        exit(1);
    } else {
        sqlite3_bind_int(stmt, 1, id);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            int hasCard = sqlite3_column_int(stmt, 0);
            if (hasCard == 0) {
                for(int i=8; i<=20; i++){ //delete previous menu options
                    gotoxy(0, i);
                    for(int j=0; j<63;j++){
                        std::cout<<" ";
                    }
                }
                
                gotoxy(6, 8);
                std::cout << "The account doesn't have a card!" << std::endl;
                gotoxy(6, 9);
                Sleep(700);
                std::cout << "Returning in 3";
                Sleep(700);
                gotoxy(6, 9);
                std::cout << "Returning in 2";
                Sleep(700);
                gotoxy(6, 9);
                std::cout << "Returning in 1";
                Sleep(300);
                sqlite3_finalize(stmt);
                newTransaction(dataBase, id, balance);
            } 
        }
    }
}

void seeTransactions(sqlite3 * dataBase, int id, double balance){
    for(int i=8; i<=14; i++){ //delete previous menu options
        gotoxy(0, i);
        for(int j=0; j<100;j++){
            std::cout<<" ";
        }
    }
    const char* sql = "SELECT transaction_id, account_id, destination_account_id, transaction_date, amount, transaction_type "
                      "FROM transactions WHERE account_id = ?";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(dataBase, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(dataBase) << std::endl;
        sqlite3_close(dataBase);
        exit(1);
    }

    sqlite3_bind_int(stmt, 1, id);
    int y = 8;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int transaction_id = sqlite3_column_int(stmt, 0);
        int account_id = sqlite3_column_int(stmt, 1);
        
        // Check if destination_account_id is NULL
        bool has_destination = (sqlite3_column_type(stmt, 2) != SQLITE_NULL);
        int destination_account_id = 0;
        if (has_destination) {
            destination_account_id = sqlite3_column_int(stmt, 2);
        }
        
        const unsigned char* transaction_date = sqlite3_column_text(stmt, 3);
        double amount = sqlite3_column_double(stmt, 4);
        const unsigned char* transaction_type = sqlite3_column_text(stmt, 5);
        gotoxy(6,y++);
        std::cout << "Transaction ID: " << transaction_id;
        gotoxy(6,y++);
        std::cout << "Account ID: " << account_id;
        gotoxy(6,y++);
        if (has_destination) {
            std::cout << "Destination Account ID: " << destination_account_id;
        }
        gotoxy(6,y++);
        std::cout << "Transaction Date: " << transaction_date;
        gotoxy(6,y++);
        std::cout << "Amount: " << amount << "\n";
        gotoxy(6,y++);
        std::cout << "Transaction Type: " << transaction_type;
        gotoxy(6,y++);
        std::cout << "---------------------------" << std::endl;
        y+=2;
    }

    sqlite3_finalize(stmt);

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
        

        gotoxy(6,y);
        color(colors[0]);
        std::cout<<"1. Go Back";

        gotoxy(6,y+1);
        color(colors[1]);
        std::cout<<"2. Go To Main Menu";

        key = _getch();

        if(key == 72 && (position == 2)){ // up arrow
            position--;
        }

        if(key == 80 && (position ==1)){ // down arrow
            position++;
        }

        if(key == '\r'){ // enter
            if(position == 1){
                for(int i=8; i<=y+1; i++){ //delete previous menu options
                    gotoxy(0, i);
                    for(int j=0; j<150;j++){
                        std::cout<<" ";
                    }
                }
                transactionMenu(dataBase, id, balance);
            }

            if(position == 2){
                for(int i=8; i<=y+1; i++){ //delete previous menu options
                    gotoxy(0, i);
                    for(int j=0; j<150;j++){
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

void transfer(sqlite3 * dataBase, int id, double amount){
    double current_balance, new_balance;
    sqlite3_stmt* stmt;
    std::string select_sql = "SELECT balance FROM accounts WHERE account_id = ?;";
    if (sqlite3_prepare_v2(dataBase, select_sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare SELECT statement: " << sqlite3_errmsg(dataBase) << std::endl;
    } else {
        sqlite3_bind_int(stmt, 1, id);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            current_balance = sqlite3_column_double(stmt, 0);
            new_balance = current_balance + amount;
            sqlite3_finalize(stmt);
        } else {
            std::cerr << "No account found with the given ID." << std::endl;
            menu:: firstMenu(dataBase);
            sqlite3_finalize(stmt);
        }
    }

    std::string update_sql = "UPDATE accounts SET balance = ? WHERE account_id = ?;";

    if (sqlite3_prepare_v2(dataBase, update_sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare UPDATE statement: " << sqlite3_errmsg(dataBase) << std::endl;
    } else {
        sqlite3_bind_double(stmt, 1, new_balance);
        sqlite3_bind_int(stmt, 2, id);
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cerr << "Error updating balance: " << sqlite3_errmsg(dataBase) << std::endl;
            exit(1);
        } 
        sqlite3_finalize(stmt);
    }


}

void transaction(sqlite3 * dataBase, int id, double amount, std::string transactionType, double balance){
    int colors[] = {11, 11};
    char key;
    int position = 1;
    int sendingTo, x=8;
    if(transactionType == "Transfer"){
        gotoxy(6,8);
        std::cout<<"Please Type Destination Account: ";
        showCursor(true);
        std::cin >> sendingTo;
        showCursor(false);
    }
    for(int i=8; i<=10; i++){ //delete previous menu options
        gotoxy(0, i);
        for(int j=0; j<63;j++){
            std::cout<<" ";
        }
    }
    gotoxy(6,x++);
    std::cout << "Account ID: "<<id;
    gotoxy(6,x++);
    std::cout << "Amount: "<<amount;
    gotoxy(6,x++);
    std::cout << "Transaction type: "<< transactionType;
    gotoxy(6, x++);
    if(transactionType == "Transfer"){
        std::cout<<"Destination Account: "<< sendingTo;
    }
    gotoxy(6,x++);
    std::cout << "------------------------- ";
    gotoxy(6,x+3);
    std::cout << "Do you confirm transaction? ";
    while(1){

        if(position == 1){
            colors[0] = 5;
        }

        if(position == 2){
            colors[1] = 5;
        }
        

        gotoxy(6,x+4);
        color(colors[0]);
        std::cout<<"1. Yes";

        gotoxy(6,x+5);
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
                for(int i=8; i<=20; i++){ //delete previous menu options
                    gotoxy(0, i);
                    for(int j=0; j<63;j++){
                        std::cout<<" ";
                    }
                }
                
                gotoxy(6, 9);
                Sleep(700);
                std::cout << "Returning in 3";
                Sleep(700);
                gotoxy(6, 9);
                std::cout << "Returning in 2";
                Sleep(700);
                gotoxy(6, 9);
                std::cout << "Returning in 1";
                Sleep(300);
                newTransaction(dataBase, id, balance);
            }  
        }
        
        colors[0] = 11;
        colors[1] = 11;
    }
    //modifying the balance of the account

    const char* sql = "UPDATE accounts SET balance = ? WHERE account_id = ?";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(dataBase, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(dataBase) << std::endl;
        sqlite3_close(dataBase);
        exit(1);
    }
    double newBalance;
    if(transactionType == "Deposit"){
        newBalance = balance + amount;
    }else{
        newBalance = balance - amount;
    }
    
    sqlite3_bind_double(stmt, 1, newBalance); //new balance
    sqlite3_bind_int(stmt, 2, id);//account id 
    
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Error updating account balance: " << sqlite3_errmsg(dataBase) << std::endl;
    }
    sqlite3_finalize(stmt);

    if(transactionType == "Transfer"){
        transfer(dataBase, sendingTo, amount);
    }

    //adding transaction to db
    const char* sql2 = "INSERT INTO transactions (account_id, destination_account_id, transaction_date, amount, transaction_type) "
                      "VALUES (?, ?, ?, ?, ?)";
    if (sqlite3_prepare_v2(dataBase, sql2, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(dataBase) << std::endl;
        sqlite3_close(dataBase);
        exit(1);
    }

    sqlite3_bind_int(stmt, 1, id);// account_id
    sqlite3_bind_null(stmt, 2);// destination_account_id
    sqlite3_bind_text(stmt, 3, date().c_str(), -1, SQLITE_STATIC);// transaction_date
    sqlite3_bind_double(stmt, 4, amount);// amount
    sqlite3_bind_text(stmt, 5, transactionType.c_str(), -1, SQLITE_STATIC);      // transaction_type

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Error inserting transaction: " << sqlite3_errmsg(dataBase) << std::endl;
    } else {
        for(int i=8; i<=20; i++){ //delete previous menu options
            gotoxy(0, i);
            for(int j=0; j<63;j++){
                std::cout<<" ";
            }
        }
        gotoxy(6, 8);
        std::cout << "Transaction made successfully." << std::endl;
    }

    sqlite3_finalize(stmt);


    gotoxy(6, 9);
    Sleep(700);
    std::cout << "Returning in 3";
    Sleep(700);
    gotoxy(6, 9);
    std::cout << "Returning in 2";
    Sleep(700);
    gotoxy(6, 9);
    std::cout << "Returning in 1";
    Sleep(300);
    newTransaction(dataBase, id, newBalance);
}

void newTransaction(sqlite3 * dataBase, int id, double balance){
    char key;
    int colors[] = {11, 11, 11, 11, 11};
    int position = 1;

    for(int i=8; i<=13; i++){ //delete previous menu options
        gotoxy(6, i);
        for(int j=0; j<100;j++){
            std::cout<<" ";
        }
    }
    std::string transactionType;
    gotoxy(6,8);
    std::cout<<"Select transaction type:";
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

        if(position == 5){
            colors[4] = 5;
        }

        gotoxy(6,9);
        color(colors[0]);
        std::cout<<"1. Deposit";

        gotoxy(6,10);
        color(colors[1]);
        std::cout<<"2. Withdrawal";

        gotoxy(6,11);
        color(colors[2]);
        std::cout<<"3. Card";

        gotoxy(6,12);
        color(colors[3]);
        std::cout<<"4. Transfer";

        gotoxy(6,13);
        color(colors[4]);
        std::cout<<"5. Go back";

        key = _getch();

        if(key == 72 && (position >=2 && position <=5)){ // up arrow
            position--;
        }

        if(key == 80 && (position >=1 && position <=4)){ // down arrow
            position++;
        }

        if(key == '\r'){ // enter
            if(position == 1){
                transactionType = "Deposit";
                break;
            }

            if(position == 2){
                transactionType = "Withdrawal";
                break;
            }   

            if(position == 3){
                cardVerification(dataBase, id, balance);
                transactionType = "Card";
                break;
            }   

            if(position == 4){
                transactionType = "Transfer";
                break;
            }  

            if(position == 5){
                transactionMenu(dataBase, id, balance);
            }
        }
        
        colors[0] = 11;
        colors[1] = 11;
        colors[2] = 11;
        colors[3] = 11;
        colors[4] = 11;
    }

    showCursor(true);
    for(int i=8; i<=20; i++){ //delete previous menu options
        gotoxy(0, i);
        for(int j=0; j<63;j++){
            std::cout<<" ";
        }
    }
    gotoxy(6, 8);
    double amount;
    std::cout << "Type Amount: "; std::cin >> amount;
    showCursor(false);
    if(transactionType == "Withdrawal" || transactionType == "Card" || transactionType == "Transfer" ){
        if(amount > balance){
            for(int i=8; i<=20; i++){ //delete previous menu options
                gotoxy(0, i);
                for(int j=0; j<63;j++){
                    std::cout<<" ";
                }
            }
            
            gotoxy(6, 8);
            std::cout << "You don't have enough balance!" << std::endl;
            gotoxy(6, 9);
            Sleep(700);
            std::cout << "Returning in 3";
            Sleep(700);
            gotoxy(6, 9);
            std::cout << "Returning in 2";
            Sleep(700);
            gotoxy(6, 9);
            std::cout << "Returning in 1";
            Sleep(300);
            newTransaction(dataBase, id, balance);
        }
    }

    for(int i=8; i<=20; i++){ //delete previous menu options
        gotoxy(0, i);
        for(int j=0; j<63;j++){
            std::cout<<" ";
        }
    }

    transaction(dataBase, id, amount, transactionType, balance);

}

void transactionMenu(sqlite3 * dataBase, int id, double balance){
    char key;
    int colors[] = {11, 11, 11};
    int position = 1;

    for(int i=8; i<=25; i++){ //delete previous menu options
        gotoxy(0, i);
        for(int j=0; j<200;j++){
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
        std::cout<<"1. See Transactions";

        gotoxy(6,9);
        color(colors[1]);
        std::cout<<"2. Make A Transaction";

        gotoxy(6,10);
        color(colors[2]);
        std::cout<<"3. Go To Main Menu";

        key = _getch();

        if(key == 72 && (position >=2 && position <=3)){ // up arrow
            position--;
        }

        if(key == 80 && (position >=1 && position <=2)){ // down arrow
            position++;
        }

        if(key == '\r'){ // enter
            if(position == 1){
                seeTransactions(dataBase, id, balance);
            }

            if(position == 2){
                newTransaction(dataBase, id, balance);
            }   

            if(position == 3){
                menu:: firstMenu(dataBase);
            }   
        }
        
        colors[0] = 11;
        colors[1] = 11;
        colors[2] = 11; 
    }
}

void accounts(sqlite3 * dataBase, long long cnp){
    int accountsNumber = numberOfAccounts(dataBase, cnp);

    Accounts* accounts = new Accounts[accountsNumber];

    const char* sql = "SELECT account_id, cnp, account_number, account_type, balance, opened_date, has_card "
                      "FROM accounts WHERE cnp = ?";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(dataBase, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(dataBase) << std::endl;
        sqlite3_close(dataBase);
        exit(1);
    }

    sqlite3_bind_int64(stmt, 1, cnp);

    int i = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        accounts[i].setId(sqlite3_column_int(stmt, 0));
        accounts[i].setCnp(sqlite3_column_int64(stmt, 1));
        accounts[i].setAccount_number(std::string((const char*)sqlite3_column_text(stmt, 2)));
        accounts[i].setAccount_type(std::string((const char*)sqlite3_column_text(stmt, 3)));
        accounts[i].setBalance(sqlite3_column_double(stmt, 4));
        accounts[i].setOpened_date(std::string((const char*)sqlite3_column_text(stmt, 5)));
        accounts[i].setHas_card(sqlite3_column_int(stmt, 6));
        i++;
    }

    sqlite3_finalize(stmt);

    //the menu
    char key;
    int position = 1;
    int * colors = new int[accountsNumber + 1];
    for(int i = 0; i < accountsNumber + 1; i++){
        colors[i] = 11;
    }
    while(1){
        for(int i = 0; i < accountsNumber + 1; i++){
            gotoxy(6, 8+i);
            if(position == i + 1){
                colors[i] = 5;
            }else{
                colors[i] = 11;
            }
            color(colors[i]);
            if(i != accountsNumber){
                std::cout << i+1 <<". " << accounts[i].getId() << " " << accounts[i].getAccount_number() <<
                " " << accounts[i].getAccount_type() << " " << accounts[i].getBalance() <<"$ ";
                if(accounts[i].getHas_card() == 0){
                    std::cout << "WITHOUT CARD";
                }else{
                    std::cout << "CARD";
                }
            }else{
                std::cout << "Back to menu";
            }
        }

        key = _getch();

        if (key == 72 && (position >= 2 && position <= accountsNumber + 1) ) { // Up arrow
            position--;
        }
        if (key == 80 && (position >= 1 && position <= accountsNumber)) { // Down arrow
            position++;
        }

        if(key == '\r') {
            if(position >= 1 && position <= accountsNumber){
                transactionMenu(dataBase, accounts[position - 1].getId(), accounts[position - 1].getBalance());
            }
            if(position == accountsNumber + 1){
                menu:: firstMenu(dataBase);
            }
        }
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
        std::cin.clear();
        std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
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

    if(ok == 1 && sentFrom == "accounts"){
        accounts(dataBase, cnp);
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

    for(int i=8; i<=20; i++){ //delete previous menu options
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