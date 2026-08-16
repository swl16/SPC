#pragma once

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <cctype>
#include <ctime>
#include <algorithm>
using namespace std;

struct Schedule {
    int scheduleID;
    string date;         // Format: YYYY-MM-DD
    int startTime;       // 24-hour format e.g., 1400 (2:00 PM)
    int endTime;         // 24-hour format e.g., 1500 (3:00 PM)
    string className;
    string trainerName;
    double price;
    int classCapacity;
    bool isCanceled;
    
};

struct ClassBooking {
    int bookingID;
    string username;
    int scheduleID;
    string bookingDate;
};

int getIntegerInput(const string& message, int min, int max);
double getDoubleInput(const string& message, double min, double max);
string getNonEmptyString(const string& message);
double getPositiveDouble(const string& message);
bool isValidDate(const string& date, bool allowPast);
string getValidDate(const string& message, bool allowPast = false);
void pauseScreen();

void addschedule(vector<Schedule>& schedules);
void displayschedule(const vector<Schedule>& schedules);
void searchschedule(const vector<Schedule>& schedules);
void updateschedule(vector<Schedule>& schedules);
void cancelschedule(vector<Schedule>& schedules);
void deleteSchedule(vector<Schedule>& schedules);
bool hasConflict(const vector<Schedule>& schedules, string date, int startTime, int endTime, int excludeID = -1);
void loadSchedulesFromFile(vector<Schedule>& schedules);
void saveSchedulesToFile(const vector<Schedule>& schedules);

int generatePlanID(const string& filename);
void saveMembershipPlans(const string& filename, const vector<MembershipPlanRecord>& plans);
