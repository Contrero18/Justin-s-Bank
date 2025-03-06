#include "menu.h"
#include <sqlite3.h>

int main(int argc, char* argv[]){
    sqlite3* dataBase;
    int verify = sqlite3_open(argv[1], &dataBase);
    if (verify != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(dataBase) << std::endl;
        exit(1);
    }
    menu::bankName();
    menu::firstMenu(dataBase);
    
    return 0;
}