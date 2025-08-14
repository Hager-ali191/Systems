#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <iomanip>
#include <limits>
#include <cstdlib> // For system("cls") or system("clear")

using namespace std;

// Cross-platform screen clearing function
void clearScreen() {
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
}

// Account types
enum AccountType { SAVINGS = 1, CURRENT };

// Transaction structure
struct Transaction {
    int id;
    string type;
    double amount;
    time_t timestamp;
    int fromAccount;
    int toAccount;
    string description;
};

// Account structure
struct Account {
    int accountNumber;
    string name;
    AccountType type;
    double balance;
    string password;
    time_t creationDate;
    vector<Transaction> transactions;
};

class OnlineBankingSystem {
private:
    vector<Account> accounts;
    int nextAccountNumber;
    
    Account* findAccount(int accountNumber) {
        for (auto &account : accounts) {
            if (account.accountNumber == accountNumber) {
                return &account;
            }
        }
        return nullptr;
    }
    
    void clearInputBuffer() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    
    string formatTime(time_t time) {
        char buffer[80];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&time));
        return string(buffer);
    }

public:
    OnlineBankingSystem() : nextAccountNumber(1000) {}
    
    int createAccount(string name, AccountType type, string password) {
        Account newAccount;
        newAccount.accountNumber = nextAccountNumber++;
        newAccount.name = name;
        newAccount.type = type;
        newAccount.balance = 0.0;
        newAccount.password = password;
        newAccount.creationDate = time(nullptr);
        
        accounts.push_back(newAccount);
        return newAccount.accountNumber;
    }
    
    bool deposit(int accountNumber, double amount, string description) {
        Account* account = findAccount(accountNumber);
        if (account != nullptr && amount > 0) {
            account->balance += amount;
            
            Transaction t;
            t.id = account->transactions.size() + 1;
            t.type = "DEPOSIT";
            t.amount = amount;
            t.timestamp = time(nullptr);
            t.fromAccount = -1;
            t.toAccount = accountNumber;
            t.description = description;
            
            account->transactions.push_back(t);
            return true;
        }
        return false;
    }
    
    bool withdraw(int accountNumber, double amount, string description) {
        Account* account = findAccount(accountNumber);
        if (account != nullptr && amount > 0 && account->balance >= amount) {
            account->balance -= amount;
            
            Transaction t;
            t.id = account->transactions.size() + 1;
            t.type = "WITHDRAWAL";
            t.amount = amount;
            t.timestamp = time(nullptr);
            t.fromAccount = accountNumber;
            t.toAccount = -1;
            t.description = description;
            
            account->transactions.push_back(t);
            return true;
        }
        return false;
    }
    
    bool transfer(int fromAccount, int toAccount, double amount, string description) {
        Account* sender = findAccount(fromAccount);
        Account* receiver = findAccount(toAccount);
        
        if (sender != nullptr && receiver != nullptr && 
            sender != receiver && amount > 0 && sender->balance >= amount) {
            
            sender->balance -= amount;
            receiver->balance += amount;
            
            Transaction t1;
            t1.id = sender->transactions.size() + 1;
            t1.type = "TRANSFER_OUT";
            t1.amount = amount;
            t1.timestamp = time(nullptr);
            t1.fromAccount = fromAccount;
            t1.toAccount = toAccount;
            t1.description = description;
            sender->transactions.push_back(t1);
            
            Transaction t2;
            t2.id = receiver->transactions.size() + 1;
            t2.type = "TRANSFER_IN";
            t2.amount = amount;
            t2.timestamp = time(nullptr);
            t2.fromAccount = fromAccount;
            t2.toAccount = toAccount;
            t2.description = description;
            receiver->transactions.push_back(t2);
            
            return true;
        }
        return false;
    }
    
    void viewAccount(int accountNumber) {
        Account* account = findAccount(accountNumber);
        if (account != nullptr) {
            cout << "\n----------------------------------------\n";
            cout << "          ACCOUNT STATEMENT\n";
            cout << "----------------------------------------\n";
            cout << left << setw(20) << "Account Number:" << account->accountNumber << endl;
            cout << setw(20) << "Account Holder:" << account->name << endl;
            cout << setw(20) << "Account Type:" << (account->type == SAVINGS ? "Savings" : "Current") << endl;
            cout << setw(20) << "Balance:" << fixed << setprecision(2) << account->balance << " $" << endl;
            cout << setw(20) << "Creation Date:" << formatTime(account->creationDate) << endl;
            cout << "----------------------------------------\n";
            
            if (!account->transactions.empty()) {
                cout << "\nTRANSACTION HISTORY:\n";
                cout << left << setw(8) << "ID" << setw(15) << "Type" 
                     << setw(12) << "Amount" << setw(22) << "Date/Time" 
                     << setw(12) << "From" << setw(12) << "To" << "Description\n";
                cout << "------------------------------------------------------------\n";
                
                for (const auto& t : account->transactions) {
                    cout << setw(8) << t.id << setw(15) << t.type 
                         << setw(12) << fixed << setprecision(2) << t.amount
                         << setw(22) << formatTime(t.timestamp)
                         << setw(12) << (t.fromAccount == -1 ? "N/A" : to_string(t.fromAccount))
                         << setw(12) << (t.toAccount == -1 ? "N/A" : to_string(t.toAccount))
                         << t.description << endl;
                }
                cout << "----------------------------------------\n";
            } else {
                cout << "\nNo transactions found for this account.\n";
            }
        } else {
            cout << "\nError: Account not found!\n";
        }
    }
};

