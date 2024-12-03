#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <limits>
#include <cstdlib> // For rand()
#include <ctime>   // For seeding rand()
using namespace std;

// Base class: User
class User
{
protected:
    int id;
    string name;
    double rating;
    int totalRatings; // To track the number of ratings

public:
    User(int id, string name)
        : id(id), name(name), totalRatings(1) {}

    virtual void display() const
    {
        cout << "ID: " << id << ", Name: " << name << endl;
    }

    int getId() const { return id; }

    void setName(const string &newName)
    {
        name = newName;
    }
};

// Rider class
class Rider : public User
{
public:
    Rider(int id, string name)
        : User(id, name) {}

    void requestRide()
    {
        cout << "Ride requested by Rider: " << name << " from DCRUST MURTHAL." << endl;
    }
};

// Driver class


class Driver : public User
{
public:
    Driver(int id, string name) : User(id, name)
    {
        // Generate a random rating between 3.0 and 5.0
        srand(time(nullptr)); // Seed random number generator
        rating = 3.0 + static_cast<double>(rand()) / RAND_MAX * (5.0 - 3.0);
    }

    void acceptRide()
    {
        cout << "Ride accepted by Driver: " << name << endl;
    }

    void updateRating(double newRating)
    {
        rating = ((rating * totalRatings) + newRating) / (totalRatings + 1);
        totalRatings++;
    }

    void display() const override
    {
        cout << "Driver - ID: " << id << ", Name: " << name << ", Rating: " << rating << endl;
    }
};

// Ride class
class Ride
{
    Rider *rider;
    Driver *driver;
    string pickupLocation;
    string dropOffLocation;
    double fare;
    string status;

public:
    Ride(Rider *r, Driver *d, string pickup, string dropoff, double fare)
        : rider(r), driver(d), pickupLocation(pickup), dropOffLocation(dropoff), fare(fare), status("Requested") {}

    void calculateFare()
    {
        cout << "Fare for the selected destination: $" << fare << endl;
    }

    void startRide()
    {
        status = "In Progress";
        cout << "Ride started to " << dropOffLocation << "!" << endl;
    }

    void completeRide()
    {
        status = "Completed";
        cout << "Ride completed to " << dropOffLocation << "! Fare: $" << fare << endl;
    }

    bool isCompleted() const
    {
        return status == "Completed";
    }

    Driver *getDriver()
    {
        return driver;
    }
};

// Structure to represent a destination with its fare
struct Destination
{
    string name;
    double fare;
    double time;
};

// Function to allow user to register as Rider
Rider *registerRider(int id, Rider *existingRider)
{

    string name;
    cout << "Enter your name (or type 'exit' to quit): ";
    getline(cin, name);

    if (name == "exit")
    {
        return nullptr;
    }

    //  / Initial rating for the rider
    return new Rider(id, name);
}

// Function to change Rider's name
void changeRiderName(Rider *rider)
{
    if (rider == nullptr)
    {
        cout << "No Rider is currently registered." << endl;
        return;
    }

    string newName;
    cout << "Enter the new name for the Rider: ";
    getline(cin, newName);
    rider->setName(newName);

    cout << "Rider's name updated successfully!" << endl;
}

// Function to register a driver
Driver *registerDriver()
{
    static int id = 1004; // Start from 1001 and increment for each new driver
    string name;

    cout << "Enter your name to register as a driver (or type 'exit' to quit): ";
    getline(cin, name);

    if (name == "exit")
    {
        return nullptr;
    }

    double initialRating = 5.0; // Initial rating for the driver
    Driver *newDriver = new Driver(id, name);
    id++; // Increment the static ID for the next driver
    return newDriver;
}

// Function to choose a destination from a predefined list
Destination chooseDestination()
{
    vector<Destination> destinations = {
        {"Mall", 15.0, 12},
        {"Airport", 25.0, 30},
        {"Train Station", 10.0, 25},
        {"Campus", 12.5, 10},
        {"Park", 8.0, 8},
        {"Hostel", 10.0, 6}};

    cout << "Available Destinations:" << endl;
    for (size_t i = 0; i < destinations.size(); ++i)
    {
        cout << i + 1 << ". " << destinations[i].name << " - $" << destinations[i].fare << " " << destinations[i].time << "Minutes" << endl;
    }
    cout << "0. Go back\n";
    cout << "e. Exit\n";

    string choice;
    while (true)
    {
        cout << "Select your destination (1-" << destinations.size() << ", 0 for back, 'e' for exit): ";
        cin >> choice;

        if (choice == "e")
        {
            exit(0);
        }
        else if (choice == "0")
        {
            throw "go_back";
        }
        else
        {
            int index = stoi(choice);
            if (index >= 1 && index <= static_cast<int>(destinations.size()))
            {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return destinations[index - 1];
            }
            else
            {
                cout << "Invalid choice. Please try again." << endl;
            }
        }
    }
}

