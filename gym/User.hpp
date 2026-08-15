#pragma once
#include<iostream>
#include "FitnessTracking.hpp"

using namespace std;

const int MAX_USERS = 1000;

struct User {
	string usernames;
	string passwords;
};

struct Member {
	User loginInfo;
	string name;
	int age;
	char gender;
	string phNo;
	string email;
	FitnessRecord fitness;
};

struct Payment {
	string paymentID;
	Member member;
	double amount;
	string paymentDate;
	string paymentMethod;
};

struct MembershipPlanRecord {
	int id;
	string planName;
	int duration;
	double price;
	string benefits;
};

void registerUser(Member* members);
int loginUser(Member* members);
void resetPassword(Member* members);
void logoutUser();

void loadUser(Member* members);
void saveUser(Member* members);

void userLogin();
void userMenu(Member* members);


void displaymenu();
void clearScreen();

void adminLogin();
void adminMenu(Member* members, int userCount);
void displayadminMenu();
void displayMembershipPlanMenu();
void addMembershipPlan();
void displayClassMenu();
void displayTrainerMenu();
//void displayScheduleMenu();
void displayAttendanceMenu();
void displayReportsMenu();

vector<MembershipPlanRecord> loadMembershipPlans(const string& filename);
void viewMembershipPlan(Member member);
void membershipPlan(Member member);
void bookClass(Member member);
void viewBooking(Member member);
void cancelBooking();

