#include<iostream>
#include<iomanip>
#include<vector>
#include<string>
#include<fstream>
#include <sstream>
#include <ctime>

#include"User.hpp"
#include"booking.h"

using namespace std;

int generateBookingID(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) return 1001; // Default start ID

    int lastID = 1000;
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        int currentID;
        if (ss >> currentID) {
            lastID = currentID;
        }
    }
    file.close();
    return lastID + 1;
}

void bookClass(Member member) {
    ifstream cFile("classes.txt");
	ifstream classFile("availableClasses.txt");

    if (!classFile.is_open() || !cFile.is_open()) {
        cerr << "Error opening availableClasses.txt!" << endl;
        return;
    }

    vector<Class> catalog;
    Class c;

    while (cFile >> c.classID >> c.name >> c.coach.trainerID >> c.coach.name >> c.price) {
        catalog.push_back(c);
    }
    1001 Yoga 0001 Jason 30.00
    cFile.close();

    vector<string> validDates;
    time_t now = time(nullptr);

    for (int i = 0; i <= 2; ++i) {
        time_t futureTime = now + (i * 86400); // 86400 seconds = 1 day
        tm ltm;

        #ifdef _WIN32
        localtime_s(&ltm, &futureTime); // Safe version for MSVC / Visual Studio
        #else
        ltm = *localtime(&futureTime);
        #endif

        char buffer[20];
        strftime(buffer, sizeof(buffer), "%Y/%m/%d", &ltm);
        validDates.push_back(string(buffer));
    }

    vector<AvailableClass> availableClasses;
    AvailableClass ac;

    while (classFile >> ac.classType.classID >> ac.date >> ac.time >> ac.MaxCapacity) {
        bool isValidDate = false;
        
        for (const string& validDate : validDates) {
            if (ac.date == validDate) {
                isValidDate = true;
                break;
            }
        }

        if (isValidDate) {
            for (const auto& item : catalog) {
                if (item.classID == ac.classType.classID) {
                    ac.classType = item;
                    availableClasses.push_back(ac);
                    break;
                }
            }
        }
    } 
    classFile.close();
    
    if (availableClasses.empty()) {
        cout << "No classes available at the moment." << endl;
        return;
    }

    int dateChoice;
    cout << "================================================" << endl;
    cout << "                  CLASS BOOKING                 " << endl;
    cout << "================================================" << endl << endl;

    cout << "STEP 1 :" << endl;
    
    do {
        int i = 1;
        cout << "Select Date (Book up to 2 days ahead, including today.)" << endl;
        for (const string& validDate : validDates) {
            cout << i << ". " << validDate << endl;
            i++;
        }

        cout << "------------------------------------------------" << endl;
        cout << "Enter your choice (or '0' to return to menu) : ";

        if (!(cin >> dateChoice)) { // Input validation for non-numeric input
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Please enter a number." << endl;
            continue;
        }

        if (dateChoice == 0) {
            cout << "Returning to menu..." << endl;
            // call main menu func
            break;
        }

        if (dateChoice < 1 || dateChoice > static_cast<int>(validDates.size())) {
            cout << "Invalid option! Please select a valid date number." << endl;
            continue;
        }

        string selectedDate = validDates[choice - 1];
        
        vector<AvailableClass> classesForSelectedDate;
        for (const auto& item : availableClasses) {
            if (item.date == selectedDate) {
                classesForSelectedDate.push_back(item);
            }
        }

        if (classesForSelectedDate.empty()) {
            cout << "\nNo classes available on " << selectedDate << ". Please pick another date." << endl;
            continue;
        }

        cout << "\n------------------------------------------------" << endl;
        cout << "STEP 2 :" << endl;
        cout << "Select Start Time" << endl;
        cout << "------------------------------------------------" << endl;
        
        cout << left << setw(5) << "No."
                        << left << setw(15) << "Class Name"
                        << left << setw(12) << "Coach"
                        << left << setw(8) << "Time"
                        << left << setw(10) << "Capacity"
                        << right << setw(10) << "Fee (RM)" << endl;
            
        cout << "------------------------------------------------" << endl;
        cout << fixed << setprecision(2);

        for (size_t idx = 0; idx < classesForSelectedDate.size(); ++idx) {
            cout << left << setw(5) << (idx + 1)
                << left << setw(15) << classesForSelectedDate[idx].classType.name
                << left << setw(12) << classesForSelectedDate[idx].classType.coach.name
                << left << setw(8) << classesForSelectedDate[idx].time
                << left << setw(10) << classesForSelectedDate[idx].MaxCapacity
                << right << setw(10) << classesForSelectedDate[idx].classType.price << endl; 
        }
        cout << "------------------------------------------------" << endl;
       
        int timeChoice;
        cout << "\nEnter Option No. (1-" << classesForSelectedDate.size() << ") to book (or '0' to back):";

        if (!(cin >> timeChoice)) { // Input validation for non-numeric input
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Please enter a number." << endl;
            continue;
        }

        if (timeChoice == 0) {
            continue;
        }

        if (timeChoice < 1 || timeChoice > static_cast<int>(classesForSelectedDate.size())) {
            cout << "Invalid class selection!" << endl;
            continue;
        }

        AvailableClass selectedClass = classesForSelectedDate[timeChoice - 1];

        ifstream checkFile("classBookings.txt");
        int bID, cID, currentCapacityCount = 0;
        string uName, bDate, bTime;
        bool alreadyBooked = false;

        if (checkFile.is_open()) {
            while (checkFile >> bID >> uName >> cID >> bDate >> bTime) {
                if (cID == selectedClass.classType.classID && bDate == selectedClass.date && bTime == selectedClass.time) {
                    currentCapacityCount++;
                    if (uName == member.loginInfo.usernames) {
                        alreadyBooked = true;
                    }
                }
            }
            checkFile.close();
        }

        if (alreadyBooked) {
            cout << "\n[ERROR] You have already booked " << selectedClass.classType.name
                << " on " << selectedClass.date << " at " << selectedClass.time << "!" << endl;
            continue;
        }

        if (currentCapacityCount >= selectedClass.MaxCapacity) {
            cout << "\n[ERROR] Class is FULL! Capacity limit reached." << endl;
            continue;
        }

        int newBookingID = generateBookingID("classBookings.txt");
       
        
        cout << "\n================================================" << endl;
        cout << "           CLASS BOOKING CONFIRMATION           " << endl;
        cout << "================================================" << endl;
        cout << "Booking ID  : " << newBookingID << endl;
        cout << "Class       : " << selectedClass.classType.name << endl;
        cout << "Coach       : " << selectedClass.classType.coach.name << endl;
        cout << "Date & Time : " << selectedClass.date << " @ " << selectedClass.time << endl;
        cout << "Fee Paid    : RM " << selectedClass.classType.price << endl;
        cout << "================================================" << endl;
        
        char confirm;
        cout << "\nConfirm booking? (Y/N): ";
        cin >> confirm;

        if (confirm == 'Y' || confirm == 'y') { 
            ClassBooking classBooked = { newBookingID, member.name, selectedClass, validDates[0] };
            //process to payment
            break;
        }
        else {
            cout << "Booking cancelled." << endl;
        }
   
    } while (true);



}

