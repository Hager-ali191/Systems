#pragma once
#include "User.hpp"
#include "ATM.hpp"
#include <iostream>

class ATMAdmin : public User
{
private:
    ATM &atm;

public:
    ATMAdmin(ATM &atmMachine, const std::string &uname, const std::string &pwd)
        : User(uname, pwd), atm(atmMachine) {}

    bool login(const std::string &uname, const std::string &pwd) override
    {
        return (username == uname && password == pwd);
    }

    void showMenu() override
    {
        std::cout << "\n===== Admin Menu =====" << std::endl;
        std::cout << "1. Refill Cash" << std::endl;
        std::cout << "2. View ATM Cash" << std::endl;
        std::cout << "3. Perform Maintenance" << std::endl;
        std::cout << "4. Logout" << std::endl;
    }

    void performAction(int choice) override
    {
        try
        {
            switch (choice)
            {
            case 1:
            {
                double amount;
                std::cout << "Enter amount to refill: ";
                std::cin >> amount;
                atm.refillMachine(amount);
                std::cout << "Refill successful. Current cash: " << atm.getCashAvailable() << std::endl;
                break;
            }
            case 2:
            {
                std::cout << "ATM Cash Available: " << atm.getCashAvailable() << std::endl;
                break;
            }
            case 3:
            {
                std::cout << "Performing maintenance..." << std::endl;
                // Simulate maintenance
                for (int i = 0; i < 3; ++i)
                {
                    std::cout << ".";
                    std::cout.flush();
                    // Simulate delay
                    for (int j = 0; j < 100000000; ++j)
                    {
                    }
                }
                std::cout << "\nMaintenance completed" << std::endl;
                break;
            }
            case 4:
            {
                std::cout << "Logged out from admin system" << std::endl;
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