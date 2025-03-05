#include "client.h"

Client::Client(const long long int cnp, const std::string first_name, const std::string last_name, const std::string date, 
    const std::string address, const std::string phone, const std::string email) : cnp(cnp), first_name(first_name),
    last_name(last_name), date(date), address(address), phone(phone), email(email) {};


Client:: ~Client(){}

long long int Client:: getCnp() const{
    return cnp;
}

std::string Client:: getFirst_name() const{
    return first_name;
}

std::string Client:: getLast_name() const{
    return last_name;
}

std::string Client:: getDate() const{
    return date;
}

std::string Client:: getAddress() const{
    return address;
}

std::string Client:: getPhone() const{
    return phone;
}

std::string Client:: getEmail() const{
    return email;
}

void Client:: setCnp(const long long int cnp){
    this->cnp = cnp;
}

void Client:: setFirst_name(const std::string first_name){
    this -> first_name = first_name;
}

void Client:: setLast_name(std::string last_name){
    this -> last_name = last_name;
}

void Client:: setDate(std::string date){
    this -> date = date;
}

void Client:: setAddress(std::string address){
    this -> address = address;
}

void Client:: setPhone(std::string phone){
    this -> phone = phone;
}

 void Client:: setEmail(std::string email){
    this -> email = email;
}

