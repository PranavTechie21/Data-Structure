#include <iostream>

using namespace std;

// Node class representing a webpage
class webpage
{
public:
    int id;
    float time;
    webpage *next;
    webpage *prev;

    webpage() : id(0), time(0), next(nullptr), prev(nullptr) {}

    webpage(int i, float t) : id(i), time(t), next(nullptr), prev(nullptr) {}
};

// Doubly linked list for browser history
class webpage_List
{
private:
    webpage *head;
    webpage *tail;
    webpage *current;

public:
    webpage_List() : head(nullptr), tail(nullptr), current(nullptr) {}

    // A utility function to clear forward history
    void clear_forward_history()
    {
        webpage *temp = current->next;
        while (temp)
        {
            webpage *toDelete = temp;
            temp = temp->next;
            delete toDelete;
        }
        current->next = nullptr;
        tail = current;
    }

    // Insert a new webpage at the current position
    void insert_webpage(int id, float time_stamp)
    {
        webpage *newPage = new webpage(id, time_stamp);

        if (head == nullptr)
        {
            head = tail = current = newPage;
        }
        else
        {
            clear_forward_history();
            current->next = newPage;
            newPage->prev = current;
            current = newPage;
            tail = newPage;
        }
        cout << "Inserted Page ID: " << id << " at time " << time_stamp << endl;
    }

    // Insert a webpage at the beginning of the list
    void insert_at_start(int id, float time_stamp)
    {
        webpage *newPage = new webpage(id, time_stamp);

        if (head == nullptr)
        {
            head = tail = current = newPage;
        }
        else
        {
            newPage->next = head;
            head->prev = newPage;
            head = newPage;
        }
        cout << "Inserted at start. Page ID: " << id << " at time " << time_stamp << endl;
    }

    // Insert a webpage at the end of the list
    void insert_at_end(int id, float time_stamp)
    {
        if (head == nullptr)
        {
            insert_at_start(id, time_stamp);
            return;
        }

        webpage *newPage = new webpage(id, time_stamp);
        tail->next = newPage;
        newPage->prev = tail;
        tail = newPage;
        cout << "Inserted at end. Page ID: " << id << " at time " << time_stamp << endl;
    }

    // Insert a webpage before a given page ID
    void insert_before_id(int new_id, float time_stamp, int existing_id)
    {
        if (head == nullptr)
        {
            cout << "History is empty. Cannot insert before a non-existent page." << endl;
            return;
        }

        if (head->id == existing_id)
        {
            insert_at_start(new_id, time_stamp);
            return;
        }

        webpage *temp = head;
        while (temp != nullptr && temp->id != existing_id)
        {
            temp = temp->next;
        }

        if (temp == nullptr)
        {
            cout << "Page with ID " << existing_id << " not found." << endl;
            return;
        }

        webpage *newPage = new webpage(new_id, time_stamp);
        newPage->next = temp;
        newPage->prev = temp->prev;
        temp->prev->next = newPage;
        temp->prev = newPage;
        cout << "Inserted Page ID: " << new_id << " before Page ID: " << existing_id << endl;
    }

    // Insert a webpage after a given page ID
    void insert_after_id(int new_id, float time_stamp, int existing_id)
    {
        if (head == nullptr)
        {
            cout << "History is empty. Cannot insert after a non-existent page." << endl;
            return;
        }

        webpage *temp = head;
        while (temp != nullptr && temp->id != existing_id)
        {
            temp = temp->next;
        }

        if (temp == nullptr)
        {
            cout << "Page with ID " << existing_id << " not found." << endl;
            return;
        }

        if (temp == tail)
        {
            insert_at_end(new_id, time_stamp);
            return;
        }

        webpage *newPage = new webpage(new_id, time_stamp);
        newPage->next = temp->next;
        newPage->prev = temp;
        temp->next->prev = newPage;
        temp->next = newPage;
        cout << "Inserted Page ID: " << new_id << " after Page ID: " << existing_id << endl;
    }

    // Navigate back in history
    void go_back()
    {
        if (current == nullptr || current->prev == nullptr)
        {
            cout << "No previous pages." << endl;
            return;
        }
        current = current->prev;
        cout << "Navigated Back to Page ID: " << current->id << " at time " << current->time << endl;
    }

    // Navigate forward in history
    void go_forward()
    {
        if (current == nullptr || current->next == nullptr)
        {
            cout << "No forward pages." << endl;
            return;
        }
        current = current->next;
        cout << "Navigated Forward to Page ID: " << current->id << " at time " << current->time << endl;
    }

    // Traverse history from head to tail
    void traverse()
    {
        if (head == nullptr)
        {
            cout << "History is empty." << endl;
            return;
        }
        webpage *temp = head;
        cout << "\n--- Browser History (Oldest to Latest) ---\n";
        while (temp)
        {
            cout << "Page ID: " << temp->id << ", Time: " << temp->time;
            if (temp == current)
            {
                cout << " (Current)";
            }
            cout << endl;
            temp = temp->next;
        }
    }

    // Traverse history from tail to head
    void reverse_traverse()
    {
        if (tail == nullptr)
        {
            cout << "History is empty." << endl;
            return;
        }
        webpage *temp = tail;
        cout << "\n--- Browser History (Latest to Oldest) ---\n";
        while (temp)
        {
            cout << "Page ID: " << temp->id << ", Time: " << temp->time;
            if (temp == current)
            {
                cout << " (Current)";
            }
            cout << endl;
            temp = temp->prev;
        }
    }

