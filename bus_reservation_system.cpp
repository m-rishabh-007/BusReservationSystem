#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <limits>
#include <iomanip>
#include <random>
#include <ctime>
#include <algorithm>
using namespace std;

// Function to print a horizontal line of characters
void vline(char character, int length)
{
    for (int i = length; i > 0; i--) // Loop to print the character 'length' times
    {
        cout << character; // Print the character
    }
    cout << endl; // Move to the next line after printing the line of characters
}

class BusReservationSystem
{
private:
    // Constants for the number of rows, columns, and maximum seats
    static const int NUM_ROWS = 8;
    static const int NUM_COLS = 4;
    static const int MAX_SEATS = NUM_ROWS * NUM_COLS;

    // Structure to represent seat information
    struct SeatInfo
    {
        string passengerName; // Passenger name
        string ticketID;      // Ticket ID
    };

    // Structure to represent a bus
    struct Bus
    {
        string busn;                   // Bus number
        string driver;                 // Driver's name
        string arrival;                // Arrival time
        string depart;                 // Departure time
        string from;                   // Source
        string to;                     // Destination
        vector<vector<SeatInfo>> seat; // 2D vector to store seat information
    };

    vector<Bus> bus;  // Vector to store bus objects
    int busCount = 0; // Counter to keep track of the number of buses

    string generateTicketID(); // Function to generate a ticket ID

public:
    // Function prototypes for public member functions
    void addNewBus();                                // Function to add a new bus
    void allotmentOfSeatToPassenger();               // Function to allot a seat to a passenger
    void showAvailableBusSeats();                    // Function to show available seats in a bus
    void showAvailableBuses();                       // Function to show available buses
    void showReservedBusSeats(int selectedBusIndex); // Function to show reserved seats in a bus
    void showTicketInfo();                           // Function to show ticket information
    bool validateTimeFormat(const string &time);     // Function to validate time format
    void deleteBus();                                // Function to delete a bus
    void cancelTicket();                             // Function to cancel a ticket
};

void BusReservationSystem::addNewBus()
{
    // Check if the maximum limit of buses has been reached
    if (busCount >= 25)
    {
        cout << "Cannot add more buses. Maximum limit reached." << endl;
        return; // Exit the function if the maximum limit is reached
    }

    Bus newBus; // Create a new bus object

    // Prompt the user to enter the bus number
    cout << "Enter bus number (4 digits): ";
    getline(cin, newBus.busn);

    // Validate the format of the entered bus number
    if (newBus.busn.length() != 4 || !isdigit(newBus.busn[0]) || !isdigit(newBus.busn[1]) || !isdigit(newBus.busn[2]) || !isdigit(newBus.busn[3]))
    {
        cout << "Invalid bus number format. Please enter a valid 4-digit bus number." << endl;
        return; // Exit the function if the bus number format is invalid
    }

    // Check if the entered bus number already exists
    for (int i = 0; i < busCount; ++i)
    {
        if (bus[i].busn == newBus.busn)
        {
            cout << "Bus number already exists. Please enter a unique bus number." << endl;
            return; // Exit the function if the bus number already exists
        }
    }

    // Prompt the user to enter the driver's name
    cout << "Enter driver's name: ";
    getline(cin, newBus.driver);

    // Validate that the driver's name is not empty
    if (newBus.driver.empty())
    {
        cout << "Driver's name cannot be empty. Please re-enter." << endl;
        return; // Exit the function if the driver's name is empty
    }

    // Prompt the user to enter the arrival time
    cout << "Enter arrival time (HH:MM AM/PM): ";
    getline(cin, newBus.arrival);

    // Validate the format of the arrival time
    if (!validateTimeFormat(newBus.arrival))
    {
        cout << "Invalid arrival time format. Please enter time in the format HH:MM AM/PM." << endl;
        return; // Exit the function if the arrival time format is invalid
    }

    // Prompt the user to enter the departure time
    cout << "Enter departure time (HH:MM AM/PM): ";
    getline(cin, newBus.depart);

    // Validate the format of the departure time
    if (!validateTimeFormat(newBus.depart))
    {
        cout << "Invalid departure time format. Please enter time in the format HH:MM AM/PM." << endl;
        return; // Exit the function if the departure time format is invalid
    }

    // Prompt the user to enter the source
    cout << "Enter source: ";
    getline(cin, newBus.from);

    // Validate that the source is not empty
    if (newBus.from.empty())
    {
        cout << "Source cannot be empty. Please re-enter." << endl;
        return; // Exit the function if the source is empty
    }

    // Prompt the user to enter the destination
    cout << "Enter destination: ";
    getline(cin, newBus.to);

    // Validate that the destination is not empty
    if (newBus.to.empty())
    {
        cout << "Destination cannot be empty. Please re-enter." << endl;
        return; // Exit the function if the destination is empty
    }

    // Resize the seat matrix to the specified number of rows and columns
    newBus.seat.resize(NUM_ROWS, vector<SeatInfo>(NUM_COLS));

    // Add the new bus to the vector of buses
    bus.push_back(newBus);

    // Increment the bus count
    busCount++;

    // Display a success message
    cout << "New bus added successfully." << endl;
}

