#include<iostream>
#include<iomanip>
#include<vector>
#include<string>
#include<fstream>
#include <sstream>
#include <ctime>

#include"User.hpp"
#include"ScheduleMenu.hpp"

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
    vector<Schedule> allSchedules;
    loadSchedulesFromFile(allSchedules);

    if (allSchedules.empty()) {
        cout << "\nNo gym schedules available at the moment." << endl;
        return;
    }

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

    vector<Schedule> availableClasses;

    for (const auto& s : allSchedules) {
        if (s.isCanceled) continue; 

        for (const string& d : validDates) {
            if (s.date == d) {
                availableClasses.push_back(s);
                break;
            }
        }
    }
    
    if (availableClasses.empty()) {
        cout << "No classes available for the next 3 days." << endl;
        return;
    }

    int dateChoice;
    cout << "================================================" << endl;
    cout << "                  CLASS BOOKING                 " << endl;
    cout << "================================================" << endl << endl;

    do {
        cout << "STEP 1 :" << endl;
        cout << "Select Date (Book up to 2 days ahead, including today.)" << endl;
        cout << "------------------------------------------------" << endl;
        for (size_t i = 0; i < validDates.size(); ++i) {
            cout << (i + 1) << ". " << validDates[i];
            if (i == 0) cout << " (Today)";
            cout << endl;
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

        string selectedDate = validDates[dateChoice - 1];
        
        vector<Schedule> classesForSelectedDate;
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
            << left << setw(8) << "ID"
            << left << setw(18) << "Class Name"
            << left << setw(15) << "Trainer"
            << left << setw(12) << "Time Slot" << endl;
            
        cout << "------------------------------------------------" << endl;
        for (size_t idx = 0; idx < classesForSelectedDate.size(); ++idx) {
            string timeRange = to_string(classesForSelectedDate[idx].startTime) + "-" + to_string(classesForSelectedDate[idx].endTime);
            cout << left << setw(5) << (idx + 1)
                << left << setw(8) << classesForSelectedDate[idx].scheduleID
                << left << setw(18) << classesForSelectedDate[idx].className
                << left << setw(15) << (classesForSelectedDate[idx].trainerName.empty() ? "None" : classesForSelectedDate[idx].trainerName)
                << left << setw(12) << timeRange << endl;
        }
        cout << "------------------------------------------------" << endl;
       
        int timeChoice;
        cout << "\nEnter Option No. (1-" << classesForSelectedDate.size() << ") to book (or '0' to back):";

        if (!(cin >> timeChoice) || timeChoice == 0) { 
            continue;
        }

        if (timeChoice < 1 || timeChoice > static_cast<int>(classesForSelectedDate.size())) {
            cout << "Invalid class selection!" << endl;
            continue;
        }

        Schedule selectedClass = classesForSelectedDate[timeChoice - 1];

        ifstream checkFile("classBookings.txt");
        int bID, cID;
        string uName;
        bool alreadyBooked = false;

        if (checkFile.is_open()) {
            while (checkFile >> bID >> uName >> cID ) {
                if (cID == selectedClass.scheduleID && uName == member.loginInfo.usernames) {
                    alreadyBooked = true;
                    break;
                }
            }
            checkFile.close();
        }

        if (alreadyBooked) {
            cout << "\n[ERROR] You have already booked " << selectedClass.className
                << " on " << selectedClass.date << " at " << selectedClass.startTime << "!" << endl;
            continue;
        }

        int newBookingID = generateBookingID("classBookings.txt");
       
        
        cout << "\n================================================" << endl;
        cout << "           CLASS BOOKING CONFIRMATION           " << endl;
        cout << "================================================" << endl;
        cout << "Booking ID  : " << newBookingID << endl;
        cout << "Schedule ID : " << selectedClass.scheduleID << endl;
        cout << "Class       : " << selectedClass.className << endl;
        cout << "Trainer     : " << (selectedClass.trainerName.empty() ? "None" : selectedClass.trainerName) << endl;
        cout << "Date        : " << selectedClass.date << endl;
        cout << "Time Slot   : " << selectedClass.startTime << " - " << selectedClass.endTime << endl;
        cout << "Fee Status  : " << "RM 0.00 (Premium Covered) Standard Rate" << endl;
        cout << "================================================" << endl;
        
        char confirm;
        cout << "\nConfirm booking? (Y/N): ";
        cin >> confirm;

        if (confirm == 'Y' || confirm == 'y') { 
            //process to payment
            break;
        }
        else {
            cout << "Booking cancelled." << endl;
        }
   
    } while (true);
}

void viewBooking(Member member) {
    vector<Schedule> allSchedules;
    loadSchedulesFromFile(allSchedules);

    ifstream classFile("classBookings.txt");
    if (!classFile.is_open()) {
        cout << "\nNo class booking records found." << endl;
        return;
    }

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

    
    int bID, sID;
    string uName;
    bool foundClass = false;

    cout << "\n---------------UPCOMING CLASSES----------------" << endl;
    cout << left << setw(12) << "Booking ID"
        << left << setw(12) << "Schedule ID"
        << left << setw(18) << "Class Name"
        << left << setw(15) << "Trainer"
        << left << setw(12) << "Date"
        << left << setw(12) << "Time Slot"
        << left << setw(10) << "Status" << endl;
    cout << "------------------------------------------------" << endl;
    cout << fixed << setprecision(2);

    while (classFile >> bID >> uName >> sID) {
        if (uName == member.loginInfo.usernames) {
            Schedule matchedSchedule;
            bool scheduleFound = false;

            for (const auto& s : allSchedules) {
                if (s.scheduleID == sID) {
                    matchedSchedule = s;
                    scheduleFound = true;
                    break;
                }
            }

            if (scheduleFound) {
                string timeStr = to_string(matchedSchedule.startTime) + "-" + to_string(matchedSchedule.endTime);
                string statusStr = matchedSchedule.isCanceled ? "CANCELED BY ADMIN" : "ACTIVE";

                cout << left << setw(12) << bID
                    << left << setw(12) << matchedSchedule.scheduleID
                    << left << setw(18) << matchedSchedule.className
                    << left << setw(15) << (matchedSchedule.trainerName.empty() ? "None" : matchedSchedule.trainerName)
                    << left << setw(12) << matchedSchedule.date
                    << left << setw(12) << timeStr
                    << left << setw(10) << statusStr << endl;
                foundClass = true;
            }
        }
    }
    classFile.close();

    if (!foundClass) {
        cout << "You currently have no active class bookings." << endl;
    }
}

void cancelBooking() {

}