#pragma once

#include <string>
#include "User.hpp"

using namespace std;

struct Trainer {
    int TrainerID;
    string Name;
    double HourlyRate;
};

struct AvailableTrainer {
    Trainer trainer;
    string date;
    string time;
};

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
    AvailableClass classBooked;
    string bookingDate; 
};

struct TrainerBooking {
    int bookingID;
    string username;
    AvailableTrainer assignedTrainer;
    string bookingDate;
    string slotTime;    
};