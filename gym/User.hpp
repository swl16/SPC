#include<iostream>

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
};



void registerUser(User* users);
int loginUser(User* users);
void loadUser(User* users);
void saveUser(User* users);
void userMenu();
void adminMenu();

void viewMembershipPlan(Member member);
void membershipPlan();

