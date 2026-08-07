#include "AttendanceTracker.hpp"
#include <fstream>
#include <sstream>

void attendanceMenu() {
    vector<Attendance> attendanceRecords;

    // --- LOAD DUMMY MEMBERS ---
    vector<DummyMember> members;
    members.push_back({ 101, true });  // ID 101 is an Active member
    members.push_back({ 102, false }); // ID 102 is an Inactive member
    // --------------------------

    int choice;

    do {
        cout << "\n=== Gym Attendance Tracking ===\n";
        cout << "1. Member Check-In\n";
        cout << "2. Member Check-Out\n";
        cout << "3. Calculate Daily Attendance\n";
        cout << "4. Identify Peak Hours\n";
        cout << "0. Return to Main Menu\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            checkInMember(attendanceRecords, members);
            break;
        case 2:
            checkOutMember(attendanceRecords);
            break;
        case 3:
            calculateDailyAttendance(attendanceRecords);
            break;
        case 4:
            identifyPeakHours(attendanceRecords);
            break;
        case 0:
            cout << "Exiting Attendance Module...\n";
            break;
        default:
            cout << "Invalid choice. Try again.\n";
        }

    } while (choice != 0);
}

void checkInMember(vector<Attendance>& attendanceRecords, const vector<DummyMember>& members) {
    int inputID;
    cout << "\n--- Member Check-In ---\n";
    cout << "Enter Member ID: ";
    cin >> inputID;

    // 1. Verify Active Membership
    bool isFound = false;
    bool isActive = false;

    for (const DummyMember& m : members) {
        if (m.memberID == inputID) {
            isFound = true;
            isActive = m.isActive;
            break;
        }
    }

    if (!isFound) {
        cout << "Error: Member ID not found in the system.\n";
        return;
    }
    if (!isActive) {
        cout << "Error: Membership is currently inactive. Cannot check in.\n";
        return;
    }

    // 2. Prevent Repeated Check-In
    string todayDate;
    cout << "Enter today's date (YYYY-MM-DD): ";
    cin >> todayDate;

    for (const Attendance& a : attendanceRecords) {
        if (a.memberID == inputID && a.date == todayDate && a.checkOutTime == -1) {
            cout << "Error: Member is already checked in and hasn't checked out yet!\n";
            return;
        }
    }

    // 3. Process the Check-In
    Attendance newRecord;
    newRecord.memberID = inputID;
    newRecord.date = todayDate;

    cout << "Enter check-in time (24-hour format, eg. 0900): ";
    cin >> newRecord.checkInTime;

    newRecord.checkOutTime = -1;
    attendanceRecords.push_back(newRecord);

    //SAVE CHECK-IN TO FILE IMMEDIATELY ---
    ofstream outFile("AttendanceHistory.csv", ios::app);
    if (outFile.is_open()) {
        outFile << newRecord.date << "," << newRecord.memberID << "," << newRecord.checkInTime << ",-1\n";
        outFile.close();
    }

    cout << "Success! Member " << inputID << " checked in at " << newRecord.checkInTime << ".\n";
}

