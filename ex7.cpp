#include <iostream>
#include <stdexcept>
#include <string>
#include <limits>
using namespace std;

// Base custom exception class
class BankException : public exception
{
protected:
    string message;

public:
    BankException(string msg) : message(msg) {}

    const char* what() const noexcept override
    {
        return message.c_str();
    }
};

class InsufficientFundsException : public BankException
{
public:
    InsufficientFundsException()
        : BankException("Error: Insufficient balance for withdrawal.") {}
};

class InvalidAmountException : public BankException
{
public:
    InvalidAmountException()
        : BankException("Error: Amount must be greater than zero.") {}
};

class BankAccount
{
private:
    string name;
    double balance;

public:
    BankAccount(string n, double b) : name(n), balance(b) {}

    void deposit(double amount)
    {
        if (amount <= 0)
            throw InvalidAmountException();
        balance += amount;
    }

    void withdraw(double amount)
    {
        // Validate first - strong exception guarantee
        if (amount <= 0)
            throw InvalidAmountException();
        if (amount > balance)
            throw InsufficientFundsException();

        balance -= amount;
    }

    void display() const
    {
        cout << "Account Holder: " << name << endl;
        cout << "Balance: Rs. " << balance << endl;
    }
};

double readAmount(const string& prompt)
{
    double amt;
    cout << prompt;
    while (!(cin >> amt))
    {
        cout << "Please enter a valid number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return amt;
}

int main()
{
    string name;
    cout << "Enter account holder name: ";
    cin >> name;

    double initial = readAmount("Enter initial balance: Rs. ");
    if (initial < 0)
    {
        cout << "Initial balance cannot be negative. Setting to 0.\n";
        initial = 0;
    }

    BankAccount account(name, initial);

    cout << "\nInitial Account Details:" << endl;
    account.display();

    int choice;
    do
    {
        cout << "\n--- BANK MENU ---\n";
        cout << "1. Deposit\n2. Withdraw\n3. Display\n4. Exit\n";
        cout << "Enter your choice: ";
        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            choice = 0;
        }

        try
        {
            switch (choice)
            {
            case 1:
                account.deposit(readAmount("Enter deposit amount: Rs. "));
                cout << "Deposit successful." << endl;
                break;
            case 2:
                account.withdraw(readAmount("Enter withdrawal amount: Rs. "));
                cout << "Withdrawal successful." << endl;
                break;
            case 3:
                account.display();
                break;
            case 4:
                cout << "Thank you!" << endl;
                break;
            default:
                cout << "Invalid choice." << endl;
            }
        }
        catch (const InsufficientFundsException &e)
        {
            cout << e.what() << endl;
        }
        catch (const InvalidAmountException &e)
        {
            cout << e.what() << endl;
        }
        catch (const BankException &e)
        {
            cout << "Bank Exception: " << e.what() << endl;
        }
        catch (const exception &e)
        {
            cout << "General Exception: " << e.what() << endl;
        }
    } while (choice != 4);

    cout << "\nFinal Account Details:" << endl;
    account.display();

    return 0;
}