bool BusReservationSystem::validateTimeFormat(const string &time)
{
    // Define a regular expression pattern for valid time format (HH:MM AM/PM)
    regex pattern("^(0?[1-9]|1[0-2]):[0-5][0-9] (AM|PM|am|pm)$");

    // Check if the input time matches the defined pattern
    return regex_match(time, pattern);
}

string BusReservationSystem::generateTicketID()
{
    // Generate a ticket ID with alphanumeric characters
    const string charset = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    const int id_length = 8; // Length of the ticket ID

    random_device rd;                                             // Random device for seeding the generator
    mt19937 gen(rd());                                            // Mersenne Twister pseudo-random number generator
    uniform_int_distribution<> index_dist(0, charset.size() - 1); // Distribution for selecting random characters

    string ticketID; // Variable to store the generated ticket ID
    for (int i = 0; i < id_length; ++i)
    {
        ticketID += charset[index_dist(gen)]; // Append a random character from the charset to the ticket ID
    }

    // Convert the ticket ID to lowercase
    transform(ticketID.begin(), ticketID.end(), ticketID.begin(), ::tolower);

    return ticketID; // Return the generated ticket ID
}

void BusReservationSystem::showAvailableBusSeats()
{
    string number;
    cout << "Enter bus no: ";
    cin >> number;

    // Find the index of the selected bus in the vector of buses
    int n;
    for (n = 0; n < busCount; ++n)
    {
        if (bus[n].busn == number)
            break;
    }

    // Check if the entered bus number is valid
    if (n == busCount)
    {
        cout << "Invalid bus number. Please enter a correct bus number." << endl;
        return; // Exit the function if the bus number is invalid
    }

    // Display bus details (bus number, driver, arrival/departure time, source, destination)
    vline('*', 75);
    cout << "\nBus no: \t" << bus[n].busn
         << "\nDriver: \t" << bus[n].driver << "\t\tArrival time: \t"
         << bus[n].arrival << "\tDeparture time:" << bus[n].depart
         << "\nFrom: \t\t" << bus[n].from << "\t\tTo: \t\t" << bus[n].to << "\n";
    vline('*', 75);

    // Display available seats in the selected bus
    int emptySeats = 0; // Counter for empty seats
    int seatNumber = 1; // Seat number counter
    for (int i = 0; i < NUM_ROWS; ++i)
    {
        cout << "\n";
        for (int j = 0; j < NUM_COLS; ++j)
        {
            // Check if the seat is empty
            if (bus[n].seat[i][j].passengerName.empty())
            {
                // Display empty seat
                cout.width(5);
                cout.fill(' ');
                cout << seatNumber << ".";
                cout.width(10);
                cout.fill(' ');
                cout << "Empty";
                ++emptySeats; // Increment empty seat count
            }
            else
            {
                // Display occupied seat with passenger name
                cout.width(5);
                cout.fill(' ');
                cout << seatNumber << ".";
                cout.width(10);
                cout.fill(' ');
                cout << bus[n].seat[i][j].passengerName;
            }
            ++seatNumber; // Increment seat number
        }
    }

    // Display the total number of empty seats in the selected bus
    cout << "\n\nThere are " << emptySeats << " seats empty in Bus No: " << bus[n].busn << endl;
}

