#pragma once

#include <string>
#include "User.hpp"

using namespace std;

struct Class {
    int classID;
    string Name;
    trainer couch;
    int MaxCapacity;
    double price;  
};

struct trainer {
    int TrainerID;
    string Name;
    double HourlyRate;
};

struct ClassBooking {
    int bookingID;
    string username;
    Class classBooked;
    string bookingDate; 
};

struct TrainerBooking {
    int bookingID;
    string username;
    trainer trainer;
    string bookingDate;
    string slotTime;    
};