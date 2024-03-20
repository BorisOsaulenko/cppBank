#include <iostream>
#include <string>
#include <vector>

using namespace std;

#ifndef BANKUSER
#define BANKUSER

class BankUser
{
private:
    string name;
    string password;

    static vector<string> takenNames;

public:
    BankUser(string name, string password);
    void print();

    string getName();
    bool checkPassword(string password);
    void setName(string name);
};

vector<string> BankUser::takenNames;

BankUser::BankUser(string name, string password)
{
    if (takenNames.end() == find(takenNames.begin(), takenNames.end(), name))
    {
        this->name = name;
        this->password = password;
        takenNames.push_back(name);
    }
    else
    {
        throw runtime_error("Name already taken");
    }

    this->name = name;
    this->password = password;
};

string BankUser::getName() { return name; }
bool BankUser::checkPassword(string password) { return password == this->password; }
void BankUser::setName(string name) { this->name = name; }

#endif