void BusReservationSystem::allotmentOfSeatToPassenger()
{
    string busNumber;
    cout << "Enter bus number: ";
    cin >> busNumber;

    // Find the index of the selected bus in the vector of buses
    int busIndex = -1;
    for (size_t i = 0; i < bus.size(); ++i)
    {
        if (bus[i].busn == busNumber)
        {
            busIndex = i;
            break;
        }
    }

    // Check if the entered bus number is valid
    if (busIndex == -1)
    {
        cout << "Bus with number " << busNumber << " not found." << endl;
        return; // Exit the function if the bus number is invalid
    }

    // Prompt for the seat number the passenger wants to reserve
    cout << "Enter seat number for bus number " << busNumber << ": ";
    int seatNumber;
    cin >> seatNumber;

    // Check for valid input for seat number
    if (cin.fail())
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input for seat number. Please enter a valid integer." << endl;
        return;
    }
    // Check if the seat number is within the valid range
    if (seatNumber < 1 || seatNumber > MAX_SEATS)
    {
        cout << "Invalid seat number. Please enter a number between 1 and " << MAX_SEATS << "." << endl;
        return;
    }

    // Calculate row and column indices for the selected seat number
    int row = (seatNumber - 1) / NUM_COLS;
    int col = (seatNumber - 1) % NUM_COLS;

    // Check if the selected seat is empty
    if (bus[busIndex].seat[row][col].passengerName.empty())
    {
        string name;
        cout << "Enter passenger name: ";
        cin.ignore(); // Ignore newline character in the buffer
        getline(cin, name);

        // Generate a unique ticket ID for the passenger
        string ticketID = generateTicketID();

        // Assign passenger name and ticket ID to the selected seat
        bus[busIndex].seat[row][col].passengerName = name;
        bus[busIndex].seat[row][col].ticketID = ticketID;

        // Display confirmation message and ticket information
        cout << "Seat number " << seatNumber << " allotted to passenger " << name << "." << endl;
        cout << "Ticket Info:\n"
             << "Name: " << name << "\n"
             << "Ticket ID: " << ticketID << "\n"
             << "Bus Number: " << bus[busIndex].busn << "\n"
             << "Seat Number: " << seatNumber << endl;
    }
    else
    {
        // If the selected seat is already occupied, notify the user
        cout << "Seat number " << seatNumber << " is already occupied. Please select another seat." << endl;
    }
}

void BusReservationSystem::showReservedBusSeats(int selectedBusIndex)
{
    int totalSeats = 0; // Counter to keep track of total seats in the bus

    // Loop through each row and column of the bus's seat matrix
    for (int row = 0; row < NUM_ROWS; row++)
    {
        cout << "\n"; // Newline for formatting
        for (int col = 0; col < NUM_COLS; col++)
        {
            // Calculate the seat number based on row and column indices
            int seatNumber = row * NUM_COLS + col + 1;

            // Set the width and fill character for formatting output
            cout.width(5);
            cout.fill(' ');

            // Display the seat number
            cout << seatNumber << ".";

            // Set the width and fill character for passenger name or "Empty"
            cout.width(10);
            cout.fill(' ');

            // Check if the seat is occupied by a passenger
            if (!bus[selectedBusIndex].seat[row][col].passengerName.empty())
            {
                // If occupied, display the passenger name
                cout << bus[selectedBusIndex].seat[row][col].passengerName;
            }
            else
            {
                // If empty, indicate that the seat is empty
                cout << "Empty";
            }

            totalSeats++; // Increment the total seats counter
        }
    }

    // Display the total number of seats in the bus
    cout << "\n\nThere are " << totalSeats << " seats in Bus No: " << bus[selectedBusIndex].busn;
}

void BusReservationSystem::deleteBus()
{
    string busNumber; // Variable to store the bus number entered by the user
    cout << "Enter the bus number you want to delete: ";
    cin >> busNumber;

    int busIndex = -1; // Index of the bus to be deleted, initialized to -1
    for (size_t i = 0; i < bus.size(); ++i)
    {
        // Iterate through the buses to find the index of the bus to be deleted
        if (bus[i].busn == busNumber)
        {
            busIndex = i; // Store the index if the bus number matches
            break;
        }
    }

    // Check if the bus number was not found
    if (busIndex == -1)
    {
        cout << "Bus number " << busNumber << " not found. Please enter a valid bus number." << endl;
        return; // Exit the function if the bus number was not found
    }

    // Ask for confirmation before deleting the bus
    cout << "Are you sure you want to delete bus number " << busNumber << "? (Y/N): ";
    char confirmation;
    cin >> confirmation;

    // Check user confirmation
    if (toupper(confirmation) != 'Y')
    {
        cout << "Deletion cancelled. Bus number " << busNumber << " not deleted." << endl;
        return; // Exit the function if the user cancels the deletion
    }

    // If user confirms, proceed with deletion
    // Remove the bus from the vector using the stored index
    bus.erase(bus.begin() + busIndex);
    busCount--;                                                             // Decrement the total bus count
    cout << "Bus number " << busNumber << " deleted successfully." << endl; // Inform the user about successful deletion
}

