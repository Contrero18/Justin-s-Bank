#include <iostream>
#include <string>

class Accounts{
    int id;
    long long int cnp;
    std::string account_number;
    std::string account_type;
    double balance;
    std::string opened_date;
    bool has_card;
public:
    Accounts(const int id = 0, const long long int cnp = 0, const std::string account_number = "",
    const std::string account_type = "", const double balance = 0, const std::string opened_date = "",
    const bool has_card = 0);
    ~Accounts();
    int getId() const;
    long long int getCnp() const;
    std::string getAccount_number() const;
    std::string getAccount_type() const;
    double getBalance() const;
    std::string getOpened_date() const;
    bool getHas_card() const;
    void setId(const int id);
    void setCnp(const int cnp);
    void setAccount_number(const std::string accont_number);
    void setAccount_type(const std::string account_type);
    void setBalance(const double balance);
    void setOpened_date(const std::string opened_date);
    void setHas_card(const bool has_card);
};