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
    }



}