void checkOutMember(vector<Attendance>& attendanceRecords) {
    if (attendanceRecords.empty()) {
        cout << "No attendance records available.\n";
        return;
    }

    int inputID;
    cout << "\n--- Member Check-Out ---\n";
    cout << "Enter Member ID: ";
    cin >> inputID;

    bool foundActiveSession = false;

    // 1. Update the live vector
    for (Attendance& a : attendanceRecords) {
        if (a.memberID == inputID && a.checkOutTime == -1) {
            foundActiveSession = true;

            cout << "Member found! Checked in at: " << a.checkInTime << "\n";
            cout << "Enter check-out time (24-hour format, eg. 1100): ";
            cin >> a.checkOutTime;

            if (a.checkOutTime <= a.checkInTime) {
                cout << "Warning: Check-out time is before or same as check-in time.\n";
            }
            cout << "Success! Member " << inputID << " checked out at " << a.checkOutTime << ".\n";

            // REWRITE THE FILE WITH THE UPDATED TIME ---
            vector<string> fileLines;
            ifstream inFile("AttendanceHistory.csv");
            string line;

            if (inFile.is_open()) {
                // Read all lines and find the one to modify
                while (getline(inFile, line)) {
                    stringstream ss(line);
                    string dStr, idStr, inStr, outStr;

                    getline(ss, dStr, ',');
                    getline(ss, idStr, ',');
                    getline(ss, inStr, ',');
                    getline(ss, outStr, ',');

                    // If this is the exact line that needs updating
                    if (dStr == a.date && idStr == to_string(a.memberID) && outStr == "-1") {
                        // Rebuild the line with the new checkout time
                        line = dStr + "," + idStr + "," + inStr + "," + to_string(a.checkOutTime);
                    }
                    fileLines.push_back(line);
                }
                inFile.close();

                // Re-open in truncate mode (default) to overwrite the old file
                ofstream outFile("AttendanceHistory.csv");
                if (outFile.is_open()) {
                    for (const string& l : fileLines) {
                        outFile << l << "\n";
                    }
                    outFile.close();
                }
            }
            // ---------------------------------------------------
            break;
        }
    }

    if (!foundActiveSession) {
        cout << "Error: No active check-in found for Member ID " << inputID << ".\n";
    }
}

void calculateDailyAttendance(const vector<Attendance>& attendanceRecords) {
    string targetDate;
    cout << "\n--- Daily Attendance Report ---\n";
    cout << "Enter date to check (YYYY-MM-DD): ";
    cin >> targetDate;

    int dailyCount = 0;
    cout << "\nMembers present on " << targetDate << ":\n";

    // Read directly from the file (which now has both active and completed sessions)
    ifstream inFile("AttendanceHistory.csv");
    if (inFile.is_open()) {
        string line;
        while (getline(inFile, line)) {
            stringstream ss(line);
            string dateStr, idStr, inTimeStr, outTimeStr;

            getline(ss, dateStr, ',');
            getline(ss, idStr, ',');
            getline(ss, inTimeStr, ',');
            getline(ss, outTimeStr, ',');

            if (dateStr == targetDate) {
                cout << "- Member ID: " << idStr << " (In: " << inTimeStr;

                if (outTimeStr == "-1") {
                    cout << ", Out: Still inside)\n";
                }
                else {
                    cout << ", Out: " << outTimeStr << ")\n";
                }
                dailyCount++;
            }
        }
        inFile.close();
    }
    else {
        cout << "No history file found.\n";
    }

    if (dailyCount == 0) {
        cout << "No attendance recorded for this date.\n";
    }
    else {
        cout << "\nTotal daily attendance: " << dailyCount << " members.\n";
    }
}

void identifyPeakHours(const vector<Attendance>& attendanceRecords) {
    // Read the historical data file
    ifstream inFile("AttendanceHistory.csv");
    if (!inFile.is_open()) {
        cout << "No historical records found. Please check-out some members first.\n";
        return;
    }

    int hourCounts[24] = { 0 };
    string line;
    bool dataFound = false;

    // Loop through historical file and extract the check-in hour
    while (getline(inFile, line)) {
        stringstream ss(line);
        string dateStr, idStr, inTimeStr, outTimeStr;

        getline(ss, dateStr, ',');
        getline(ss, idStr, ',');
        getline(ss, inTimeStr, ',');
        getline(ss, outTimeStr, ',');

        if (!inTimeStr.empty()) {
            int inTime = stoi(inTimeStr);
            int hour = inTime / 100;

            if (hour >= 0 && hour < 24) {
                hourCounts[hour]++;
                dataFound = true;
            }
        }
    }
    inFile.close();

    if (!dataFound) {
        cout << "Not enough valid data to determine peak hours.\n";
        return;
    }

    // Find the hour with the highest count
    int peakHour = 0;
    int maxCount = 0;

    for (int i = 0; i < 24; i++) {
        if (hourCounts[i] > maxCount) {
            maxCount = hourCounts[i];
            peakHour = i;
        }
    }

    cout << "\n--- Historical Peak Hour Analysis ---\n";
    cout << "Based on all saved records, the peak hour for check-ins is around ";
    if (peakHour < 10) cout << "0";
    cout << peakHour << "00 hours with " << maxCount << " total historical check-ins.\n";
}