#include<iostream>
#include "FitnessTracking.hpp"

using namespace std;

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


void registerUser(Member* members);
int loginUser(Member* members);
void loadUser(Member* members);
void saveUser(Member* members);
void userLogin();
void resetPassword(Member* members);
void logoutUser();
void displaymenu();
void clearScreen();
void adminMenu();

void viewMembershipPlan(Member member);
void membershipPlan();

