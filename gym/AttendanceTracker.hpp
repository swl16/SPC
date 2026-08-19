#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>

#include "User.hpp"
using namespace std;

struct Attendance {
    string username;
    string date;         // Format: YYYY-MM-DD
    int checkInTime;     // 24-hour format e.g., 0900
    int checkOutTime;    // -1 means they haven't checked out yet
};

// Main entry point for this module
void attendanceMenu(Member member);

// The core features (we will write the code for these one by one)
void checkInMember(Member member);
void checkOutMember(Member member);
void viewMyAttendance(Member member);