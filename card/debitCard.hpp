#include <iostream>
#include <string>
#include "./card.hpp"

using namespace std;

#ifndef DEBITCARD
#define DEBITCARD

class DebitCard : public Card
{
private:
    void addBalance(double amount) override;

public:
    DebitCard(Banks bank, string &password);
};

DebitCard::DebitCard(Banks bank, string &password) : Card(bank, Type::DEBIT, password)
{
    this->balance = 0;
};

void DebitCard::addBalance(double amount)
{
    balance += amount;
}

#endif