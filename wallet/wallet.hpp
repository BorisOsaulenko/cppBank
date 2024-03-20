#include <iostream>
#include "../user/user.hpp"
#include "../card/card.hpp"
#include "../expense/expense.hpp"
#include <vector>
#include <algorithm>
#include <fstream>
#include <chrono>
#include <map>

using namespace std;

#ifndef WALLET
#define WALLET

struct CardComparator
{
    bool operator()(Card *lhs, Card *rhs) const
    {
        return lhs->getRegistrationTime() < rhs->getRegistrationTime();
    }
}; // cards are sorted by registration time

class Wallet
{
private:
    BankUser user;
    map<Card *, string, CardComparator> cards; // wallet is saving all your passwords
    vector<Banks> banksUsed;
    vector<Expense> expenses;

public:
    Wallet(BankUser bankUser);
    void addCard(Card *card, string password);
    void removeCard(int cardId);
    void print();
    void pay(int cardId, ExpenseType type, double amount);
    vector<Expense> getExpenses();
    vector<Expense> getExpenses(chrono::time_point<chrono::system_clock> startTime, chrono::time_point<chrono::system_clock> endTime);
    vector<Expense> getExpenses(chrono::time_point<chrono::system_clock> startTime);
    vector<Expense> getExpenses(ExpenseType type);
    vector<Expense> getTopExpences(chrono::time_point<chrono::system_clock> startTime, chrono::time_point<chrono::system_clock> endTime);
    vector<Expense> getTopExpences(chrono::time_point<chrono::system_clock> startTime);
    vector<pair<ExpenseType, double>> getTopCategories(chrono::time_point<chrono::system_clock> startTime, chrono::time_point<chrono::system_clock> endTime);
    vector<pair<ExpenseType, double>> getTopCategories(chrono::time_point<chrono::system_clock> startTime);
    void printExpenses();
    void addBalance(int cardId, double amount);

    void saveToFile(string fileName);
};

Wallet::Wallet(BankUser bankUser) : user(bankUser), cards(CardComparator())
{
    this->user = bankUser;
};

void Wallet::addCard(Card *card, string password)
{
    if (!card->checkPassword(password))
    {
        cout << "Wrong password" << endl;
        return;
    }
    if (cards.find(card) != cards.end())
    {
        cout << "Card already exists" << endl;
        return;
    }
    cards[card] = password;
    banksUsed.push_back(card->getBank());
}

void Wallet::removeCard(int cardId)
{
    if (cardId < 0 || cardId >= cards.size())
    {
        cout << "Wrong card id" << endl;
        return;
    }

    Card *c = next(cards.begin(), cardId)->first;
    if (!c->isEmpty())
    {
        cout << "Card is not empty. Finish your expenses" << endl;
        return;
    }
    int amountOfCardsWithSameBank = count_if(cards.begin(), cards.end(), [c](pair<Card *, string> card)
                                             { return card.first->getBank() == c->getBank(); });
    if (amountOfCardsWithSameBank == 1)
    {
        banksUsed.erase(find(banksUsed.begin(), banksUsed.end(), c->getBank()));
    }
    cards.erase(next(cards.begin(), cardId));
    cout << c->getBank() << " " << c->getType() << " card removed" << endl;
}

void Wallet::print()
{
    cout << user.getName() << "'s wallet" << endl;
    cout << "Cards: " << cards.size() << endl;
};

void Wallet::pay(int cardId, ExpenseType type, double amount)
{
    if (cardId < 0 || cardId >= cards.size())
    {
        cout << "Wrong card id" << endl;
        return;
    }

    Card *c = next(cards.begin(), cardId)->first;
    bool success = c->pay(cards[c], amount);

    if (success)
        expenses.push_back(Expense(type, amount));
};

vector<Expense> Wallet::getExpenses()
{
    return expenses;
}

void Wallet::printExpenses()
{
    cout << "Expenses:" << endl;
    for (Expense expense : expenses)
    {
        cout << expense;
    }
}

void Wallet::addBalance(int cardId, double amount)
{
    if (cardId < 0 || cardId >= cards.size())
    {
        throw runtime_error("Card not found");
    }
    Card *c = next(cards.begin(), cardId)->first;
    c->addBalance(cards[c], amount);
    cout << "Added " << amount << " to " << c->getBank() << " " << c->getType() << "card" << endl;
};

vector<Expense> Wallet::getExpenses(chrono::time_point<chrono::system_clock> startTime, chrono::time_point<chrono::system_clock> endTime)
{
    vector<Expense> result;
    copy_if(expenses.begin(), expenses.end(), back_inserter(result), [startTime, endTime](Expense expense)
            { return expense.getTime() >= startTime && expense.getTime() <= endTime; });
    return result;
}

vector<Expense> Wallet::getExpenses(chrono::time_point<chrono::system_clock> startTime)
{
    return getExpenses(startTime, chrono::system_clock::now());
}

vector<Expense> Wallet::getExpenses(ExpenseType type)
{
    vector<Expense> result;
    copy_if(expenses.begin(), expenses.end(), back_inserter(result), [type](Expense expense)
            { return expense.getType() == type; });
    return result;
}

vector<Expense> Wallet::getTopExpences(chrono::time_point<chrono::system_clock> startTime, chrono::time_point<chrono::system_clock> endTime)
{
    vector<Expense> result = getExpenses(startTime, endTime);
    sort(result.begin(), result.end(), [](Expense a, Expense b)
         { return a.getCost() > b.getCost(); });
    return result;
}

vector<Expense> Wallet::getTopExpences(chrono::time_point<chrono::system_clock> startTime)
{
    return getTopExpences(startTime, chrono::system_clock::now());
}

vector<pair<ExpenseType, double>> Wallet::getTopCategories(chrono::time_point<chrono::system_clock> startTime, chrono::time_point<chrono::system_clock> endTime)
{
    vector<Expense> expenses = getExpenses(startTime, endTime);

    map<ExpenseType, double> temp;
    for (Expense expense : expenses)
    {
        temp[expense.getType()] += expense.getCost();
    }

    vector<pair<ExpenseType, double>> result(temp.begin(), temp.end());
    sort(result.begin(), result.end(), [](pair<ExpenseType, double> a, pair<ExpenseType, double> b)
         { return a.second > b.second; });

    return result;
}

vector<pair<ExpenseType, double>> Wallet::getTopCategories(chrono::time_point<chrono::system_clock> startTime)
{
    return getTopCategories(startTime, chrono::system_clock::now());
};

void Wallet::saveToFile(string fileName)
{
    ofstream file;
    file.open(fileName);

    file << user.getName() << "'s wallet: " << endl
         << endl;
    file << "Cards:\n";
    for (auto card : cards)
    {
        file << card.first->getBank() << " " << card.first->getType() << " card" << endl;
    }
    file << "\nExpenses:" << endl;
    for (auto expense : expenses)
    {
        file << expense;
    }
    file.close();
};

#endif