void BusReservationSystem::showTicketInfo()
{
    string ticketID; // Variable to store the ticket ID entered by the user
    cout << "Enter ticket ID: ";
    cin >> ticketID;

    // Convert the entered ticket ID to lowercase for case-insensitive comparison
    transform(ticketID.begin(), ticketID.end(), ticketID.begin(), ::tolower);

    // Flag to track if the ticket was found
    bool ticketFound = false;

    // Iterate over each bus entry to search for the ticket ID
    for (const auto &busEntry : bus)
    {
        for (size_t i = 0; i < busEntry.seat.size(); ++i)
        {
            for (size_t j = 0; j < busEntry.seat[i].size(); ++j)
            {
                // Convert the stored ticket ID to lowercase for comparison
                string storedTicketID = busEntry.seat[i][j].ticketID;
                transform(storedTicketID.begin(), storedTicketID.end(), storedTicketID.begin(), ::tolower);

                // Check if the stored ticket ID matches the entered ticket ID
                if (storedTicketID == ticketID)
                {
                    // Ticket found, display information
                    cout << "Ticket Info:" << endl;
                    cout << "Name: " << busEntry.seat[i][j].passengerName << endl;
                    cout << "Bus No.: " << busEntry.busn << endl;
                    // Calculate seat number based on row and column index
                    int seatNumber = i * NUM_COLS + j + 1;
                    cout << "Seat No.: " << seatNumber << endl;
                    cout << "Ticket ID: " << storedTicketID << endl; // Display the actual ticket ID
                    ticketFound = true;                              // Set the flag to true to indicate ticket found
                    break;                                           // Exit loop since ticket is found
                }
            }
            if (ticketFound)
                break; // Exit loop since ticket is found
        }
        if (ticketFound)
            break; // Exit loop since ticket is found
    }

    // If ticket is not found, display appropriate message
    if (!ticketFound)
    {
        cout << "Ticket with ID " << ticketID << " not found." << endl;
    }
}

void BusReservationSystem::showAvailableBuses()
{
    // Check if there are no buses available
    if (busCount == 0)
    {
        cout << "No buses available at the moment." << endl;
        return;
    }

    // Find the length of the longest departure time among all buses
    int maxDepartureTimeLength = 0;
    for (int n = 0; n < busCount; n++)
    {
        // Check if departure time contains extra characters like AM/PM
        size_t pos = bus[n].depart.find_first_of("APM");
        int departureTimeLength = pos != string::npos ? pos + 2 : bus[n].depart.length();

        // Update the maximum departure time length if needed
        if (departureTimeLength > maxDepartureTimeLength)
        {
            maxDepartureTimeLength = departureTimeLength;
        }
    }

    // Display information for each bus
    for (int n = 0; n < busCount; n++)
    {
        // Draw a horizontal line to separate buses
        vline('*', 75);

        // Display bus details with aligned columns
        cout << "Bus no: \t" << bus[n].busn << "\nDriver: \t" << bus[n].driver
             << "\t\tArrival time: \t" << bus[n].arrival << "\tDeparture Time: \t"
             << setw(maxDepartureTimeLength) << left << setfill(' ') << bus[n].depart
             << "\nFrom: \t\t" << bus[n].from << "\t\tTo: \t\t\t" << setw(20) << left << bus[n].to << "\n";

        // Draw a horizontal line to separate bus details and available seats
        vline('*', 75);
        vline('_', 75);

        // Count and display available seats for the current bus
        cout << "Available Seats: ";
        int emptySeats = 0;
        for (int i = 0; i < NUM_ROWS; ++i)
        {
            for (int j = 0; j < NUM_COLS; ++j)
            {
                // Check if the seat is empty
                if (bus[n].seat[i][j].passengerName.empty())
                {
                    ++emptySeats;
                }
            }
        }
        cout << emptySeats << "/" << MAX_SEATS << endl;
    }
}

