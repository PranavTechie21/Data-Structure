#include <iostream>
#include <cstring> // For C-style string functions like strcmp
#include <string>

using namespace std;

// Class as specified in the manual
class Dictionary {
public:
    // Data Members
    char name[10];
    long int phone;
    bool occupied;

    // Member Functions
    void init_Table() {
        strcpy(name, "--");
        phone = 0;
        occupied = false;
    }
};

class HashTable {
private:
    static const int TABLE_SIZE = 10; // As per the manual's example HT[10]
    Dictionary HT[TABLE_SIZE];

    // Hash Function as specified in the manual
    int hashFunction(const string &key) {
        int sum = 0;
        for (char ch : key) {
            sum += tolower(ch);
        }
        return sum % TABLE_SIZE;
    }

public:
    // To insert default values in Hash Table
    void init_Table() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            HT[i].init_Table();
        }
        cout << "Hash Table initialized successfully.\n";
    }

    // To insert records in Hash Table
    bool insertRecords(const string &client_name, long int telephone) {
        int index = hashFunction(client_name);

        for (int i = 0; i < TABLE_SIZE; i++) {
            int tryIndex = (index + i) % TABLE_SIZE;

            if (!HT[tryIndex].occupied) {
                strcpy(HT[tryIndex].name, client_name.c_str());
                HT[tryIndex].phone = telephone;
                HT[tryIndex].occupied = true;
                cout << "Client inserted at index " << tryIndex << ".\n";
                return true;
            }
        }
        cout << "Hash Table is full! Cannot insert " << client_name << endl;
        return false;
    }

    // To search a client's telephone in Hash Table
    long int look_up(const string &target_name) {
        int index = hashFunction(target_name);

        for (int i = 0; i < TABLE_SIZE; i++) {
            int tryIndex = (index + i) % TABLE_SIZE;
            if (HT[tryIndex].occupied && strcmp(HT[tryIndex].name, target_name.c_str()) == 0) {
                return HT[tryIndex].phone;
            }
            if (!HT[tryIndex].occupied && HT[tryIndex].phone == 0) {
                // This condition assumes empty slots mean the end of the probe chain.
                // It works for initial insertion but lazy deletion requires a more complex check.
                return -1;
            }
        }
        return -1; // Not found
    }

    // To display Hash Table
    void display_Table() {
        cout << "\n--- Hash Table Contents ---\n";
        for (int i = 0; i < TABLE_SIZE; i++) {
            cout << "Index " << i << ": ";
            if (HT[i].occupied) {
                cout << "Name = " << HT[i].name << ", Phone = " << HT[i].phone << endl;
            } else {
                cout << "Empty\n";
            }
        }
    }
};

int main() {
    HashTable ht;
    ht.init_Table();

    int choice;
    string name;
    long int phone;

    do {
        cout << "\n===== TELEPHONE BOOK MENU =====\n";
        cout << "1. Insert a client record\n";
        cout << "2. Look-up a client's phone number\n";
        cout << "3. Display Hash Table\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter client name: ";
                cin >> name;
                cout << "Enter telephone number: ";
                cin >> phone;
                ht.insertRecords(name, phone);
                break;
            case 2:
                cout << "Enter client name to search: ";
                cin >> name;
                phone = ht.look_up(name);
                if (phone != -1) {
                    cout << "The phone number for " << name << " is " << phone << ".\n";
                } else {
                    cout << "Client not found.\n";
                }
                break;
            case 3:
                ht.display_Table();
                break;
            case 4:
                cout << "Exiting program. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 4);
    cout<<string(50,' ')<<"Thankyou for using Company Telephone Database "<<endl;
    return 0;
}