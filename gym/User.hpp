#include<iostream>

using namespace std;

struct Member {
	string id;
	string name;
	int age;
	char gender;
	string phNo;
};

struct User {
	string usernames;
	string passwords;
};

void registerUser(User* users);
int loginUser(User* users);
void loadUser(User* users);
void saveUser(User* users);
void userMenu();
void adminMenu();