void BusReservationSystem::cancelTicket()
{
    // Prompt the user to enter the ticket ID to cancel
    string ticketID;
    cout << "Enter ticket ID: ";
    getline(cin, ticketID);

    // Convert the entered ticket ID to lowercase for comparison
    transform(ticketID.begin(), ticketID.end(), ticketID.begin(), ::tolower);

    // Flag to track if the ticket is found
    bool ticketFound = false;

    // Iterate through buses and their seats to find the ticket
    for (size_t i = 0; i < bus.size(); ++i)
    {
        for (size_t row = 0; row < NUM_ROWS; ++row)
        {
            for (size_t col = 0; col < NUM_COLS; ++col)
            {
                // Convert the stored ticket ID to lowercase for comparison
                string storedTicketID = bus[i].seat[row][col].ticketID;
                transform(storedTicketID.begin(), storedTicketID.end(), storedTicketID.begin(), ::tolower);

                // Check if the stored ticket ID matches the entered ticket ID
                if (storedTicketID == ticketID)
                {
                    // Display ticket information
                    cout << "Ticket Info:" << endl;
                    cout << "Name: " << bus[i].seat[row][col].passengerName << endl;
                    cout << "Ticket ID: " << storedTicketID << endl;
                    cout << "Bus Number: " << bus[i].busn << endl;

                    // Calculate the seat number
                    int seatNumber = row * NUM_COLS + col + 1;
                    cout << "Seat Number: " << seatNumber << endl;

                    // Confirm ticket cancellation
                    char confirmation;
                    cout << "Are you sure you want to cancel this ticket? (Y/N): ";
                    cin >> confirmation;

                    // If confirmation is not 'Y', cancel the cancellation process
                    if (toupper(confirmation) != 'Y')
                    {
                        cout << "Ticket cancellation cancelled. No changes made." << endl;
                        return;
                    }

                    // Cancel the ticket by clearing passenger name and ticket ID
                    bus[i].seat[row][col].passengerName.clear();
                    bus[i].seat[row][col].ticketID.clear();
                    cout << "Ticket for seat number " << seatNumber << " cancelled successfully." << endl;

                    ticketFound = true;
                    break;
                }
            }
            if (ticketFound)
                break;
        }
        if (ticketFound)
            break;
    }

    // If ticket not found, inform the user
    if (!ticketFound)
    {
        cout << "Ticket with ID " << ticketID << " not found." << endl;
    }
}

int main()
{
    int choice;                  // Variable to store user's choice
    BusReservationSystem system; // Object of BusReservationSystem class

    // Main program loop
    while (true)
    {
        // Display menu options
        vline('*', 75);
        cout << "\n\n";
        cout << "\t\t\t1. Add new Bus Details\n\t\t\t"
             << "2. Reserve your seats\n\t\t\t"
             << "3. Show the available seats in a bus\n\t\t\t"
             << "4. Buses Available Now\n\t\t\t"
             << "5. Delete Bus\n\t\t\t"
             << "6. Show Ticket Info\n\t\t\t"
             << "7. Cancel Ticket\n\t\t\t"
             << "8. Exit\n";
        vline('*', 75);

        // Prompt the user to enter their choice
        cout << "\n\t\t\tEnter your choice:-> ";

        // Check if the input is an integer
        if (!(cin >> choice))
        {
            // Clear the error state
            cin.clear();
            // Ignore the rest of the input buffer
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            // Display an error message
            cout << "Invalid input. Please enter a valid integer." << endl;
            continue; // Restart the loop
        }

        // Clear input buffer
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer

        // Handle non-numeric choices
        if (choice < 1 || choice > 8)
        {
            cout << "Invalid choice. Please enter a number between 1 and 8." << endl;
            continue; // Restart the loop
        }

        vline('*', 75);

        // Process user's choice
        switch (choice)
        {
        case 1:
            system.addNewBus(); // Add new bus details
            break;
        case 2:
            system.allotmentOfSeatToPassenger(); // Reserve seats
            break;
        case 3:
            system.showAvailableBusSeats(); // Show available seats in a bus
            break;
        case 4:
            system.showAvailableBuses(); // Show available buses
            break;
        case 5:
            system.deleteBus(); // Delete a bus
            break;
        case 6:
            system.showTicketInfo(); // Show ticket information
            break;
        case 7:
            system.cancelTicket(); // Cancel a ticket
            break;
        case 8:
            cout << "Thank You ... Visit Again!" << endl; // Exit the program
            return 0;
        default:
            cout << "Invalid choice. Please enter a valid option." << endl; // Handle invalid choice
        }
    }

    return 0;
}
