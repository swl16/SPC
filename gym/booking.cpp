#include<iostream>
#include<iomanip>
#include<vector>
#include<string>
#include<fstream>
#include <sstream>
#include <ctime>
#include <algorithm>

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

int getBookedCount(int scheduleID) {
    ifstream file("classBookings.txt");
    if (!file.is_open()) return 0;

    int count = 0;
    string line;

    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string bID, uName, sIDStr, bDate;

        getline(ss, bID, ',');
        getline(ss, uName, ',');
        getline(ss, sIDStr, ',');
        getline(ss, bDate, ',');

        try {
            if (!sIDStr.empty() && stoi(sIDStr) == scheduleID) {
                count++;
            }
        }
        catch (...) {
            continue;
        }
    }
    file.close();
    return count;
}

void viewClassSchedule() {
    vector<Schedule> allSchedules;
    loadSchedulesFromFile(allSchedules);

    if (allSchedules.empty()) {
        cout << "\nNo gym schedules available at the moment." << endl;
        pauseScreen();
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

    vector<Schedule> upcomingSchedules;

    for (const auto& s : allSchedules) {
        if (s.date < todayDate) {
            continue;
        }
        else {
            upcomingSchedules.push_back(s);
        }
    }

    cout << "\n================================================" << endl;
    cout << "               GYM CLASS TIMETABLE              " << endl;
    cout << "================================================" << endl;
    cout << "Today's Date: " << todayDate << endl;

    // 3. Display Upcoming / Today's Active Classes
    cout << "\n-------- [ UPCOMING & TODAY'S CLASSES ] --------" << endl;
    cout << left << setw(6) << "ID"
        << left << setw(13) << "Date"
        << left << setw(16) << "Class Name"
        << left << setw(14) << "Time"
        << left << setw(15) << "Trainer"
        << left << setw(12) << "Seats Left"
        << right << setw(10) << "Fee (RM)"
        << "   " << left << setw(15) << "Status" << endl;
    cout << "------------------------------------------------" << endl;
    cout << fixed << setprecision(2);

    if (upcomingSchedules.empty()) {
        cout << "No upcoming classes scheduled at the moment." << endl;
    }
    else {
        for (const auto& s : upcomingSchedules) {
            string timeRange = to_string(s.startTime) + " - " + to_string(s.endTime);
            int bookedSeats = getBookedCount(s.scheduleID);
            int remainingSeats = s.classCapacity - bookedSeats;
            if (remainingSeats < 0) remainingSeats = 0;

            string seatsStr = to_string(remainingSeats) + "/" + to_string(s.classCapacity);
            string statusStr;

            if (s.isCanceled) {
                statusStr = "CANCELED";
            }
            else if (remainingSeats == 0) {
                statusStr = "FULL";
            }
            else {
                statusStr = "AVAILABLE";
            }

            cout << left << setw(6) << s.scheduleID
                << left << setw(13) << s.date
                << left << setw(16) << s.className
                << left << setw(14) << timeRange
                << left << setw(15) << (s.trainerName.empty() ? "None" : s.trainerName)
                << left << setw(12) << seatsStr
                << right << setw(10) << s.price
                << "   " << left << setw(15) << statusStr << endl;
        }
    }
    cout << "================================================" << endl;
    pauseScreen();
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

        if (!(cin >> timeChoice)) { // Input validation for non-numeric input
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Please enter a number." << endl;
            continue;
        }

        if (timeChoice == 0) {
            cout << "Returning to menu..." << endl;
            // call main menu func
            break;
        }

        if (timeChoice < 1 || timeChoice > static_cast<int>(classesForSelectedDate.size())) {
            cout << "Invalid class selection!" << endl;
            continue;
        }

        Schedule selectedClass = classesForSelectedDate[timeChoice - 1];

        ifstream checkFile("classBookings.txt");
        ClassBooking classBooked;
        int currentCapacityCount = 0;
        bool alreadyBooked = false;
        string line;

        if (checkFile.is_open()) {
            while (getline(checkFile, line)) {
                if (line.empty()) continue;
                stringstream ss(line);
                string bID, cID;

                getline(ss, bID, ',');
                getline(ss, classBooked.username, ',');
                getline(ss, cID, ',');
                getline(ss, classBooked.bookingDate, ',');

                classBooked.bookingID = stoi(bID);
                classBooked.scheduleID = stoi(cID);
                if (classBooked.scheduleID == selectedClass.scheduleID) {
                    currentCapacityCount++;
                    
                    if (classBooked.username == member.loginInfo.usernames) {
                        alreadyBooked = true;
                    }
                }
            }
            checkFile.close();
        }

        if (alreadyBooked) {
            cout << "\n[ERROR] You have already booked " << selectedClass.className
                << " on " << selectedClass.date << " at " << selectedClass.startTime << "!" << endl;
            continue;
        }

        if (currentCapacityCount >= selectedClass.classCapacity) {
            cout << "\n[ERROR] Class is FULL! Capacity limit of " << selectedClass.classCapacity << " reached." << endl;
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
        cout << "Fee Status  : " << fixed << setprecision(2) << selectedClass.price << endl;
        cout << "================================================" << endl;
        
        char confirm;
        cout << "\nConfirm booking? (Y/N): ";
        cin >> confirm;

        if (confirm == 'Y' || confirm == 'y') { 
            //process to payment

            classPaymentProcess(member, selectedClass, newBookingID);

        }
        else {
            cout << "Booking cancelled." << endl;
        }

        cout << "\nPress Enter to return to the User Menu...";
        cin.ignore(1000, '\n');
        cin.get();
        return;
   
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

    struct BookingDisplay {
        int bookingID;
        int scheduleID;
        string className;
        string trainerName;
        string date;
        string timeStr;
        string status;
        string dateBooked;
    };

    vector<BookingDisplay> upcomingBookings;
    vector<BookingDisplay> pastBookings;

    string line;
    ClassBooking classBooked;

    while (getline(classFile, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string bID, cID;

            getline(ss, bID, ',');
            getline(ss, classBooked.username, ',');
            getline(ss, cID, ',');
            getline(ss, classBooked.bookingDate, ',');

            classBooked.bookingID = stoi(bID);
            classBooked.scheduleID = stoi(cID);
       
        if (classBooked.username == member.loginInfo.usernames) {
            Schedule matchedSchedule;
            bool scheduleFound = false;

            for (const auto& s : allSchedules) {
                if (s.scheduleID == classBooked.scheduleID) {
                    matchedSchedule = s;
                    scheduleFound = true;
                    break;
                }
            }

            if (scheduleFound) {
                string timeRange = to_string(matchedSchedule.startTime) + "-" + to_string(matchedSchedule.endTime);
                string statusStr = matchedSchedule.isCanceled ? "CANCELED BY ADMIN" : "ACTIVE";

                BookingDisplay bd = {
                    classBooked.bookingID,
                    matchedSchedule.scheduleID,
                    matchedSchedule.className,
                    (matchedSchedule.trainerName.empty() ? "None" : matchedSchedule.trainerName),
                    matchedSchedule.date,
                    timeRange,
                    statusStr,
                    classBooked.bookingDate
                };

                if (matchedSchedule.date < todayDate) {
                    pastBookings.push_back(bd);
                }
                else {
                    upcomingBookings.push_back(bd);
                }
            }
        }
    }
    classFile.close();

    cout << "===================================================================================" << endl;
    cout << "                              FITNESS CLASS BOOKINGS                               " << endl;
    cout << "===================================================================================" << endl;

    // Upcoming Bookings
    cout << "\n------------------------------- UPCOMING CLASSES ----------------------------------" << endl;
    cout << left << setw(12) << "Booking ID"
        << left << setw(12) << "Schedule ID"
        << left << setw(18) << "Class Name"
        << left << setw(15) << "Trainer"
        << left << setw(12) << "Date"
        << left << setw(12) << "Time Slot"
        << left << setw(18) << "Status" 
        << left << setw(12) << "Date Booked" << endl;
    cout << "-----------------------------------------------------------------------------------" << endl;

    if (upcomingBookings.empty()) {
        cout << "No upcoming class reservations found." << endl;
    }
    else {
        for (const auto& b : upcomingBookings) {
            cout << left << setw(12) << b.bookingID
                << left << setw(12) << b.scheduleID
                << left << setw(18) << b.className
                << left << setw(15) << b.trainerName
                << left << setw(12) << b.date
                << left << setw(12) << b.timeStr
                << left << setw(18) << b.status 
                << left << setw(12) << b.dateBooked << endl;
        }
    }

    // Past Bookings
    cout << "\n--------------------------------- PAST CLASSES ------------------------------------" << endl;
    cout << left << setw(12) << "Booking ID"
        << left << setw(12) << "Schedule ID"
        << left << setw(18) << "Class Name"
        << left << setw(15) << "Trainer"
        << left << setw(12) << "Date"
        << left << setw(12) << "Time Slot"
        << left << setw(18) << "Status" 
        << left << setw(12) << "Date Booked" << endl;
    cout << "-----------------------------------------------------------------------------------" << endl;

    if (pastBookings.empty()) {
        cout << "No past class reservations found." << endl;
    }
    else {
        for (const auto& b : pastBookings) {
            cout << left << setw(12) << b.bookingID
                << left << setw(12) << b.scheduleID
                << left << setw(18) << b.className
                << left << setw(15) << b.trainerName
                << left << setw(12) << b.date
                << left << setw(12) << b.timeStr
                << left << setw(18) << b.status 
                << left << setw(12) << b.dateBooked << endl;
        }
    }
    cout << "===================================================================================" << endl;

}

void cancelBooking(Member member) {
    ifstream classFile("classBookings.txt");
    if (!classFile.is_open()) {
        cout << "\nNo active bookings found.\n";
        return;
    }

    vector<ClassBooking> allBookings;
    vector<int> userBookingIDs;
    string line;

    while (getline(classFile, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        ClassBooking book;
        string bID, cID;

        getline(ss, bID, ',');
        getline(ss, book.username, ',');
        getline(ss, cID, ',');
        getline(ss, book.bookingDate, ',');

        book.bookingID = stoi(bID);
        book.scheduleID = stoi(cID);

        allBookings.push_back(book);
        if (book.username == member.loginInfo.usernames) {
            userBookingIDs.push_back(book.bookingID);
        }
    }
    classFile.close();

    if (userBookingIDs.empty()) {
        cout << "\nYou have no bookings to cancel.\n";
        return;
    }

    viewBooking(member);

    int targetBookingID = getIntegerInput("\nEnter Booking ID to cancel (or 0 to back): ", 1000, 1999);
    if (targetBookingID == 0) return;

    // Verify booking belongs to this member
    bool isOwned = false;
    for (int id : userBookingIDs) {
        if (id == targetBookingID) {
            isOwned = true;
            break;
        }
    }

    if (!isOwned) {
        cout << "\n[ERROR] Booking ID " << targetBookingID << " does not belong to your account.\n";
        return;
    }

    char confirm;
    cout << "Are you sure you want to cancel Booking ID " << targetBookingID << "? (Y/N): ";
    cin >> confirm;

    if (confirm != 'Y' && confirm != 'y') {
        cout << "Cancellation aborted.\n";
        return;
    }

    auto it = remove_if(allBookings.begin(), allBookings.end(), [&](const ClassBooking& b) {
        return b.bookingID == targetBookingID;
        });

    if (it != allBookings.end()) {
        allBookings.erase(it, allBookings.end());

        ofstream outFile("classBookings.txt");
        if (outFile.is_open()) {
            for (const auto& b : allBookings) {
                outFile << b.bookingID << "," << b.username << "," << b.scheduleID << "," << b.bookingDate << "\n";
            }
            outFile.close();
            cout << "\nBooking ID " << targetBookingID << " cancelled successfully!\n";
            cout << "The fee for booking will be return to your account.";
        }
    }
}


void bookingClass(Member member) {
    clearScreen();

    int choice;

    cout << "1. Book Class" << endl;
    cout << "2. View Class Schedule" << endl;
    cout << "0. Return to Main Menu" << endl;
    cout << "-------------------------" << endl;

    do {
        cout << "Enter choice: ";
        if (!(cin >> choice)) { // Input validation for non-numeric input
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Please enter a number." << endl;
            continue;
        }

        switch (choice) {
        case 1:
            bookClass(member);
            break;

        case 2:
            viewClassSchedule();
            break;

        case 0:
            cout << "Returning to menu..." << endl;
            return;
            // call main menu func
            break;

        default:
            cout << "Invalid Input. Please try again." << endl;
        }


    } while (true);
    
}


void viewingBooking(Member member) {
    clearScreen();

    int choice;

    cout << "1. View Bookings" << endl;
    cout << "2. Cancel Booking" << endl;
    cout << "0. Return to Main Menu" << endl;
    cout << "------------------------" << endl;

    do {

        cout << "Enter choice: ";
        if (!(cin >> choice)) { // Input validation for non-numeric input
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Please enter a number." << endl;
            continue;
        }

        switch (choice) {
        case 1:
            viewBooking(member);
            break;

        case 2:
            cancelBooking(member);
            break;

        case 0:
            cout << "Returning to menu..." << endl;
            return;
            // call main menu func
            break;

        default:
            cout << "Invalid Input. Please try again." << endl;
        }

    } while (true);
}