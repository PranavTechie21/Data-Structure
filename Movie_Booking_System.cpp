#include <iostream>
#include <vector>
#include <string>
#include <limits>

using namespace std;

struct Seat
{
    int row;
    int seatNumber;
    bool isBooked;
    Seat *next;
    Seat *prev;

    Seat(int r, int s) : row(r), seatNumber(s), isBooked(false), next(nullptr), prev(nullptr) {}
};

class MovieTheater
{
private:
    Seat *head;
    int ROWS;
    int COLS;
    int pricePerSeat = 150; // Add this line

public:
    MovieTheater(int rows, int cols) : head(nullptr), ROWS(rows), COLS(cols)
    {
        if (rows <= 0 || cols <= 0)
        {
            cout << "Invalid theater dimensions. Please use positive numbers for rows and columns." << endl;
            return;
        }

        head = new Seat(1, 1);
        Seat *current = head;

        for (int r = 1; r <= rows; ++r)
        {
            for (int s = (r == 1 ? 2 : 1); s <= cols; ++s)
            {
                Seat *newSeat = new Seat(r, s);
                current->next = newSeat;
                newSeat->prev = current;
                current = newSeat;
            }
        }

        current->next = head;
        head->prev = current;
    }

    ~MovieTheater()
    {
        if (head == nullptr)
        {
            return;
        }
        Seat *current = head->next;
        while (current != head)
        {
            Seat *temp = current;
            current = current->next;
            delete temp;
        }
        delete head;
        head = nullptr;
    }

    // Display the seating arrangement
    void displaySeating() const
    {
        if (head == nullptr)
        {
            cout << "The theater is not set up." << endl;
            return;
        }

        cout << "\nCurrent Seating Arrangement ('O' = available, 'X' = booked):\n\n";
        cout << "\t";
        for (int c = 1; c <= COLS; ++c)
        {
            cout << c << " ";
        }
        cout << "\n\n";

        Seat *current = head;
        int currentRow = 1;
        cout << currentRow << "\t";

        do
        {
            if (current->row > currentRow)
            {
                cout << endl;
                currentRow = current->row;
                cout << currentRow << "\t";
            }
            cout << (current->isBooked ? 'X' : 'O') << " ";
            current = current->next;
        } while (current != head);

        cout << endl
             << endl;
    }

    // Find a specific seat node
    Seat *findSeat(int row, int seat) const
    {
        if (head == nullptr || row > ROWS || seat > COLS || row < 1 || seat < 1)
        {
            return nullptr;
        }
        Seat *current = head;
        do
        {
            if (current->row == row && current->seatNumber == seat)
            {
                return current;
            }
            current = current->next;
        } while (current != head);
        return nullptr;
    }

    // Book a single seat
    void bookSeat(int row, int seat)
    {
        Seat *selectedSeat = findSeat(row, seat);
        if (selectedSeat == nullptr)
        {
            cout << "Invalid seat position. Please try again." << endl;
            return;
        }
        if (selectedSeat->isBooked)
        {
            cout << "Sorry, seat (" << row << ", " << seat << ") is already booked." << endl;
        }
        else
        {
            selectedSeat->isBooked = true;
            cout << "Successfully booked seat (" << row << ", " << seat << ")." << endl;
            cout << "Total price: " << pricePerSeat << endl; // Show price
        }
    }

    // Book multiple seats
    void bookMultipleSeats(int startRow, int startSeat, int numSeats)
    {
        if (numSeats <= 0)
        {
            cout << "Number of seats must be positive." << endl;
            return;
        }

        Seat *current = findSeat(startRow, startSeat);
        if (current == nullptr)
        {
            cout << "Invalid starting position. Please try again." << endl;
            return;
        }

        vector<pair<int, int>> bookedSeats;
        int seatsBooked = 0;
        Seat *startNode = current;

        // Traverse the list from the starting point
        for (int i = 0; i < ROWS * COLS && seatsBooked < numSeats; ++i)
        {
            if (!current->isBooked)
            {
                current->isBooked = true;
                bookedSeats.push_back({current->row, current->seatNumber});
                seatsBooked++;
            }
            current = current->next;
            // Check if we have traversed the entire list without finding enough seats
            if (current == startNode && seatsBooked < numSeats)
            {
                cout << "Cannot book " << numSeats << " seats. Only " << seatsBooked << " seats were available." << endl;
                return;
            }
        }

        if (seatsBooked == numSeats)
        {
            cout << "Successfully booked " << seatsBooked << " seats:" << endl;
            for (const auto &seat : bookedSeats)
            {
                cout << "  (" << seat.first << ", " << seat.second << ")";
            }
            cout << endl;
            cout << "Total price: " << pricePerSeat * seatsBooked << endl; // Show total price
        }
        else
        {
            cout << "Cannot book " << numSeats << " seats. Only " << seatsBooked << " seats were available." << endl;
        }
    }

    // Cancel a single seat booking
    void cancelBooking(int row, int seat)
    {
        Seat *selectedSeat = findSeat(row, seat);
        if (selectedSeat == nullptr)
        {
            cout << "Invalid seat position. Please try again." << endl;
            return;
        }
        if (!selectedSeat->isBooked)
        {
            cout << "This seat is not currently booked. Nothing to cancel." << endl;
        }
        else
        {
            selectedSeat->isBooked = false;
            cout << "Booking for seat (" << row << ", " << seat << ") has been successfully canceled." << endl;
        }
    }

