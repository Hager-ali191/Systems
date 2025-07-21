#pragma once
#include <iostream>
#include <string>
#include <stdexcept>

using namespace std;

class ATM
{
private:
    std::string currentAccount;
    double balance;
    double cashAvailable;
    bool authenticated;
    std::string currentPIN;

public:
    ATM() : balance(5000), cashAvailable(10000), authenticated(false), currentPIN("1234") {}

    // Basic ATM functions
    void insertCard(const std::string &accountNumber)
    {
        if (accountNumber.empty())
        {
            throw std::invalid_argument("Account number cannot be empty");
        }
        currentAccount = accountNumber;
        authenticated = false;
    }

    bool enterPIN(const std::string &pin)
    {
        if (pin.length() != 4)
        {
            throw std::invalid_argument("PIN must be 4 digits");
        }
        authenticated = (pin == currentPIN);
        return authenticated;
    }

    double checkBalance() const
    {
        if (!authenticated)
        {
            throw std::runtime_error("Please login first");
        }
        return balance;
    }

    void withdraw(double amount)
    {
        if (!authenticated)
        {
            throw std::runtime_error("Please login first");
        }
        if (amount <= 0)
        {
            throw std::invalid_argument("Amount must be positive");
        }
        if (amount > balance)
        {
            throw std::runtime_error("Insufficient funds");
        }
        if (amount > cashAvailable)
        {
            throw std::runtime_error("Not enough cash in ATM");
        }
        balance -= amount;
        cashAvailable -= amount;
    }

    void deposit(double amount)
    {
        if (!authenticated)
        {
            throw std::runtime_error("Please login first");
        }
        if (amount <= 0)
        {
            throw std::invalid_argument("Amount must be positive");
        }
        balance += amount;
        cashAvailable += amount;
    }

    bool changePIN(const std::string &oldPin, const std::string &newPin)
    {
        if (!authenticated)
        {
            throw std::runtime_error("Please login first");
        }
        if (oldPin != currentPIN)
        {
            return false;
        }
        if (newPin.length() != 4)
        {
            throw std::invalid_argument("New PIN must be 4 digits");
        }
        currentPIN = newPin;
        return true;
    }

    void endSession()
    {
        authenticated = false;
        currentAccount = "";
    }

    // Admin functions
    void refillMachine(double amount)
    {
        if (amount <= 0)
        {
            throw std::invalid_argument("Amount must be positive");
        }
        cashAvailable += amount;
    }

    double getCashAvailable() const { return cashAvailable; }
};