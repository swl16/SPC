#pragma once
#include<iostream>
#include "FitnessTracking.hpp"

using namespace std;

const int MAX_USERS = 1000;

// login information
struct User {
	string usernames;
	string passwords;
};

//fitness information
struct FitnessRecord {
	double weight;
	double height;
	double bmi = 0.0;
	int workoutDuration = 0;
	int targetWorkoutMins = 0;
	double caloriesBurned = 0.0;
};

// member information
struct Member {
	User loginInfo;
	string name;
	int age;
	char gender;
	string phNo;
	string email;
	FitnessRecord fitness;
};

// payment
struct Payment {
	string paymentID;
	Member member;
	double amount;
	string paymentDate;
	string paymentMethod;
};

// user login
void registerUser(Member* members);
int loginUser(Member* members);
void resetPassword(Member* members);
void logoutUser();

void loadUser(Member* members);
void saveUser(Member* members);

void userLogin();

// user menu
void userMenu(Member* members);

// profile
void viewProfile(Member* members);
void editProfile(Member* members);

// membership
void viewMembershipPlan(Member member);
void membershipPlan(Member member);

// booking
void bookClass(Member member);
void viewBooking(Member member);
void cancelBooking();

// payment

// attendance

// fitness
void fitnessMenu(Member& member);

// admin
void adminLogin();
void adminMenu(Member* members, int userCount);

void displaymenu();
void displayadminMenu();


void pauseScreen();