void displayMainMenu() {
    clearScreen();
    cout << "------------------------------------------\n";
    cout << "       ONLINE BANKING SYSTEM - MENU\n";
    cout << "------------------------------------------\n";
    cout << "1. Create New Account\n";
    cout << "2. Deposit Money\n";
    cout << "3. Withdraw Money\n";
    cout << "4. Transfer Money\n";
    cout << "5. View Account Statement\n";
    cout << "6. Exit\n";
    cout << "----------------------------------------\n";
    cout << "Enter your choice (1-6): ";
}

void createAccountUI(OnlineBankingSystem& bank) {
    clearScreen();
    string name, password;
    int type;
    
    cout << "----------------------------------------\n";
    cout << "          CREATE NEW ACCOUNT\n";
    cout << "----------------------------------------\n";
    
    cout << "Enter your full name: ";
    cin.ignore();
    getline(cin, name);
    
    cout << "Enter password: ";
    cin >> password;
    
    cout << "Select account type:\n";
    cout << "1. Savings Account\n";
    cout << "2. Current Account\n";
    cout << "Enter choice (1-2): ";
    cin >> type;
    
    while (type < 1 || type > 2) {
        cout << "Invalid choice. Please enter 1 or 2: ";
        cin >> type;
    }
    
    int accNum = bank.createAccount(name, static_cast<AccountType>(type), password);
    cout << "\nAccount created successfully!\n";
    cout << "Your account number is: " << accNum << endl;
}

void depositUI(OnlineBankingSystem& bank) {
    clearScreen();
    int accountNumber;
    double amount;
    string description;
    
    cout << "----------------------------------------\n";
    cout << "              DEPOSIT MONEY\n";
    cout << "----------------------------------------\n";
    
    cout << "Enter account number: ";
    cin >> accountNumber;
    
    cout << "Enter amount to deposit: $";
    cin >> amount;
    
    cout << "Enter description: ";
    cin.ignore();
    getline(cin, description);
    
    if (bank.deposit(accountNumber, amount, description)) {
        cout << "\nDeposit successful!\n";
    } else {
        cout << "\nDeposit failed. Invalid account or amount.\n";
    }
}

void withdrawUI(OnlineBankingSystem& bank) {
    clearScreen();
    int accountNumber;
    double amount;
    string description;
    
    cout << "----------------------------------------\n";
    cout << "             WITHDRAW MONEY\n";
    cout << "----------------------------------------\n";
    
    cout << "Enter account number: ";
    cin >> accountNumber;
    
    cout << "Enter amount to withdraw: $";
    cin >> amount;
    
    cout << "Enter description: ";
    cin.ignore();
    getline(cin, description);
    
    if (bank.withdraw(accountNumber, amount, description)) {
        cout << "\nWithdrawal successful!\n";
    } else {
        cout << "\nWithdrawal failed. Invalid account, amount or insufficient funds.\n";
    }
}

void transferUI(OnlineBankingSystem& bank) {
    clearScreen();
    int fromAccount, toAccount;
    double amount;
    string description;
    
    cout << "----------------------------------------\n";
    cout << "             TRANSFER MONEY\n";
    cout << "----------------------------------------\n";
    
    cout << "Enter your account number: ";
    cin >> fromAccount;
    
    cout << "Enter recipient account number: ";
    cin >> toAccount;
    
    cout << "Enter amount to transfer: $";
    cin >> amount;
    
    cout << "Enter description: ";
    cin.ignore();
    getline(cin, description);
    
    if (bank.transfer(fromAccount, toAccount, amount, description)) {
        cout << "\nTransfer successful!\n";
    } else {
        cout << "\nTransfer failed. Check account numbers and balance.\n";
    }
}

void viewAccountUI(OnlineBankingSystem& bank) {
    clearScreen();
    int accountNumber;
    
    cout << "----------------------------------------\n";
    cout << "          VIEW ACCOUNT STATEMENT\n";
    cout << "----------------------------------------\n";
    
    cout << "Enter account number: ";
    cin >> accountNumber;
    
    bank.viewAccount(accountNumber);
}

void pauseScreen() {
    cout << "\nPress Enter to continue...";
    cin.ignore();
    cin.get();
}

int main() {
    OnlineBankingSystem bank;
    int choice;
    
    do {
        displayMainMenu();
        
        while (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number (1-6): ";
        }
        
        switch(choice) {
            case 1:
                createAccountUI(bank);
                pauseScreen();
                break;
            case 2:
                depositUI(bank);
                pauseScreen();
                break;
            case 3:
                withdrawUI(bank);
                pauseScreen();
                break;
            case 4:
                transferUI(bank);
                pauseScreen();
                break;
            case 5:
                viewAccountUI(bank);
                pauseScreen();
                break;
            case 6:
                clearScreen();
                cout << "\nThank you for using our banking system!\n";
                break;
            default:
                cout << "Invalid choice. Please enter a number between 1 and 6.\n";
                pauseScreen();
        }
        
    } while (choice != 6);
    
    return 0;
}