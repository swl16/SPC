#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>

#include "User.hpp"
using namespace std;

// --- DUMMY DATA FOR TESTING ---
// will delete this later
struct DummyMember {
    int memberID;
    bool isActive;
};
// ------------------------------

struct Attendance {
    int memberID;
    string date;         // Format: YYYY-MM-DD
    int checkInTime;     // 24-hour format e.g., 0900
    int checkOutTime;    // -1 means they haven't checked out yet
};

// Main entry point for this module
void attendanceMenu();

// The core features (we will write the code for these one by one)
void checkInMember(vector<Attendance>& attendanceRecords, const vector<DummyMember>& members);
void checkOutMember(vector<Attendance>& attendanceRecords);
void calculateDailyAttendance(const vector<Attendance>& attendanceRecords);
void identifyPeakHours(const vector<Attendance>& attendanceRecords);