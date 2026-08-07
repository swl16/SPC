#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

using namespace std;

struct Schedule {
    int scheduleID;
    string date;         // Format: YYYY-MM-DD
    int startTime;       // 24-hour format e.g., 1400 (2:00 PM)
    int endTime;         // 24-hour format e.g., 1500 (3:00 PM)
    string className;
    string trainerName;
    bool isCanceled;
};

// This is the new "main" function for your specific module
void scheduleMenu();

// Your existing prototypes
void addschedule(vector<Schedule>& schedules);
void displayschedule(const vector<Schedule>& schedules);
void searchschedule(const vector<Schedule>& schedules);
void updateschedule(vector<Schedule>& schedules);
void cancelschedule(vector<Schedule>& schedules);
void assigntrainer(vector<Schedule>& schedules);
bool hasConflict(const vector<Schedule>& schedules, string date, int startTime, int endTime, int excludeID = -1);