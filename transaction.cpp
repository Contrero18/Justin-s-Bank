#include "transaction.h"

Transaction:: Transaction(const int transaction_id, const int account_id, const int destination_account_id,
const std::string transaction_date, const double amount, const std::string transaction_type) : transaction_id(transaction_id),
account_id(account_id), destination_account_id(destination_account_id), transaction_date(transaction_date), amount(amount),
transaction_type(transaction_type) {} 

Transaction:: ~Transaction(){}

int Transaction:: getTransaction_id() const{
    return transaction_id;
}

int Transaction:: getAccount_id() const{
    return account_id;
}

int Transaction:: getDestination_account_id() const{
    return destination_account_id;
}

std::string Transaction:: getTransaction_date() const{
    return transaction_date;
}

double Transaction:: getAmount() const{
    return amount;
}

std::string Transaction:: getTransaction_type() const{
    return transaction_type;
}

void Transaction:: setTransaction_id(const int transaction_id){
    this -> transaction_id = transaction_id;
}

void Transaction:: setAccount_id(const int account_id){
    this -> account_id = account_id;
}

void Transaction:: setDestination_account_id(const int destination_account_id){
    this -> destination_account_id = destination_account_id;
}

void Transaction:: setTransaction_date(const std::string transaction_date){
    this -> transaction_date = transaction_date;
}

void Transaction:: setAmount(const double amount){
    this -> amount = amount;
}

void Transaction:: setTransaction_type(const std::string transaction_type){
    this -> transaction_type = transaction_type;
}
