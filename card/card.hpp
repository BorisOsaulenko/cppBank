#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <cstdlib>
#include "../user/user.hpp"

using namespace std;

#ifndef PASSWORD
#define PASSWORD

const string validPasswordChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()-_=+";
const int passwordLength = 20;

#endif

#ifndef TYPE
#define TYPE

enum class Type
{
    CREDIT,
    DEBIT
};

string getTypeName(Type type)
{
    if (type == Type::CREDIT)
    {
        return "Credit";
    }
    if (type == Type::DEBIT)
    {
        return "Debit";
    }

    return "";
}

ostream &operator<<(ostream &os, const Type &type)
{
    os << getTypeName(type);
    return os;
}

#endif

#ifndef BANKS
#define BANKS

enum class Banks
{
    TINKOFF,
    SBERBANK,
    MONOBANK,
};

string getBankName(Banks bank)
{
    if (bank == Banks::TINKOFF)
    {
        return "Tinkoff";
    }
    if (bank == Banks::SBERBANK)
    {
        return "Sberbank";
    }
    if (bank == Banks::MONOBANK)
    {
        return "MonoBank";
    }

    return "";
}

ostream &operator<<(ostream &os, const Banks &bank)
{
    os << getBankName(bank);
    return os;
}

#endif

#ifndef CARD
#define CARD
class Card
{
private:
    void virtual addBalance(double amount) = 0;
    string password;

protected:
    Banks bank;
    Type type;
    double balance;
    chrono::time_point<chrono::system_clock> registrationTime;

public:
    void print();
    Card(Banks bank, Type type, string &password);
    bool pay(string password, double amount);
    void addBalance(string password, double amount);
    bool checkPassword(string password);
    bool isEmpty();
    Type getType();
    Banks getBank();
    chrono::time_point<chrono::system_clock> getRegistrationTime();

    bool operator<(const Card &other);
};

Card::Card(Banks bank, Type type, string &password)
{
    this->bank = bank;
    this->type = type;
    this->registrationTime = chrono::system_clock::now();

    string p = "";
    srand(time(0)); // Seed for random number generator

    for (int i = 0; i < passwordLength; ++i)
    {
        int random_index = rand() % validPasswordChars.size();
        p.push_back(validPasswordChars[random_index]);
    }
    this->password = p;
    password = p;
}

void Card::print()
{
    cout << type << " card" << endl;
    cout << bank << endl;
    time_t now = chrono::system_clock::to_time_t(registrationTime);
    tm *timeinfo = localtime(&now);
    cout << std::put_time(timeinfo, "%Y-%m-%d %H:%M:%S") << endl;
}

Banks Card::getBank()
{
    return bank;
}

Type Card::getType()
{
    return type;
};

bool Card::pay(string password, double amount)
{
    if (this->password != password)
    {
        cout << "Wrong password" << endl;
        return false;
    }
    if (balance < amount)
    {
        cout << "Not enough money" << endl;
        return false;
    }

    balance -= amount;
    cout << "Paid " << amount << " from " << bank << " " << type << " card" << endl;
    return true;
}

void Card::addBalance(string password, double amount)
{
    if (this->password != password)
    {
        cout << "Wrong password" << endl;
        return;
    }
    addBalance(amount);
}

bool Card::checkPassword(string password)
{
    return this->password == password;
}

bool Card::isEmpty()
{
    return balance == 0;
}

bool Card::operator<(const Card &other)
{
    return registrationTime < other.registrationTime;
};

chrono::time_point<chrono::system_clock> Card::getRegistrationTime()
{
    return registrationTime;
}

#endif