#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <string>
#include <sqlite3.h>
#include <ctime>
#include "client.h"
#include "accounts.h"
#include "transaction.h"

namespace menu{
    void bankName();
    void firstMenu(sqlite3 * dataBase);
}