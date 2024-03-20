#include <iostream>
#include <chrono>
#include <future>
#include "../wallet/wallet.hpp"
#include "../user/user.hpp"
#include "../card/creditCard.hpp"
#include "../card/debitCard.hpp"
#include "../expense/expense.hpp"

using namespace std;

int main()
{
    string creditPassword1, creditPassword2, debitPassword;
    CreditCard creditCard(Banks::TINKOFF, 1000, 0.01, chrono::system_clock::now() + chrono::seconds(5), creditPassword1);
    CreditCard creditCard2(Banks::TINKOFF, 1000, 0.01, chrono::system_clock::now() + chrono::seconds(5), creditPassword2);
    std::this_thread::sleep_for(chrono::seconds(2));
    DebitCard debitCard(Banks::SBERBANK, debitPassword);

    // cout << creditCard.checkPassword(creditPassword) << endl;
    // cout << debitCard.checkPassword(debitPassword) << endl;

    BankUser user1("Vasya", "12345");
    Wallet wallet1(user1);

    wallet1.addCard(&creditCard, creditPassword1);
    wallet1.addCard(&creditCard2, creditPassword2);
    wallet1.addCard(&debitCard, debitPassword);
    wallet1.print();

    // wallet sorts cards by registration time

    cout << endl;
    wallet1.pay(0, ExpenseType::FOOD, 600);    // pay from credit card 300
    wallet1.pay(1, ExpenseType::CLOTHES, 300); // pay from credit card 300
    wallet1.pay(2, ExpenseType::FOOD, 300);    // pay from credit card 300
    cout << endl;

    creditCard.print();
    cout << endl;
    debitCard.print();

    wallet1.removeCard(0);
    wallet1.removeCard(1);
    wallet1.removeCard(2);

    cout << endl;
    wallet1.print();
    cout << endl;

    for (auto expense : wallet1.getTopCategories(chrono::system_clock::now() - chrono::hours(1), chrono::system_clock::now()))
    {
        cout << expense.first << " " << expense.second << endl;
    }

    wallet1.saveToFile("./demo/wallet.txt");
    return 0;
}