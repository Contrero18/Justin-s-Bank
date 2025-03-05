#include <iostream>
#include <string>

class Transaction{
    int transaction_id;
    int account_id;
    int destination_account_id;
    std::string transaction_date;
    double amount;
    std::string transaction_type;
public:
    Transaction(const int transaction_id = 0, const int account_id = 0, const int destination_account_id = 0,
    const std::string transaction_date = "", const double amount = 0, const std::string transaction_type = "");
    ~Transaction();
    int getTransaction_id() const;
    int getAccount_id() const;
    int getDestination_account_id() const;
    std::string getTransaction_date() const;
    double getAmount() const;
    std::string getTransaction_type() const;
    void setTransaction_id(const int transaction_id);
    void setAccount_id(const int account_id);
    void setDestination_account_id(const int destination_account_id);
    void setTransaction_date(const std::string transaction_date);
    void setAmount(const double amount);
    void setTransaction_type(const std::string transaction_type);
};