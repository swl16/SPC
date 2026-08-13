#pragma once

#include <string>
#include "User.hpp"

using namespace std;


struct Class {
    int classID;
    string Name;
    Trainer coach;
    double price;
};

struct AvailableClass {
    Class classType;
    string date;
    string time;
    int MaxCapacity;
};

struct ClassBooking {
    int bookingID;
    string username;
    int scheduleID;
    string bookingDate; 
};

