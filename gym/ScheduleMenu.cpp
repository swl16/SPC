#include "ScheduleMenu.hpp"

void scheduleMenu() {
    vector<Schedule> schedules;

    // --- NEW: Load existing schedules from the file immediately ---
    loadSchedulesFromFile(schedules);

    int choice;

    do {
        cout << "\n=== Gym Schedule Management ===\n";
        cout << "1. Add Schedule\n";
        cout << "2. Display Schedules\n";
        cout << "3. Search Schedule by Date\n";
        cout << "4. Update Schedule\n";
        cout << "5. Cancel Schedule\n";
        cout << "6. Assign Trainer\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: addschedule(schedules); break;
        case 2: displayschedule(schedules); break;
        case 3: searchschedule(schedules); break;
        case 4: updateschedule(schedules); break;
        case 5: cancelschedule(schedules); break;
        case 6: assigntrainer(schedules); break;
        case 0: cout << "Exiting Schedule Module...\n"; break;
        default: cout << "Invalid choice. Try again.\n";
        }

    } while (choice != 0);
}

// --- NEW HELPER: Load from file ---
void loadSchedulesFromFile(vector<Schedule>& schedules) {
    ifstream inFile("Schedules.csv");
    if (!inFile.is_open()) {
        return; // File doesn't exist yet, which is fine for the first run
    }

    string line;
    while (getline(inFile, line)) {
        stringstream ss(line);
        string idStr, date, name, startStr, endStr, trainer, cancelStr;

        // Parse CSV columns
        getline(ss, idStr, ',');
        getline(ss, date, ',');
        getline(ss, name, ',');
        getline(ss, startStr, ',');
        getline(ss, endStr, ',');
        getline(ss, trainer, ',');
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
        s.isCanceled = (cancelStr == "1"); // 1 means true, 0 means false

        schedules.push_back(s);
    }
    inFile.close();
}

// --- NEW HELPER: Save to file ---
void saveSchedulesToFile(const vector<Schedule>& schedules) {
    ofstream outFile("Schedules.csv");
    if (outFile.is_open()) {
        for (const Schedule& s : schedules) {
            outFile << s.scheduleID << ","
                << s.date << ","
                << s.className << ","
                << s.startTime << ","
                << s.endTime << ","
                << s.trainerName << ","
                << (s.isCanceled ? "1" : "0") << "\n";
        }
        outFile.close();
    }
    else {
        cout << "Error: Could not save to Schedules.csv.\n";
    }
}

void addschedule(vector<Schedule>& schedules) {
    Schedule newClass;

    newClass.scheduleID = schedules.empty() ? 100 : schedules.back().scheduleID + 1;
    newClass.isCanceled = false;

    cin.ignore();
    cout << "\n--- Add New Schedule ---\n";
    cout << "Enter schedule name: ";
    getline(cin, newClass.className);

    cout << "Enter schedule date (YYYY/MM/DD): ";
    getline(cin, newClass.date);

    cout << "Enter start time (eg. 1400): ";
    cin >> newClass.startTime;

    cout << "Enter end time (eg. 1600): ";
    cin >> newClass.endTime;

    cin.ignore();
    cout << "Enter trainer name: ";
    getline(cin, newClass.trainerName);

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

    cout << "\n--- All Gym Schedules ---\n";
    cout << left << setw(5) << "ID"
        << setw(15) << "Date"
        << setw(15) << "Class Name"
        << setw(10) << "Start"
        << setw(10) << "End"
        << setw(15) << "Trainer"
        << "Status\n";
    cout << "-------------------------------------------------------------------------------\n";

    bool activeFound = false;

    for (const Schedule& s : schedules) {
        if (s.isCanceled) {
            continue;
        }

        cout << left << setw(5) << s.scheduleID
            << setw(15) << s.date
            << setw(15) << s.className
            << setw(10) << s.startTime
            << setw(10) << s.endTime
            << setw(15) << (s.trainerName.empty() ? "None" : s.trainerName)
            << "Active\n";

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
    cout << "Enter the date you want to search (YYYY/MM/DD): ";
    cin >> searchDate;

    bool found = false;

    for (const Schedule& s : schedules) {
        if (s.isCanceled) {
            continue;
        }

        if (s.date == searchDate) {
            if (!found) {
                cout << "\nResults for " << searchDate << ":\n";
                cout << left << setw(5) << "ID"
                    << setw(15) << "Date"
                    << setw(15) << "Class Name"
                    << setw(10) << "Start"
                    << setw(10) << "End"
                    << setw(15) << "Trainer"
                    << "Status\n";
                cout << "-------------------------------------------------------------------------------\n";
            }

            cout << left << setw(5) << s.scheduleID
                << setw(15) << s.date
                << setw(15) << s.className
                << setw(10) << s.startTime
                << setw(10) << s.endTime
                << setw(15) << (s.trainerName.empty() ? "None" : s.trainerName)
                << "Active\n";

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
    cout << "Enter the Schedule ID you want to update: ";
    cin >> searchID;

    bool found = false;

    for (Schedule& s : schedules) {
        if (s.scheduleID == searchID) {
            found = true;
            cout << "\nSchedule found! Enter new details below.\n";
            cin.ignore();

            string tempName, tempDate;
            int tempStart, tempEnd;

            cout << "Enter new schedule name: ";
            getline(cin, tempName);

            cout << "Enter new schedule date (YYYY/MM/DD): ";
            getline(cin, tempDate);

            cout << "Enter new start time ( eg. 1400 ): ";
            cin >> tempStart;

            cout << "Enter new end time ( eg. 1600 ): ";
            cin >> tempEnd;

            if (tempStart >= tempEnd) {
                cout << "Error: Start time must be before end time. Update failed.\n";
            }
            else if (hasConflict(schedules, tempDate, tempStart, tempEnd, searchID)) {
                cout << "Error: This time slot conflicts with another class. Update failed.\n";
            }
            else {
                s.className = tempName;
                s.date = tempDate;
                s.startTime = tempStart;
                s.endTime = tempEnd;
                cout << "Schedule updated successfully!\n";

                // --- NEW: Save updates ---
                saveSchedulesToFile(schedules);
            }
            break;
        }
    }

    if (!found) {
        cout << "Error: Schedule ID " << searchID << " not found.\n";
    }
}

void assigntrainer(vector<Schedule>& schedules) {
    if (schedules.empty()) {
        cout << "No schedules available to assign a trainer to.\n";
        return;
    }

    int searchID;
    cout << "\n--- Assign Trainer ---\n";
    cout << "Enter the Schedule ID to assign a trainer: ";
    cin >> searchID;

    bool found = false;

    for (Schedule& s : schedules) {
        if (s.scheduleID == searchID) {
            found = true;

            if (s.isCanceled) {
                cout << "Error: Cannot assign a trainer to a canceled schedule.\n";
            }
            else {
                cout << "Current Trainer: " << (s.trainerName.empty() ? "None" : s.trainerName) << "\n";

                cin.ignore();
                cout << "Enter new trainer name: ";
                getline(cin, s.trainerName);

                cout << "Trainer assigned successfully to Schedule ID " << searchID << "!\n";

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

void cancelschedule(vector<Schedule>& schedules) {
    if (schedules.empty()) {
        cout << "No schedules available to cancel.\n";
        return;
    }

    int searchID;
    cout << "\n--- Cancel Schedule ---\n";
    cout << "Enter the Schedule ID you want to cancel: ";
    cin >> searchID;

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