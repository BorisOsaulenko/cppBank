#include <iostream>
#include <chrono>
#include <type_traits>

using namespace std;

#ifndef EXPENSETYPE
#define EXPENSETYPE

enum ExpenseType
{
    FOOD,
    TRANSPORT,
    CLOTHES,
    JOY,
    OTHER
};

string getExpenseTypeName(ExpenseType type)
{
    if (type == ExpenseType::FOOD)
    {
        return "Food";
    }
    if (type == ExpenseType::TRANSPORT)
    {
        return "Transport";
    }
    if (type == ExpenseType::CLOTHES)
    {
        return "Clothes";
    }
    if (type == ExpenseType::JOY)
    {
        return "Joy";
    }
    return "Other";
};

ostream &operator<<(ostream &os, const ExpenseType &type)
{
    os << getExpenseTypeName(type);
    return os;
};

#endif

#ifndef EXPENSE
#define EXPENSE

class Expense
{
private:
    ExpenseType type;
    double cost;
    chrono::time_point<chrono::system_clock> time;

public:
    Expense(ExpenseType type, double cost);
    chrono::time_point<chrono::system_clock> getTime() const;
    ExpenseType getType();
    double getCost();
    void print();
    friend ostream &operator<<(ostream &os, const Expense &exp);
};

Expense::Expense(ExpenseType type, double cost)
{
    this->type = type;
    this->cost = cost;
    this->time = chrono::system_clock::now();
}

ostream &operator<<(ostream &os, Expense &exp)
{
    time_t now = chrono::system_clock::to_time_t(exp.getTime());
    tm *timeinfo = localtime(&now);
    os << std::put_time(timeinfo, "%Y-%m-%d %H:%M:%S") << " " << exp.getType() << " " << exp.getCost() << endl;
    return os;
}

chrono::time_point<chrono::system_clock> Expense::getTime() const
{
    return time;
};

ExpenseType Expense::getType()
{
    return type;
}

double Expense::getCost()
{
    return cost;
}
#endif