void bookTrainer() {

}

void viewBooking(Member member) {
    time_t now = time(nullptr);
    tm ltm;

#ifdef _WIN32
    localtime_s(&ltm, &now);
#else
    ltm = *localtime(&now);
#endif

    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y/%m/%d", &ltm);
    string todayDate = string(buffer);

    cout << "================================================" << endl;
    cout << "             FITNESS CLASS BOOKINGS             " << endl;
    cout << "================================================" << endl << endl;

    vector<Class> classCatalog = loadClassCatalog();
    vector<Trainer> trainerCatalog = loadTrainerCatalog();
    
    ifstream classFile("classBookings.txt");
    int bID, cID;
    string uName, bDate, bTime;
    bool foundClass = false;

    cout << "\n----------------UPCOMING CLASSES----------------" << endl;
    cout << left << setw(12) << "Booking ID"
        << left << setw(18) << "Class Name"
        << left << setw(15) << "Coach"
        << left << setw(12) << "Date"
        << left << setw(10) << "Time"
        << right << setw(10) << "Fee (RM)" << endl;
    cout << "------------------------------------------------" << endl;
    cout << fixed << setprecision(2);

    if (classFile.is_open()) {
        // classBookings.txt format: bookingID username classID date time
        while (classFile >> bID >> uName >> cID >> bDate >> bTime) {
            if (uName == member.loginInfo.usernames) {
                string className = "Unknown";
                string coachName = "Unknown";
                double originalPrice = 0.0;

                // Look up class details from catalog
                for (const auto& item : classCatalog) {
                    if (item.classID == cID) {
                        className = item.name;
                        coachName = item.coach.name;
                        originalPrice = item.price;
                        break;
                    }
                }

                // Premium members pay RM 0.00
                double chargedPrice = isPremium ? 0.00 : originalPrice;

                cout << left << setw(12) << bID
                    << left << setw(18) << className
                    << left << setw(15) << coachName
                    << left << setw(12) << bDate
                    << left << setw(10) << bTime
                    << right << setw(10) << chargedPrice << endl;
                foundClass = true;
            }
        }
        classFile.close();

        if (!foundClass) {
            cout << "No active fitness class reservations found." << endl;
        }
    }
}

void cancelBooking() {

}