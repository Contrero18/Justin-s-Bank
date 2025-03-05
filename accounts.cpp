#include "accounts.h"

Accounts:: Accounts(const int id, const long long int cnp, const std::string account_number,
const std::string account_type , const double balance, const std::string opened_date,
const bool has_card ) : id(id), cnp(cnp), account_number(account_number), account_type(account_type),
balance(balance), opened_date(opened_date){}

Accounts:: ~Accounts(){}

int Accounts:: getId() const{
    return id;
}

long long int Accounts:: getCnp() const{
    return cnp;
}

std::string Accounts:: getAccount_number() const{
    return account_number;
}

std::string Accounts:: getAccount_type() const{
    return account_type;
}

double Accounts:: getBalance() const{
    return balance;
}

std::string Accounts:: getOpened_date() const{
    return opened_date;
}

bool Accounts:: getHas_card() const{
    return has_card;
}

void Accounts:: setId(const int id){
    this -> id = id;
}

void Accounts:: setCnp(const int cnp){
    this -> cnp = cnp;
}

void Accounts:: setAccount_number(const std::string account_number){
    this -> account_number = account_number;
}

void Accounts:: setAccount_type(const std::string account_type){
    this -> account_type = account_type;
}

void Accounts:: setBalance(const double balance){
    this -> balance = balance;
}

void Accounts:: setOpened_date(const std::string opened_date){
    this -> opened_date = opened_date;
}

void Accounts:: setHas_card(const bool has_card){
    this -> has_card = has_card;
}
