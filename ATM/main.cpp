#include <iostream>
#include <iomanip>
#include <limits>
#include <cstdlib> // for system()
#include "ATM.hpp"
#include "ATMCustomer.hpp"
#include "ATMAdmin.hpp"

using namespace std;

// Cross-platform clear screen
void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Display header with ATM name
void displayHeader()
{
    cout << "========================================\n";
    cout << "|      WELCOME TO BANK OF DEVELOPERS   |\n";
    cout << "========================================\n\n";
}

// Centered text display
void centerText(const string &text)
{
    int width = 40;
    int padding = (width - text.length()) / 2;
    cout << string(padding, ' ') << text << endl;
}

// Draw a box around text
void boxedText(const string &text)
{
    cout << "+--------------------------------------+\n";
    cout << "| " << setw(36) << left << text << " |\n";
    cout << "+--------------------------------------+\n";
}

// Display a progress bar
void showProgressBar(int seconds)
{
    cout << "\nProcessing: [";
    for (int i = 0; i < 20; i++)
    {
        cout << ".";
        cout.flush();
// Sleep for a fraction of the total time
#ifdef _WIN32
        _sleep(seconds * 1000 / 20);
#else
        usleep(seconds * 1000000 / 20);
#endif
    }
    cout << "] Done!\n";
}

void runCustomerSession(ATM &atm)
{
    ATMCustomer customer(atm, "123456789", "1234");

    string card, pin;
    clearScreen();
    displayHeader();
    centerText("CUSTOMER LOGIN");
    cout << "\n";
    boxedText("Please enter your card number:");
    cout << ">> ";
    cin >> card;

    boxedText("Please enter your 4-digit PIN:");
    cout << ">> ";
    cin >> pin;

    if (customer.login(card, pin))
    {
        showProgressBar(2);

        int choice;
        do
        {
            clearScreen();
            displayHeader();
            centerText("MAIN MENU");
            cout << "\n";
            customer.showMenu();

            cout << "\nEnter your choice (1-5): ";
            while (!(cin >> choice) || choice < 1 || choice > 5)
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter a number between 1-5: ";
            }

            clearScreen();
            displayHeader();
            switch (choice)
            {
            case 1:
                centerText("BALANCE INQUIRY");
                break;
            case 2:
                centerText("CASH WITHDRAWAL");
                break;
            case 3:
                centerText("CASH DEPOSIT");
                break;
            case 4:
                centerText("CHANGE PIN");
                break;
            case 5:
                centerText("END TRANSACTION");
                break;
            }
            cout << "\n";

            customer.performAction(choice);

            if (choice != 5)
            {
                cout << "\nPress Enter to return to menu...";
                cin.ignore();
                cin.get();
            }
        } while (choice != 5);
    }
    else
    {
        cout << "\n";
        boxedText("LOGIN FAILED - Invalid credentials");
        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
    }
}

void runAdminSession(ATM &atm)
{
    ATMAdmin admin(atm, "admin", "admin123");

    string uname, pwd;
    clearScreen();
    displayHeader();
    centerText("ADMINISTRATOR LOGIN");
    cout << "\n";
    boxedText("Enter admin username:");
    cout << ">> ";
    cin >> uname;

    boxedText("Enter admin password:");
    cout << ">> ";
    cin >> pwd;

    if (admin.login(uname, pwd))
    {
        showProgressBar(2);

        int choice;
        do
        {
            clearScreen();
            displayHeader();
            centerText("ADMIN MENU");
            cout << "\n";
            admin.showMenu();

            cout << "\nEnter your choice (1-4): ";
            while (!(cin >> choice) || choice < 1 || choice > 4)
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter a number between 1-4: ";
            }

            clearScreen();
            displayHeader();
            switch (choice)
            {
            case 1:
                centerText("REFILL CASH");
                break;
            case 2:
                centerText("VIEW ATM CASH");
                break;
            case 3:
                centerText("MAINTENANCE");
                break;
            case 4:
                centerText("LOGOUT");
                break;
            }
            cout << "\n";

            admin.performAction(choice);

            if (choice != 4)
            {
                cout << "\nPress Enter to return to menu...";
                cin.ignore();
                cin.get();
            }
        } while (choice != 4);
    }
    else
    {
        cout << "\n";
        boxedText("LOGIN FAILED - Invalid credentials");
        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
    }
}

int main()
{
    ATM atm;

    while (true)
    {
        clearScreen();
        displayHeader();
        centerText("PLEASE SELECT USER TYPE");
        cout << "\n";
        boxedText("1. Customer");
        boxedText("2. Administrator");
        boxedText("3. Exit");

        cout << "\nEnter your choice (1-3): ";

        int userType;
        while (!(cin >> userType) || userType < 1 || userType > 3)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number between 1-3: ";
        }

        switch (userType)
        {
        case 1:
            runCustomerSession(atm);
            break;
        case 2:
            runAdminSession(atm);
            break;
        case 3:
            clearScreen();
            displayHeader();
            centerText("THANK YOU FOR USING");
            centerText("BANK OF DEVELOPERS ATM");
            cout << "\n";
            boxedText("Goodbye!");
            cout << "\n";
            return 0;
        }
    }
    return 0;
}