    // Delete a page by ID
    void delete_page_by_id(int id)
    {
        if (head == nullptr)
        {
            cout << "History is empty." << endl;
            return;
        }

        webpage *temp = head;
        while (temp && temp->id != id)
        {
            temp = temp->next;
        }

        if (temp == nullptr)
        {
            cout << "Page with ID " << id << " not found." << endl;
            return;
        }

        // Adjust head, tail, and current pointers
        if (temp == head)
        {
            head = head->next;
            if (head)
                head->prev = nullptr;
        }
        if (temp == tail)
        {
            tail = tail->prev;
            if (tail)
                tail->next = nullptr;
        }
        if (temp->prev)
            temp->prev->next = temp->next;
        if (temp->next)
            temp->next->prev = temp->prev;

        if (temp == current)
        {
            current = temp->prev ? temp->prev : temp->next;
        }

        delete temp;
        cout << "Page with ID " << id << " deleted from history." << endl;
    }

    // Delete the first page
    void delete_first_page()
    {
        if (head == nullptr)
        {
            cout << "History is empty." << endl;
            return;
        }
        int id = head->id;
        webpage *toDelete = head;
        head = head->next;
        if (head)
        {
            head->prev = nullptr;
        }
        else
        {
            tail = nullptr;
            current = nullptr;
        }
        if (toDelete == current)
        {
            current = head;
        }
        delete toDelete;
        cout << "Deleted the first page with ID: " << id << endl;
    }

    // Delete the last page
    void delete_last_page()
    {
        if (tail == nullptr)
        {
            cout << "History is empty." << endl;
            return;
        }
        int id = tail->id;
        webpage *toDelete = tail;
        tail = tail->prev;
        if (tail)
        {
            tail->next = nullptr;
        }
        else
        {
            head = nullptr;
            current = nullptr;
        }
        if (toDelete == current)
        {
            current = tail;
        }
        delete toDelete;
        cout << "Deleted the last page with ID: " << id << endl;
    }
};

void show_insert_menu(webpage_List &web_object)
{
    int insert_choice;
    int id;
    float time;
    int existing_id;

    cout << "\n--- Insert Menu ---\n";
    cout << "1. Insert a new webpage (Visit)\n";
    cout << "2. Insert at the start of history\n";
    cout << "3. Insert at the end of history\n";
    cout << "4. Insert before an existing page\n";
    cout << "5. Insert after an existing page\n";
    cout << "Enter your choice: ";
    cin >> insert_choice;

    switch (insert_choice)
    {
    case 1:
        cout << "Enter Page ID: ";
        cin >> id;
        cout << "Enter Time Stamp: ";
        cin >> time;
        web_object.insert_webpage(id, time);
        break;
    case 2:
        cout << "Enter Page ID: ";
        cin >> id;
        cout << "Enter Time Stamp: ";
        cin >> time;
        web_object.insert_at_start(id, time);
        break;
    case 3:
        cout << "Enter Page ID: ";
        cin >> id;
        cout << "Enter Time Stamp: ";
        cin >> time;
        web_object.insert_at_end(id, time);
        break;
    case 4:
        cout << "Enter Page ID of new page: ";
        cin >> id;
        cout << "Enter Time Stamp of new page: ";
        cin >> time;
        cout << "Enter the ID of the page to insert before: ";
        cin >> existing_id;
        web_object.insert_before_id(id, time, existing_id);
        break;
    case 5:
        cout << "Enter Page ID of new page: ";
        cin >> id;
        cout << "Enter Time Stamp of new page: ";
        cin >> time;
        cout << "Enter the ID of the page to insert after: ";
        cin >> existing_id;
        web_object.insert_after_id(id, time, existing_id);
        break;
    default:
        cout << "Invalid choice." << endl;
    }
}

void show_delete_menu(webpage_List &web_object)
{
    int delete_choice;
    int del_id;

    cout << "\n--- Delete Menu ---\n";
    cout << "1. Delete a specific webpage by ID\n";
    cout << "2. Delete the first page\n";
    cout << "3. Delete the last page\n";
    cout << "Enter your choice: ";
    cin >> delete_choice;

    switch (delete_choice)
    {
    case 1:
        cout << "Enter Page ID to delete: ";
        cin >> del_id;
        web_object.delete_page_by_id(del_id);
        break;
    case 2:
        web_object.delete_first_page();
        break;
    case 3:
        web_object.delete_last_page();
        break;
    default:
        cout << "Invalid choice." << endl;
    }
}

int main()
{
    webpage_List web_object;
    int choice;

    do
    {
        cout << "\n--- Browser History Main Menu ---\n";
        cout << "1. Insert/Add Webpage\n";
        cout << "2. Navigate Back\n";
        cout << "3. Navigate Forward\n";
        cout << "4. Show History (Oldest to Latest)\n";
        cout << "5. Show History (Latest to Oldest)\n";
        cout << "6. Delete Webpage(s)\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            show_insert_menu(web_object);
            break;
        case 2:
            web_object.go_back();
            break;
        case 3:
            web_object.go_forward();
            break;
        case 4:
            web_object.traverse();
            break;
        case 5:
            web_object.reverse_traverse();
            break;
        case 6:
            show_delete_menu(web_object);
            break;
        case 7:
            cout << "Exiting browser history program.\n";
            break;
        default:
            cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 7);
    cout << string(50, ' ') << "Thank you for using the browser history program!" << endl;
    return 0;
}   