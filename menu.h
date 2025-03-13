#include <iostream>
#include <limits>
#include <Windows.h>
#include <conio.h>
#include <string>
#include <sqlite3.h>
#include <ctime>
#include <sstream>
#include <iomanip>
#include "client.h"
#include "accounts.h"
#include "transaction.h"

namespace menu{
    void bankName();
    void firstMenu(sqlite3 * dataBase);
}