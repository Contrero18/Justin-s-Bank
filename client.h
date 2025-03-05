#include <iostream>
#include <string>

class Client{
    long long int cnp;
    std::string first_name;
    std::string last_name;
    std::string date;
    std::string address;
    std::string phone;
    std::string email;
public:
    Client(const long long int cnp = 0, const std::string first_name = "", const std::string last_name = "",
    const std::string date = "", const std::string address = "", const std::string phone = "",
    const std::string email = "" );
    ~Client();
    long long int getCnp() const;
    std::string getFirst_name() const;
    std::string getLast_name() const;
    std::string getDate() const;
    std::string getAddress() const;
    std::string getPhone() const;
    std::string getEmail() const;
    void setCnp(const long long int cnp);
    void setFirst_name(const std::string first_name);
    void setLast_name(std::string last_name);
    void setDate(std::string date);
    void setAddress(std::string address);
    void setPhone(std::string phone);
    void setEmail(std::string email);
};