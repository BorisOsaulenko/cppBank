#include <iostream>
#include <chrono>
#include "./card.hpp"

using namespace std;

#ifndef CREDITCARD
#define CREDITCARD

class CreditCard : public Card
{
private:
    void addBalance(double amount) override;
    double percentage;
    chrono::time_point<chrono::system_clock> deadline;

public:
    CreditCard(Banks bank, double creditLimit, double percentage, chrono::time_point<chrono::system_clock> deadline, string &password);
};

CreditCard::CreditCard(Banks bank, double creditLimit, double percentage, chrono::time_point<chrono::system_clock> deadline, string &password) : Card(bank, Type::CREDIT, password)
{
    this->balance = creditLimit;
    this->percentage = percentage;
    this->deadline = deadline;
};

void CreditCard::addBalance(double amount)
{
    // no credit card add balance logic in the project. Read the readme for details
}

#endif