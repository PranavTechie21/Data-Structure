#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <cctype>
#include <algorithm>
#include <stdexcept>

using namespace std;

class TelephoneBook
{
private:
    struct Record
    {
        string name;
        unsigned long long phone;
        bool occupied = false;
    };

    static const int TABLE_SIZE = 100;
    Record table[TABLE_SIZE];
    int count;

    int hash(const string &key) const
    {
        int sum = 0;
        for (char c : key)
        {
            sum += c;
        }
        return sum % TABLE_SIZE;
    }

    int findIndex(const string &name) const
    {
        int index = hash(name);
        for (int i = 0; i < TABLE_SIZE; i++)
        {
            int tryIndex = (index + i) % TABLE_SIZE;
            if (table[tryIndex].occupied && table[tryIndex].name == name)
            {
                return tryIndex;
            }
            if (!table[tryIndex].occupied)
            {
                break;
            }
        }
        return -1;
    }

    bool isValidName(const string &name) const
    {
        if (name.empty() || name.length() > 50)
            return false;
        return all_of(name.begin(), name.end(), [](char c)
                      { return isalpha(c) || isspace(c); });
    }

public:
    TelephoneBook() : count(0) {}

    bool insertRecord(const string &name, unsigned long long phone)
    {
        if (!isValidName(name))
        {
            cout << "Error: Invalid name. Use only letters and spaces, max 50 characters.\n";
            return false;
        }
        if (!isValidPhoneNumber(phone))
        {
            cout << "Error: Invalid phone number. Must be exactly 10 digits.\n";
            return false;
        }
        if (count >= TABLE_SIZE)
        {
            cout << "Error: Telephone book is full.\n";
            return false;
        }

        int index = hash(name);
        for (int i = 0; i < TABLE_SIZE; i++)
        {
            int tryIndex = (index + i) % TABLE_SIZE;
            if (!table[tryIndex].occupied)
            {
                table[tryIndex] = {name, phone, true};
                count++;
                cout << "Record for " << name << " inserted successfully.\n";
                return true;
            }
            if (table[tryIndex].name == name)
            {
                cout << "Error: A record with this name already exists.\n";
                return false;
            }
        }
        cout << "Error: Could not insert record.\n";
        return false;
    }

    bool lookUp(const string &name)
    {
        if (name.empty())
        {
            cout << "Error: Name cannot be empty.\n";
            return false;
        }
        int index = hash(name);
        for (int i = 0; i < TABLE_SIZE; i++)
        {
            int tryIndex = (index + i) % TABLE_SIZE;
            if (table[tryIndex].occupied && table[tryIndex].name == name)
            {
                cout << "Phone number for " << name << " is " << table[tryIndex].phone << ".\n";
                return true;
            }
            if (!table[tryIndex].occupied)
            {
                break;
            }
        }
        cout << "No record found for " << name << ".\n";
        return false;
    }

    bool deleteRecord(const string &name)
    {
        if (name.empty())
        {
            cout << "Error: Name cannot be empty.\n";
            return false;
        }
        int index = hash(name);
        for (int i = 0; i < TABLE_SIZE; i++)
        {
            int tryIndex = (index + i) % TABLE_SIZE;
            if (table[tryIndex].occupied && table[tryIndex].name == name)
            {
                table[tryIndex].occupied = false;
                count--;
                cout << "Record for " << name << " deleted successfully.\n";
                return true;
            }
            if (!table[tryIndex].occupied)
            {
                break;
            }
        }
        cout << "No record found for " << name << ".\n";
        return false;
    }

    void displayAll()
    {
        cout << "\n--- Telephone Book Contents ---\n";
        bool empty = true;
        for (const auto &record : table)
        {
            if (record.occupied)
            {
                cout << "Name: " << record.name << ", Phone: " << record.phone << "\n";
                empty = false;
            }
        }
        if (empty)
        {
            cout << "The telephone book is empty.\n";
        }
        cout << "Total records: " << count << "\n";
    }

    bool isValidPhoneNumber(unsigned long long phone) const
    {
        return phone > 0 && to_string(phone).length() == 10;
    }

    bool isValidPhoneNumber(const string &phone) const
    {
        if (phone.length() != 10)
            return false;
        return all_of(phone.begin(), phone.end(), ::isdigit);
    }
};

void clearInputBuffer()
{
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

string getValidName()
{
    string name;
    do
    {
        cout << "Enter name (letters and spaces only, max 50 characters): ";
        getline(cin, name);
        if (!all_of(name.begin(), name.end(), [](char c)
                    { return isalpha(c) || isspace(c); }))
        {
            cout << "Invalid name. Use only letters and spaces.\n";
        }
        else if (name.length() > 50)
        {
            cout << "Name too long. Maximum 50 characters.\n";
        }
    } while (!all_of(name.begin(), name.end(), [](char c)
                     { return isalpha(c) || isspace(c); }) ||
             name.length() > 50);
    return name;
}

int main()
{
    TelephoneBook book;
    int choice;
    string name;
    string phone;

    do
    {
        cout << "\n===== TELEPHONE BOOK MENU =====\n"
             << "1. Insert a record\n"
             << "2. Look-up a phone number\n"
             << "3. Delete a record\n"
             << "4. Display all records\n"
             << "5. Exit\n"
             << "Enter your choice: ";

        if (!(cin >> choice))
        {
            cout << "Invalid input. Please enter a number.\n";
            clearInputBuffer();
            continue;
        }

        clearInputBuffer();

        switch (choice)
        {
        case 1:
            name = getValidName();
            cout << "Enter phone number (10 digits): ";
            getline(cin, phone);
            if (!book.isValidPhoneNumber(phone))
            {
                cout << "Invalid phone number. Must be exactly 10 digits.\n";
                break;
            }
            try
            {
                unsigned long long phoneNum = stoull(phone);
                book.insertRecord(name, phoneNum);
            }
            catch (const exception &e)
            {
                cout << "Error converting phone number: " << e.what() << endl;
            }
            break;
        case 2:
            name = getValidName();
            book.lookUp(name);
            break;
        case 3:
            name = getValidName();
            book.deleteRecord(name);
            break;
        case 4:
            book.displayAll();
            break;
        case 5:
            cout << "Exiting program. Goodbye!\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 5);

    cout << string(50, ' ') << "Thank you for using the Telephone Book\n";
    return 0;
}
