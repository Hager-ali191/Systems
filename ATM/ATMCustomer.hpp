#pragma once
#include "User.hpp"
#include "ATM.hpp"
#include <iostream>

class ATMCustomer : public User
{
private:
    ATM &atm;
    std::string cardNumber;

public:
    ATMCustomer(ATM &atmMachine, const std::string &card, const std::string &pin)
        : User(card, pin), atm(atmMachine), cardNumber(card) {}

    bool login(const std::string &card, const std::string &pin) override
    {
        try
        {
            atm.insertCard(card);
            return atm.enterPIN(pin);
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error: " << e.what() << std::endl;
            return false;
        }
    }

    void showMenu() override {
        cout << "\n+--------------------------------------+\n";
        cout << "|           CUSTOMER MENU             |\n";
        cout << "+--------------------------------------+\n";
        
        cout << " 1. "; cout << "Check Balance\n";
        cout << " 2. "; cout << "Withdraw Cash\n";
        cout << " 3. "; cout << "Deposit Cash\n";
        cout << " 4. "; cout << "Change PIN\n";
        cout << " 5. "; cout << "End Transaction\n";
        cout << "+--------------------------------------+\n";
}

    void performAction(int choice) override
    {
        try
        {
            switch (choice)
            {
            case 1:
            {
                std::cout << "Current Balance: " << atm.checkBalance() << std::endl;
                break;
            }
            case 2:
            {
                double amount;
                std::cout << "Enter amount to withdraw: ";
                std::cin >> amount;
                atm.withdraw(amount);
                std::cout << "Withdrawal successful. Remaining balance: " << atm.checkBalance() << std::endl;
                break;
            }
            case 3:
            {
                double amount;
                std::cout << "Enter amount to deposit: ";
                std::cin >> amount;
                atm.deposit(amount);
                std::cout << "Deposit successful. New balance: " << atm.checkBalance() << std::endl;
                break;
            }
            case 4:
            {
                std::string oldPin, newPin;
                std::cout << "Enter current PIN: ";
                std::cin >> oldPin;
                std::cout << "Enter new PIN: ";
                std::cin >> newPin;
                if (atm.changePIN(oldPin, newPin))
                {
                    std::cout << "PIN changed successfully" << std::endl;
                }
                else
                {
                    std::cout << "Failed to change PIN. Current PIN is incorrect" << std::endl;
                }
                break;
            }
            case 5:
            {
                atm.endSession();
                std::cout << "Transaction ended. Thank you for using our ATM." << std::endl;
                break;
            }
            default:
                std::cout << "Invalid choice!" << std::endl;
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
};