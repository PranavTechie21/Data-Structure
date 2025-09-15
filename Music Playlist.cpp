#include <iostream>
#include <cstring>
#include <limits>
#include <stdexcept>

using namespace std;

class Media
{
public:
    int music_id;
    char music_title[10];
    int music_duration;
    Media *next;
};

class Playlist
{
private:
    Media *head;

    // Helper function for bubble sort
    void swapNodes(Media *a, Media *b)
    {
        swap(a->music_id, b->music_id);
        swap(a->music_title, b->music_title);
        swap(a->music_duration, b->music_duration);
    }

    // Check if a song ID already exists
    bool idExists(int id)
    {
        if (!head)
            return false;
        Media *temp = head;
        do
        {
            if (temp->music_id == id)
                return true;
            temp = temp->next;
        } while (temp != head);
        return false;
    }

public:
    Playlist() : head(nullptr) {}

    ~Playlist()
    {
        if (!head)
            return;
        Media *current = head->next;
        Media *next;
        while (current != head)
        {
            next = current->next;
            delete current;
            current = next;
        }
        delete head;
    }

    // Helper function to validate integer input
    int getValidIntInput()
    {
        int input;
        while (!(cin >> input))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number: ";
        }
        return input;
    }

    void insert_songs()
    {
        Media *newNode = new Media;
        cout << "Enter Music ID: ";
        newNode->music_id = getValidIntInput();

        // Check for duplicate ID
        while (idExists(newNode->music_id))
        {
            cout << "This ID already exists. Please enter a unique ID: ";
            newNode->music_id = getValidIntInput();
        }

        cout << "Enter Music Title (max 9 chars): ";
        cin.ignore();
        cin.getline(newNode->music_title, 10);

        // Ensure null-termination
        newNode->music_title[9] = '\0';

        cout << "Enter Music Duration in seconds: ";
        newNode->music_duration = getValidIntInput();

        if (!head)
        {
            head = newNode;
            newNode->next = head;
        }
        else
        {
            Media *last = head;
            while (last->next != head)
            {
                last = last->next;
            }
            last->next = newNode;
            newNode->next = head;
        }
        cout << "Song inserted successfully.\n";
    }

    void display_songs()
    {
        if (!head)
        {
            cout << "No songs to display." << endl;
            return;
        }

        Media *temp = head;
        cout << "\n--- Songs List ---\n";
        do
        {
            cout << "ID: " << temp->music_id
                 << ", Title: " << temp->music_title
                 << ", Duration: " << temp->music_duration << " seconds" << endl;
            temp = temp->next;
        } while (temp != head);
    }

    void delete_songs(int id)
    {
        if (!head)
        {
            cout << "Playlist is empty.\n";
            return;
        }

        Media *curr = head;
        Media *prev = nullptr;

        do
        {
            if (curr->music_id == id)
            {
                if (curr == head && curr->next == head)
                {
                    delete head;
                    head = nullptr;
                }
                else if (curr == head)
                {
                    prev = head;
                    while (prev->next != head)
                        prev = prev->next;
                    head = head->next;
                    prev->next = head;
                    delete curr;
                }
                else
                {
                    prev->next = curr->next;
                    delete curr;
                }
                cout << "Song with ID " << id << " deleted.\n";
                return;
            }
            prev = curr;
            curr = curr->next;
        } while (curr != head);

        cout << "Song with ID " << id << " not found.\n";
    }

    void count_songs()
    {
        if (!head)
        {
            cout << "The number of songs is: 0\n";
            return;
        }
        int count = 0;
        Media *temp = head;
        do
        {
            count++;
            temp = temp->next;
        } while (temp != head);
        cout << "\nThe number of songs is: " << count << "\n";
    }

    void sort_asperDuration()
    {
        if (!head || head->next == head)
        {
            cout << "No songs to display." << endl;
            return;
        }

        bool swapped;
        Media *current;
        Media *last = nullptr;

        do
        {
            swapped = false;
            current = head;

            while (current->next != last)
            {
                if (current->music_duration > current->next->music_duration)
                {
                    swapNodes(current, current->next);
                    swapped = true;
                }
                current = current->next;
            }
            last = current;
        } while (swapped);
        cout << "Songs sorted by duration successfully.\n";
    }

    void play_inLoop()
    {
        if (!head)
        {
            cout << "No songs to play.\n";
            return;
        }
        cout << "\nPlaying songs in a continuous loop... (Enter 'q' to stop)\n";
        Media *temp = head;
        string input;
        do
        {
            cout << "Playing: " << temp->music_title << " [" << temp->music_duration << " seconds]" << endl;
            cout << "Enter 'n' for next song or 'q' to quit: ";
            cin >> input;
            if (input == "n")
            {
                temp = temp->next;
            }
        } while (input != "q");
    }
};

int main()
{
    Playlist playlist;
    int choice;

    do
    {
        cout << "\n--- Music Playlist Menu ---\n";
        cout << "1. Insert Songs\n";
        cout << "2. Display Songs\n";
        cout << "3. Delete Song by ID\n";
        cout << "4. Count Songs\n";
        cout << "5. Sort Songs by Duration\n";
        cout << "6. Play in Loop\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";

        try
        {
            choice = playlist.getValidIntInput();

            switch (choice)
            {
            case 1:
                playlist.insert_songs();
                break;
            case 2:
                playlist.display_songs();
                break;
            case 3:
            {
                int id;
                cout << "Enter Music ID to delete: ";
                id = playlist.getValidIntInput();
                playlist.delete_songs(id);
                break;
            }
            case 4:
                playlist.count_songs();
                break;
            case 5:
                playlist.sort_asperDuration();
                break;
            case 6:
                playlist.play_inLoop();
                break;
            case 7:
                cout << "Exiting the program. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
            }
        }
        catch (const exception &e)
        {
            cerr << "An error occurred: " << e.what() << endl;
        }
    } while (choice != 7);

    cout << string(50, ' ') << "Thank you for using the Music Playlist!" << endl;
    return 0;
}