    // Cancel multiple seat bookings
    void cancelMultipleSeats(int startRow, int startSeat, int numSeats)
    {
        if (numSeats <= 0)
        {
            cout << "Number of seats must be positive." << endl;
            return;
        }

        Seat *current = findSeat(startRow, startSeat);
        if (current == nullptr)
        {
            cout << "Invalid starting position. Please try again." << endl;
            return;
        }

        vector<pair<int, int>> cancelledSeats;
        int seatsCancelled = 0;
        Seat *startNode = current;

        // Traverse the list from the starting point
        for (int i = 0; i < ROWS * COLS && seatsCancelled < numSeats; ++i)
        {
            if (current->isBooked)
            {
                current->isBooked = false;
                cancelledSeats.push_back({current->row, current->seatNumber});
                seatsCancelled++;
            }
            current = current->next;
            // Check if we have traversed the entire list without finding enough seats
            if (current == startNode && seatsCancelled < numSeats)
            {
                cout << "Cannot cancel " << numSeats << " seats. Only " << seatsCancelled << " seats were booked." << endl;
                return;
            }
        }

        if (seatsCancelled == numSeats)
        {
            cout << "Successfully cancelled " << seatsCancelled << " seats:" << endl;
            for (const auto &seat : cancelledSeats)
            {
                cout << "  (" << seat.first << ", " << seat.second << ")";
            }
            cout << endl;
        }
        else
        {
            cout << "Cannot cancel " << numSeats << " seats. Only " << seatsCancelled << " seats were booked." << endl;
        }
    }

    // Check seat availability
    void checkAvailability(int row, int seat)
    {
        Seat *selectedSeat = findSeat(row, seat);
        if (selectedSeat == nullptr)
        {
            cout << "Invalid seat position. Please try again." << endl;
            return;
        }
        if (selectedSeat->isBooked)
        {
            cout << "Seat (" << row << ", " << seat << ") is currently booked." << endl;
        }
        else
        {
            cout << "Seat (" << row << ", " << seat << ") is available." << endl;
        }
    }

    int rows() const { return ROWS; }
    int seatsPerRow() const { return COLS; }
};

void handleInvalidInput()
{
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Invalid input. Please enter a valid number." << endl;
}

bool readInt(int &out)
{
    if (!(cin >> out))
    {
        handleInvalidInput();
        return false;
    }
    return true;
}

int main()
{
    int rows, cols;

    cout << "Welcome to Theater Booking System!" << endl;
    cout << "Enter number of rows: ";
    if (!readInt(rows) || rows <= 0)
    {
        cout << "Invalid number of rows. Exiting..." << endl;
        return 1;
    }

    cout << "Enter number of seats per row: ";
    if (!readInt(cols) || cols <= 0)
    {
        cout << "Invalid number of seats per row. Exiting..." << endl;
        return 1;
    }

    MovieTheater booking_system(rows, cols);
    cout << "\nTheater created with " << rows << " rows and " << cols << " seats per row." << endl;

    int choice = 0;
    int row = 0;
    int seat = 0;
    int numSeats = 0;

    do
    {
        cout << "\n--- Theater Booking System Menu ---" << endl;
        cout << "1. Display Seating" << endl;
        cout << "2. Book a Single Seat" << endl;
        cout << "3. Book Multiple Seats" << endl;
        cout << "4. Cancel a Single Booking" << endl;
        cout << "5. Cancel Multiple Bookings" << endl;
        cout << "6. Check Seat Availability" << endl;
        cout << "7. Exit" << endl;
        cout << "-----------------------------------" << endl;
        cout << "Enter your choice: ";

        if (!readInt(choice))
        {
            continue;
        }

        switch (choice)
        {
        case 1:
            booking_system.displaySeating();
            break;

        case 2:
            cout << "Enter row number: ";
            if (!readInt(row))
                break;

            cout << "Enter seat number: ";
            if (!readInt(seat))
                break;

            booking_system.bookSeat(row, seat);
            break;

        case 3:
            cout << "Enter starting row number: ";
            if (!readInt(row))
                break;

            cout << "Enter starting seat number: ";
            if (!readInt(seat))
                break;

            cout << "Enter number of seats to book: ";
            if (!readInt(numSeats))
                break;

            booking_system.bookMultipleSeats(row, seat, numSeats);
            break;

        case 4:
            cout << "Enter row number: ";
            if (!readInt(row))
                break;

            cout << "Enter seat number: ";
            if (!readInt(seat))
                break;

            booking_system.cancelBooking(row, seat);
            break;

        case 5:
            cout << "Enter starting row number: ";
            if (!readInt(row))
                break;

            cout << "Enter starting seat number: ";
            if (!readInt(seat))
                break;

            cout << "Enter number of seats to cancel: ";
            if (!readInt(numSeats))
                break;

            booking_system.cancelMultipleSeats(row, seat, numSeats);
            break;

        case 6:
            cout << "Enter row number: ";
            if (!readInt(row))
                break;

            cout << "Enter seat number: ";
            if (!readInt(seat))
                break;

            booking_system.checkAvailability(row, seat);
            break;

        case 7:
            cout << "Exiting program. Goodbye!" << endl;
            break;

        default:
            cout << "Invalid choice. Please enter a number between 1 and 7." << endl;
            break;
        }
    } while (choice != 7);
    cout << string(50, ' ') << "Thank you for using the Theater Booking System!" << endl;

    return 0;
}
