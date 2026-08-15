#include "ScheduleMenu.hpp"



//Input validation function
int getIntegerInput(const string& message, int min, int max) {
    int value;
    while (true) {
        cout << message;
        if (cin >> value && value >= min && value <= max) {
            return value;
        }
        cout << "Invalid input. Please enter a number from " << min << " to " << max << ".\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

double getDoubleInput(const string& message, double min, double max) {
    double value;
    while (true) {
        cout << message;
        if (cin >> value && value >= min && value <= max) {
            return value;
        }
        cout << "Invalid input. Please enter an amount from " << fixed << setprecision(2) << min << " to " << max << ".\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

string getNonEmptyString(const string& message) {
    string value;
    while (true) {
        cout << message;
        getline(cin >> ws, value); // ws extracts leading whitespace/newlines safely
        if (!value.empty()) {
            return value;
        }
        cout << "Input cannot be empty. Please try again.\n";
    }
}

bool isValidDate(const string& date, bool allowPast) {
    // 1. Check length and slash positions
    if (date.length() != 10) return false;
    if (date[4] != '/' || date[7] != '/') return false;

    // 2. Check if all other characters are digits
    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) continue;
        if (!isdigit(date[i])) return false;
    }

    // 3. Extract year, month, and day as integers
    int year = stoi(date.substr(0, 4));
    int month = stoi(date.substr(5, 2));
    int day = stoi(date.substr(8, 2));

    // 4. Basic calendar range checks
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31) return false;

    int daysInMonth[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    // Leap year calculation for February
    if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) {
        daysInMonth[2] = 29;
    }

    if (day > daysInMonth[month]) return false;

    // 5. Check against the actual current date
    if (!allowPast) {
        time_t t = time(nullptr);
        tm nowTm;
        localtime_s(&nowTm, &t);
        tm* now = &nowTm;

        int currentYear = now->tm_year + 1900;
        int currentMonth = now->tm_mon + 1;
        int currentDay = now->tm_mday;

        // Convert both dates to an integer (e.g., 20260812) for easy comparison
        int inputDateInt = (year * 10000) + (month * 100) + day;
        int currentDateInt = (currentYear * 10000) + (currentMonth * 100) + currentDay;

        if (inputDateInt < currentDateInt) {
            return false; // Date is successfully rejected for being in the past
        }
    }

    return true;
}

string getValidDate(const string& message, bool allowPast) {
    string date;
    while (true) {
        cout << message;
        getline(cin >> ws, date);

        if (isValidDate(date, allowPast)) {
            return date;
        }

        if (!allowPast) {
            cout << "Invalid date! Please use YYYY/MM/DD format. The date cannot be in the past.\n";
        }
        else {
            cout << "Invalid date! Please ensure it is a real calendar date in YYYY/MM/DD format.\n";
        }
    }
}

void pauseScreen() {
    cout << "\nPress ENTER to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// --- NEW HELPER: Load from file ---
void loadSchedulesFromFile(vector<Schedule>& schedules) {
    ifstream inFile("Schedules.txt");
    if (!inFile.is_open()) {
        return; // File doesn't exist yet, which is fine for the first run
    }

    string line;
    while (getline(inFile, line)) {
        stringstream ss(line);
        string idStr, date, name, startStr, endStr, trainer, price, classCapacity, cancelStr;

        // Parse CSV columns
        getline(ss, idStr, ',');
        getline(ss, date, ',');
        getline(ss, name, ',');
        getline(ss, startStr, ',');
        getline(ss, endStr, ',');
        getline(ss, trainer, ',');
        getline(ss, price, ',');
        getline(ss, classCapacity, ',');
        getline(ss, cancelStr, ',');

        if (idStr.empty()) continue;

        // Rebuild the schedule object
        Schedule s;
        s.scheduleID = stoi(idStr);
        s.date = date;
        s.className = name;
        s.startTime = stoi(startStr);
        s.endTime = stoi(endStr);
        s.trainerName = trainer;
        s.price = stod(price);
        s.classCapacity = stoi(classCapacity);
        s.isCanceled = (cancelStr == "1"); // 1 means true, 0 means false

        schedules.push_back(s);
    }
    inFile.close();
}

// --- NEW HELPER: Save to file ---
void saveSchedulesToFile(const vector<Schedule>& schedules) {
    ofstream outFile("Schedules.txt");
    if (outFile.is_open()) {
        for (const Schedule& s : schedules) {
            outFile << s.scheduleID << ","
                << s.date << ","
                << s.className << ","
                << s.startTime << ","
                << s.endTime << ","
                << s.trainerName << ","
                << s.price << ","
                << s.classCapacity << ","
                << (s.isCanceled ? "1" : "0") << "\n";
        }
        outFile.close();
    }
    else {
        cout << "Error: Could not save to file.\n";
    }
}

void addschedule(vector<Schedule>& schedules) {
    Schedule newClass;

    newClass.scheduleID = schedules.empty() ? 100 : schedules.back().scheduleID + 1;
    newClass.isCanceled = false;


    cout << "\n--- Add New Schedule ---\n";

    newClass.className = getNonEmptyString("Enter schedule name: ");

    newClass.date = getValidDate("Enter schedule date (YYYY/MM/DD): ");

    newClass.startTime = getIntegerInput("Enter start time (eg. 1400): ", 1000, 2000);

    newClass.endTime = getIntegerInput("Enter end time (eg. 1600): ", 1000, 2000);
    newClass.trainerName = getNonEmptyString("Enter trainer name: ");
    newClass.price = getDoubleInput("Enter class fee (RM, e.g., 30.00): ", 0.0, 500.0);
    newClass.classCapacity = getIntegerInput("Enter the Class Capacity: ", 5, 30);

    if (newClass.startTime >= newClass.endTime) {
        cout << "Error: Start time must be before end time.\n";
        return;
    }

    if (hasConflict(schedules, newClass.date, newClass.startTime, newClass.endTime, -1)) {
        cout << "Error: This time slot is already taken by an active class.\n";
    }
    else {
        schedules.push_back(newClass);
        cout << "Schedule added successfully with ID: " << newClass.scheduleID << "\n";

        // --- NEW: Save immediately ---
        saveSchedulesToFile(schedules);
    }
}

void displayschedule(const vector<Schedule>& schedules) {
    if (schedules.empty()) {
        cout << "No schedules available to display.\n";
        return;
    }

    cout << "\n====================================================================================================\n";
    cout << "                                         ALL GYM SCHEDULES                                          \n";
    cout << "====================================================================================================\n";
    cout << left << setw(6) << "ID"
        << setw(13) << "Date"
        << setw(16) << "Class Name"
        << setw(8) << "Start"
        << setw(8) << "End"
        << setw(16) << "Trainer"
        << setw(10) << "Capacity"
        << right << setw(10) << "Fee (RM)"
        << "   " << left << setw(8) << "Status\n";
    cout << "-------------------------------------------------------------------------------\n";

    bool activeFound = false;

    cout << fixed << setprecision(2);

    for (const Schedule& s : schedules) {
        if (s.isCanceled) {
            continue;
        }

        cout << left << setw(6) << s.scheduleID
            << setw(13) << s.date
            << setw(16) << s.className
            << setw(8) << s.startTime
            << setw(8) << s.endTime
            << setw(16) << (s.trainerName.empty() ? "None" : s.trainerName)
            << setw(10) << s.classCapacity
            << right << setw(10) << s.price
            << "   " << left << setw(8) << "Active\n";

        activeFound = true;
    }

    if (!activeFound) {
        cout << "No active schedules available to display.\n";
    }
}

void searchschedule(const vector<Schedule>& schedules) {
    if (schedules.empty()) {
        cout << "No schedules available to search.\n";
        return;
    }

    string searchDate;
    cout << "\n--- Search Schedule by Date ---\n";
    searchDate = getValidDate("Enter the date you want to search (YYYY/MM/DD): ");

    bool found = false;

    for (const Schedule& s : schedules) {
        if (s.isCanceled) {
            continue;
        }

        if (s.date == searchDate) {
            if (!found) {
                cout << "\n====================================================================================================\n";
                cout << "                                     RESULTS FOR " << searchDate << "                                      \n";
                cout << "====================================================================================================\n";
                cout << left << setw(6) << "ID"
                    << setw(13) << "Date"
                    << setw(16) << "Class Name"
                    << setw(8) << "Start"
                    << setw(8) << "End"
                    << setw(16) << "Trainer"
                    << setw(10) << "Capacity"
                    << right << setw(10) << "Fee (RM)"
                    << "   " << left << setw(8) << "Status\n";
                cout << "----------------------------------------------------------------------------------------------------\n";
            }

            cout << fixed << setprecision(2);
            cout << left << setw(6) << s.scheduleID
                << setw(13) << s.date
                << setw(16) << s.className
                << setw(8) << s.startTime
                << setw(8) << s.endTime
                << setw(16) << (s.trainerName.empty() ? "None" : s.trainerName)
                << setw(10) << s.classCapacity
                << right << setw(10) << s.price
                << "   " << left << setw(8) << "Active\n";

            found = true;
        }
    }

    if (!found) {
        cout << "No active classes found on " << searchDate << ".\n";
    }
}

void updateschedule(vector<Schedule>& schedules) {
    if (schedules.empty()) {
        cout << "No schedules available to update.\n";
        return;
    }

    int searchID;
    cout << "\n--- Update Schedule ---\n";
	searchID = getIntegerInput("Enter the Schedule ID you want to update: ", 1000, 9999);

    bool found = false;

    for (Schedule& s : schedules) {
        if (s.scheduleID == searchID) {
            found = true;
            char updateChoice;
            bool isModified = false;

            do {
                cout << fixed << setprecision(2);
                cout << "\n--- Updating Schedule ID: " << s.scheduleID << " ---\n";
                cout << "Current Details:\n";
                cout << "1. Class Name : " << s.className << "\n";
                cout << "2. Date       : " << s.date << "\n";
                cout << "3. Start Time : " << s.startTime << "\n";
                cout << "4. End Time   : " << s.endTime << "\n";
                cout << "5. Trainer    : " << (s.trainerName.empty() ? "None" : s.trainerName) << "\n";
                cout << "6. Fee (RM)   : RM " << s.price << "\n";
                cout << "7. Capacity   : " << s.classCapacity << "\n";
                cout << "0. Finish & Save Changes\n";
                cout << "--------------------------------\n";
                cout << "Enter choice: ";
                cin >> updateChoice;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                string tempDate;
                int tempStart, tempEnd;

                switch (updateChoice) {
                case '1' :
                    s.className = getNonEmptyString("Enter new schedule name: ");
                    isModified = true;
                    cout << "Class name updated successfully!\n";
                    break;

                case '2':

                    tempDate = getValidDate("Enter new schedule date (YYYY/MM/DD): ");

                    if (hasConflict(schedules, tempDate, s.startTime, s.endTime, searchID)) {
                        cout << "Error: The current time slot (" << s.startTime << "-" << s.endTime
                            << ") conflicts with another class on " << tempDate << ". Update failed.\n";
                    }
                    else {
                        s.date = tempDate;
                        isModified = true;
                        cout << "Date updated successfully!\n";
                    }
                    break;

                case '3':

                    tempStart = getIntegerInput("Enter new start time (eg. 1400): ", 0, 2400);

                    if (tempStart >= s.endTime) {
                        cout << "Error: Start time must be before the current end time (" << s.endTime << "). Update failed.\n";
                    }
                    else if (hasConflict(schedules, s.date, tempStart, s.endTime, searchID)) {
                        cout << "Error: This new start time conflicts with another class. Update failed.\n";
                    }
                    else {
                        s.startTime = tempStart;
                        isModified = true;
                        cout << "Start time updated successfully!\n";
                    }

                    break;
                case '4':

                    tempEnd = getIntegerInput("Enter new end time (eg. 1600): ", 0, 2400);

                    if (s.startTime >= tempEnd) {
                        cout << "Error: End time must be after the current start time (" << s.startTime << "). Update failed.\n";
                    }
                    else if (hasConflict(schedules, s.date, s.startTime, tempEnd, searchID)) {
                        cout << "Error: This new end time conflicts with another class. Update failed.\n";
                    }
                    else {
                        s.endTime = tempEnd;
                        isModified = true;
                        cout << "End time updated successfully!\n";
                    }

                    break;

                case '0':

                    if (isModified) {
                        saveSchedulesToFile(schedules);
                        cout << "\nAll changes saved to file successfully!\n";
                    }
                    else {
                        cout << "\nNo changes were made.\n";
                    }

                    break;

                default:
                    cout << "Invalid choice. Please try again.\n";
                }

            } while (updateChoice != '0');

            break;
        }
    }

    if (!found) {
        cout << "Error: Schedule ID " << searchID << " not found.\n";
    }
}

//void assigntrainer(vector<Schedule>& schedules) {
//    if (schedules.empty()) {
//        cout << "No schedules available to assign a trainer to.\n";
//        return;
//    }
//
//    int searchID;
//    cout << "\n--- Assign Trainer ---\n";
//    searchID = getIntegerInput("Enter the Schedule ID to assign a trainer: ", 100, 9999);
//
//
//    bool found = false;
//
//    for (Schedule& s : schedules) {
//        if (s.scheduleID == searchID) {
//            found = true;
//
//            if (s.isCanceled) {
//                cout << "Error: Cannot assign a trainer to a canceled schedule.\n";
//            }
//            else {
//                cout << "Current Trainer: " << (s.trainerName.empty() ? "None" : s.trainerName) << "\n";
//
//                s.trainerName = getNonEmptyString("Enter new trainer name: ");
//
//                cout << "Trainer assigned successfully to Schedule ID " << searchID << "!\n";
//
//                // --- NEW: Save changes ---
//                saveSchedulesToFile(schedules);
//            }
//            break;
//        }
//    }
//
//    if (!found) {
//        cout << "Error: Schedule ID " << searchID << " not found.\n";
//    }
//}

void cancelschedule(vector<Schedule>& schedules) {
    if (schedules.empty()) {
        cout << "No schedules available to cancel.\n";
        return;
    }

    int searchID;
    cout << "\n--- Cancel Schedule ---\n";
    searchID = getIntegerInput("Enter the Schedule ID you want to cancel: ", 100, 99999);

    bool found = false;

    for (Schedule& s : schedules) {
        if (s.scheduleID == searchID) {
            found = true;

            if (s.isCanceled) {
                cout << "Schedule ID " << searchID << " is already marked as canceled.\n";
            }
            else {
                s.isCanceled = true;
                cout << "Schedule ID " << searchID << " has been successfully canceled!\n";

                // --- NEW: Save changes ---
                saveSchedulesToFile(schedules);
            }
            break;
        }
    }

    if (!found) {
        cout << "Error: Schedule ID " << searchID << " not found.\n";
    }
}

void deleteSchedule(vector<Schedule>& schedules) {

}

bool hasConflict(const vector<Schedule>& schedules, string date, int startTime, int endTime, int excludeID) {
    for (const Schedule& s : schedules) {
        if (!s.isCanceled && s.date == date && s.scheduleID != excludeID) {
            if ((startTime >= s.startTime && startTime < s.endTime) ||
                (endTime > s.startTime && endTime <= s.endTime) ||
                (startTime <= s.startTime && endTime >= s.endTime)) {
                return true;
            }
        }
    }
    return false;
}