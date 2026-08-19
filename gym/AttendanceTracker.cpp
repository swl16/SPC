#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <ctime>
#include <iomanip>

#include "User.hpp";

extern bool hasActiveMembership(const string& username);
extern string getCurrentDate();

string getCurrentTimeHHMM() {
    time_t now = time(nullptr);
    tm ltm;
#ifdef _WIN32
    localtime_s(&ltm, &now);
#else
    ltm = *localtime(&now);
#endif
    stringstream ss;
    ss << setfill('0') << setw(2) << ltm.tm_hour << setw(2) << ltm.tm_min;
    return ss.str();
}

void checkInMember(Member member) {
    cout << "\n--- Member Check-In ---\n";

    if (!hasActiveMembership(member.loginInfo.usernames)) {
        cout << "ERROR! You do not have an active membership.\n";
        cout << "Please purchase or renew a plan from the Main Menu first.\n";
        return;
    }

    string todayDate = getCurrentDate();
    string username = member.loginInfo.usernames;

    ifstream inFile("AttendanceHistory.txt");
    string line;
    if (inFile.is_open()) {
        while (getline(inFile, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string dStr, uName, inStr, outStr;

            getline(ss, dStr, ',');
            getline(ss, uName, ',');
            getline(ss, inStr, ',');
            getline(ss, outStr, ',');

            // If the user is already checked in today and hasn't checked out (-1)
            if (uName == username && dStr == todayDate && outStr == "-1") {
                cout << "[ERROR] You are already checked in today and haven't checked out!\n";
                inFile.close();
                return;
            }
        }
        inFile.close();
    }

    string checkInTime = getCurrentTimeHHMM();

    //SAVE CHECK-IN TO FILE IMMEDIATELY ---
    ofstream outFile("AttendanceHistory.txt");
    if (outFile.is_open()) {
        outFile << todayDate << "," << username << "," << checkInTime << ",-1\n";
        outFile.close();
    }

    cout << "Success! Member " << member.name << " checked in at " << checkInTime << " today.\n";
}

void checkOutMember(Member member) {

    cout << "\n--- Member Check-Out ---\n";

    string username = member.loginInfo.usernames;
    bool foundActiveSession = false;
    string checkInTimeStr = "";

    vector<string> fileLines;
    ifstream inFile("AttendanceHistory.txt");
    string line;

    if (inFile.is_open()) {
        while (getline(inFile, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string dStr, uName, inStr, outStr;

            getline(ss, dStr, ',');
            getline(ss, uName, ',');
            getline(ss, inStr, ',');
            getline(ss, outStr, ',');

            // Find their active session
            if (uName == username && outStr == "-1") {
                foundActiveSession = true;
                checkInTimeStr = inStr;
                string checkOutTime = getCurrentTimeHHMM();

                // Update this line with the real checkout time
                line = dStr + "," + uName + "," + inStr + "," + checkOutTime;

                cout << "Success! You checked in at " << inStr << " and checked out at " << checkOutTime << ".\n";
            }
            fileLines.push_back(line);
        }
        inFile.close();
    }

    if (foundActiveSession) {
        ofstream outFile("AttendanceHistory.txt");
        if (outFile.is_open()) {
            for (const string& l : fileLines) {
                outFile << l << "\n";
            }
            outFile.close();
        }
    }

    if (!foundActiveSession) {
        cout << "[ERROR] You don't have an active check-in session right now.\n";
    }
}

void viewMyAttendance(Member member) {
    cout << "\n==================================================\n";
    cout << "              MY ATTENDANCE HISTORY               \n";
    cout << "==================================================\n";
    cout << left << setw(15) << "Date"
        << left << setw(15) << "Check-In"
        << left << setw(15) << "Check-Out" << endl;
    cout << "--------------------------------------------------\n";

    ifstream inFile("AttendanceHistory.txt");
    string line;
    bool hasHistory = false;

    if (inFile.is_open()) {
        while (getline(inFile, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string dStr, uName, inStr, outStr;

            getline(ss, dStr, ',');
            getline(ss, uName, ',');
            getline(ss, inStr, ',');
            getline(ss, outStr, ',');

            if (uName == member.loginInfo.usernames) {
                hasHistory = true;
                if (outStr == "-1") outStr = "Still Inside";

                cout << left << setw(15) << dStr
                    << left << setw(15) << inStr
                    << left << setw(15) << outStr << endl;
            }
        }
        inFile.close();
    }

    if (!hasHistory) {
        cout << "No attendance records found.\n";
    }
    cout << "==================================================\n";
}

void attendanceMenu(Member member) {

    int choice;

    do {
        cout << "\n==================================================\n";
        cout << "               GYM ATTENDANCE MENU                \n";
        cout << "==================================================\n";
        cout << "1. Check-In\n";
        cout << "2. Check-Out\n";
        cout << "3. View My Attendance History\n";
        cout << "0. Return to Main Menu\n";
        cout << "--------------------------------------------------\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        switch (choice) {
        case 1:
            checkInMember(member);
            break;
        case 2:
            checkOutMember(member);
            break;
        case 3:
            viewMyAttendance(member);
            break;
        case 0:
            cout << "Returning Main Menu...\n";
            break;
        default:
            cout << "Invalid choice. Try again.\n";
        }

        if (choice != 0) {
            cout << "\nPress Enter to continue...";
            cin.ignore(1000, '\n');
            cin.get();
        }

    } while (choice != 0);
}