#pragma once
#include<iostream>
#include<vector>

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

struct MembershipPlanRecord {
	int id;
	string planName;
	int duration;
	double price;
	string benefits;
};

// user login file
void registerUser(Member* members);
int loginUser(Member* members);
void resetPassword(Member* members);
void logoutUser();
void displaymenu();
void loadUser(Member* members);
void saveUser(Member* members);
void userLogin();
void clearScreen();


// user menu file
void displayUserMenu();
int loggedInMember(Member members[]);
void viewProfile(Member* members);
void editProfile(Member* members);
void userMenu(Member* members);
void pauseScreen();


// membership file
vector<MembershipPlanRecord> loadMembershipPlans(const string& filename);
string getCurrentDate();
bool hasActiveMembership(const string& username);
void viewMembershipPlan(Member member);
void registerMembershipPlan(Member member);
void renewMembership(Member member);
void membershipPlan(Member member);

// booking file
int generateBookingID(const string& filename);
void bookClass(Member member);
void viewBooking(Member member);
void cancelBooking(Member member);

// payment file
string getCurrentDateTime();
string generatePaymentID();
void saveMembership(string username, int planID, string planName, string startDate, string endDate, string status);
void savePayment(string paymentID, string username, int planID, string planName, double amount, string paymentDate, string paymentMethod);
void generateMemberReceipt(string paymentID, string username, MembershipPlanRecord plan, string paymentDate, string paymentMethod,
	string startDate, string endDate);
void membershipPayment(Member* members);

// attendance file



// fitness file
//void fitnessMenu(Member& member);

// admin file
void adminLogin();
void displayadminMenu();
void displayMembershipPlanMenu();
void displayClassMenu();
void displayAttendanceMenu();
void displayReportsMenu();
void viewAllMembers();
void addMembershipPlan();
void viewMembershipPlans();
void updateMembershipPlan();
void deleteMembershipPlan();
void adminMenu(Member* members, int userCount);








