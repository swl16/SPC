#include<iostream>

using namespace std;

struct Member {
	string name;
	int age;
	char gender;
	string phNo;
	string email;
};

struct User {
	string usernames;
	string passwords;
};

void registerUser(User* users);
int loginUser(User* users);
void loadUser(User* users);
void saveUser(User* users);
void userLogin();
void adminMenu();
