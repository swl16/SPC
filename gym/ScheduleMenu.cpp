#include "ScheduleMenu.hpp"

void scheduleMenu() {
    vector<Schedule> schedules; // Vector to store schedules
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

void addschedule(vector<Schedule>& schedules) {
    Schedule newClass;

    // Auto-increment ID based on the last element, or start at 100
    newClass.scheduleID = schedules.empty() ? 100 : schedules.back().scheduleID + 1;
    newClass.isCanceled = false; // Always default to active when adding

    cin.ignore(); // Clear buffer in case menu input left an enter key
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

    // 1. Basic Time Validation
    if (newClass.startTime >= newClass.endTime) {
        cout << "Error: Start time must be before end time.\n";
        return;
    }

    // 2. Automated Clash Validation
    if (hasConflict(schedules, newClass.date, newClass.startTime, newClass.endTime)) {
        cout << "Error: This time slot is already taken by an active class.\n";
    }
    else {
        // 3. Final Decision
        schedules.push_back(newClass);
        cout << "Schedule added successfully with ID: " << newClass.scheduleID << "\n";
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

    bool activeFound = false; // Track if we actually print anything active

    for (const Schedule& s : schedules) {
        // Skip canceled classes so they do not show up in the display
        if (s.isCanceled) {
            continue;
        }

        cout << left << setw(5) << s.scheduleID
            << setw(15) << s.date
            << setw(15) << s.className
            << setw(10) << s.startTime
            << setw(10) << s.endTime
            << setw(15) << s.trainerName
            << "Active\n"; // We can hardcode "Active" here since canceled ones are skipped

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
        // Skip canceled classes
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
                << setw(15) << s.trainerName
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

            // Use temporary variables so we don't ruin the original data if there is a conflict
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

            // Validate the temporary data
            if (tempStart >= tempEnd) {
                cout << "Error: Start time must be before end time. Update failed.\n";
            }
            // Pass 'searchID' so it doesn't conflict with its old self
            else if (hasConflict(schedules, tempDate, tempStart, tempEnd, searchID)) {
                cout << "Error: This time slot conflicts with another class. Update failed.\n";
            }
            else {
                // Safe to apply updates!
                s.className = tempName;
                s.date = tempDate;
                s.startTime = tempStart;
                s.endTime = tempEnd;
                cout << "Schedule updated successfully!\n";
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

    // Loop through the vector to find the matching ID
    for (Schedule& s : schedules) {
        if (s.scheduleID == searchID) {
            found = true;

            // Prevent assigning trainers to soft-deleted classes
            if (s.isCanceled) {
                cout << "Error: Cannot assign a trainer to a canceled schedule.\n";
            }
            else {
                cout << "Current Trainer: " << (s.trainerName.empty() ? "None" : s.trainerName) << "\n";

                // Clear the buffer before using getline
                cin.ignore();
                cout << "Enter new trainer name: ";
                getline(cin, s.trainerName);

                cout << "Trainer assigned successfully to Schedule ID " << searchID << "!\n";
            }
            break; // Stop looping once we find and update the schedule
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

    // Loop through to find the matching ID
    for (Schedule& s : schedules) {
        if (s.scheduleID == searchID) {
            found = true;

            // Check if it is already canceled to prevent redundant actions
            if (s.isCanceled) {
                cout << "Schedule ID " << searchID << " is already marked as canceled.\n";
            }
            else {
                // Flip the boolean switch
                s.isCanceled = true;
                cout << "Schedule ID " << searchID << " has been successfully canceled!\n";
            }
            break; // Stop looping once we find it
        }
    }

    if (!found) {
        cout << "Error: Schedule ID " << searchID << " not found.\n";
    }
}

bool hasConflict(const vector<Schedule>& schedules, string date, int startTime, int endTime, int excludeID) {
    for (const Schedule& s : schedules) {
        // Check active schedules on the same date, ignoring the one we might be updating
        if (!s.isCanceled && s.date == date && s.scheduleID != excludeID) {

            // Overlap logic
            if ((startTime >= s.startTime && startTime < s.endTime) ||
                (endTime > s.startTime && endTime <= s.endTime) ||
                (startTime <= s.startTime && endTime >= s.endTime)) {
                return true; // Conflict found!
            }
        }
    }
    return false; // No conflicts
}