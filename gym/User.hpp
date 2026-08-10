#pragma once
#include<iostream>

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
};

struct Payment {
	string paymentID;
	Member member;
	double amount;
	string paymentDate;
	string paymentMethod;
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

void adminLogin(Member* members, int userCount);
void adminMenu(Member* members, int userCount);
void displayadminMenu();
void displayMembershipPlanMenu();
void displayClassMenu();
void displayTrainerMenu();
void displayScheduleMenu();
void displayAttendanceMenu();
void displayReportsMenu();


void viewMembershipPlan(Member member);
void membershipPlan(Member member);