// Function to choose a driver by ID
Driver *chooseDriverById(vector<Driver *> &drivers, const vector<Driver *> &defaultDrivers)
{
    cout << "Available Drivers:" << endl;

    // Display default drivers
    for (const Driver *driver : defaultDrivers)
    {
        driver->display();
    }

    // Display registered drivers
    for (Driver *driver : drivers)
    {
        driver->display();
    }

    cout << "0. Go back\n";
    cout << "e. Exit\n";

    string input;
    Driver *selectedDriver = nullptr;

    while (true)
    {
        cout << "Enter the Driver ID to select (0 for back, 'e' for exit): ";
        cin >> input;

        if (input == "e")
        {
            exit(0);
        }
        else if (input == "0")
        {
            throw "go_back";
        }
        else
        {
            int driverId = stoi(input);

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            // Check both default and registered drivers for matching ID
            for (const Driver *driver : defaultDrivers)
            {
                if (driver->getId() == driverId)
                {
                    selectedDriver = const_cast<Driver *>(driver); // Cast to non-const for use
                    break;
                }
            }

            if (selectedDriver == nullptr)
            {
                for (Driver *driver : drivers)
                {
                    if (driver->getId() == driverId)
                    {
                        selectedDriver = driver;
                        break;
                    }
                }
            }

            if (selectedDriver != nullptr)
            {
                break;
            }
            else
            {
                cout << "Invalid Driver ID. Please try again." << endl;
            }
        }
    }

    return selectedDriver;
}
// drivers list
void listDrivers(const vector<Driver *> &drivers, const vector<Driver *> &defaultDrivers)
{
    cout << "List of Drivers:" << endl;

    for (const Driver *driver : defaultDrivers)
    {
        driver->display();
    }

    for (const Driver *driver : drivers)
    {
        driver->display();
    }

    if (drivers.empty() && defaultDrivers.empty())
    {
        cout << "No drivers are currently registered." << endl;
    }
}
// list of riders
void listRiders(const vector<Rider *> &riders)
{
    cout << "List of Riders:" << endl;

    for (const Rider *rider : riders)
    {
        rider->display();
    }

    if (riders.empty())
    {
        cout << "No riders are currently registered." << endl;
    }
}
// Main function to simulate the ride-sharing app
int main()
{
    vector<Driver *> drivers;
    vector<Rider *> riders;

    // Create 3 default drivers
    vector<Driver *> defaultDrivers = {
        new Driver(1001, "Driver 1"),
        new Driver(1002, "Driver 2"),
        new Driver(1003, "Driver 3")};

    int userIdCounter = 1;
    int driverIdCounter = 2001; // Start IDs for registered drivers after default ones
    Rider *currentRider = nullptr;
    Ride *currentRide = nullptr;
    bool riderRegistered = false; // New flag to track if a rider is registered

    while (true)
    {
        cout << "Welcome to the Ride-Sharing App!" << endl;

        // Conditionally display the Register as Rider option
        if (!riderRegistered)
        {
            cout << "1. Register as Rider" << endl;
        }

        cout << "2. Change Rider's Name" << endl;
        cout << "3. Register as Driver" << endl;
        cout << "4. List Drivers" << endl;
        cout << "5. List Riders" << endl;

        cout << "6. Book a Ride" << endl;
        cout << "7. Exit" << endl;

        int choice;
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        try
        {
            if (choice == 1 && !riderRegistered)
            {
                if (currentRide != nullptr && !currentRide->isCompleted())
                {
                    cout << "Cannot register a new rider while a ride is in progress." << endl;
                }
                else
                {
                    currentRider = registerRider(userIdCounter++, currentRider);
                    if (currentRider != nullptr)
                    {
                        riders.push_back(currentRider);
                        cout << "Rider registered successfully!" << endl;
                        riderRegistered = true;
                    }
                }
            }
            else if (choice == 2)
            {
                changeRiderName(currentRider);
            }
            else if (choice == 3)
            {
                Driver *newDriver = registerDriver();
                if (newDriver != nullptr)
                {
                    drivers.push_back(newDriver);
                    cout << "Driver registered successfully! ID: " << newDriver->getId() << endl;
                }
            }

            else if (choice == 4)
            {
                listDrivers(drivers, defaultDrivers);
            }
            else if (choice == 5)
            {
                listRiders(riders);
            }
            else if (choice == 6)
            {
                if (currentRider == nullptr)
                {
                    cout << "You must register a rider before booking a ride." << endl;
                }
                else
                {
                    Destination destination = chooseDestination();
                    Driver *selectedDriver = chooseDriverById(drivers, defaultDrivers);

                    if (selectedDriver != nullptr)
                    {
                        currentRide = new Ride(currentRider, selectedDriver, "DCRUST MURTHAL", destination.name, destination.fare);
                        currentRide->calculateFare();
                        currentRide->startRide();
                        currentRide->completeRide();

                        // Optionally update driver rating after ride completion
                        double rating;
                        cout << "Rate your driver (1-5): ";
                        cin >> rating;
                        selectedDriver->updateRating(rating);
                        cout << "Driver rated successfully!" << endl;
                    }
                }
            }
            else if (choice == 7)
            {
                cout << "Exiting the app. Thank you!" << endl;
                break;
            }
            else
            {
                cout << "Invalid choice. Please try again." << endl;
            }
        }
        catch (const char *msg)
        {
            if (string(msg) == "go_back")
            {
                // Handle the go_back case here if needed
            }
        }
    }

    // Cleanup: Delete dynamically allocated memory
    for (Driver *driver : drivers)
    {
        delete driver;
    }
    for (Rider *rider : riders)
    {
        delete rider;
    }
    for (Driver *driver : defaultDrivers)
    {
        delete driver;
    }

    delete currentRide;

    return 0;
}
