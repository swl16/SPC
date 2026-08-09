#pragma once

#include <string>
#include "User.hpp"

using namespace std;

struct Trainer {
    int TrainerID;
    string Name;
    double HourlyRate;
};

struct Class {
    int classID;
    string Name;
    Trainer coach;
    int MaxCapacity;
    double price;  
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
    Trainer assignedTrainer;
    string bookingDate;
    string slotTime;    
};