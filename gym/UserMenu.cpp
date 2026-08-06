#include<iostream>
#include<iomanip>
#include<vector>
#include<string>
#include<fstream>
#include<sstream>

#include"User.hpp"

using namespace std;

const int MAX_USERS = 1000;
int userCount = 0;
string loggedInUser = "";

void registerUser(User* users) {   // user registration 

	User registeringUser = User();

	string username, password;

	while (true) {
		cout << "Enter a new username (without space): ";
		cin >> username;

		bool usernameExists = false;
		for (int i = 0; i < userCount; i++) {
			if (users[i].usernames == username) {
				cout << "Username already exists. Please try again.\n";
				usernameExists = true;
				break;
			}
		}

		if (!usernameExists) {
			break; // Valid username found
		}
		cout << "\nPress ENTER to continue.";
		cin.ignore(numeric_limits<streamsize>::max(), '\n');    // Proper buffer clearing
		cin.get();
	}
	cout << "Enter a new password: ";
	cin >> password;

	registeringUser.usernames = username;
	registeringUser.passwords = password;
	users[userCount] = registeringUser;
	userCount++;

	cout << "Registration successful!\n";

}

int loginUser(User* users) {    // user login

	if (!loggedInUser.empty()) {
		cout << "You are already logged in as " << loggedInUser << ". Please log out first.\n";
		return 1;
	}

	string username, password;
	cout << "Enter username: ";
	cin >> username;
	cout << "Enter password: ";
	cin >> password;

	bool found = false;
	for (int i = 0; i < userCount; ++i) {
		if (users[i].usernames == username && users[i].passwords == password) {
			loggedInUser = username;
			found = true;
			break;
		}
	}

	if (found) {
		cout << "Login successful. Welcome, " << loggedInUser << "!\n";
		cout << "\nPress enter to continue.\n";
		cin.ignore();
		cin.get();
		return 1;
	}
	else {
		cout << "Invalid username or password. Please try again.\n\n";
		return 0;
	}
	return 0;
}

void loadUser(User* users) {
	ifstream file("user.txt");

	if (!file) {
		cout << "Error: Could not open file." << endl;
		return;
	}

	string line;
	while (getline(file, line) && userCount < MAX_USERS) {

		stringstream ss(line);
		User userInfo;

		getline(ss, userInfo.usernames, ',');
		getline(ss, userInfo.passwords, ',');

		users[userCount] = userInfo;
		userCount++;
	}

	file.close();
}

void saveUser(User* users) {
	ofstream file("user.txt");

	if (!file) {
		cout << "Error: Could not open file." << endl;
		return;
	}

	for (int i = 0; i < userCount; i++) {
		file << users[i].usernames << "," << users[i].passwords << endl;
	}

	file.close();

}


void userMenu() {

	char loginChoice;
	User user;
	string enteredUsername, enteredPassword;
	loadUser(&user);

	cout << "=================================" << endl;
	cout << "            USER MENU            " << endl;
	cout << "=================================" << endl;

	cout << "1. Register\n";
	cout << "2. Login\n";
	cout << "0. Exit\n";

	do {
		cout << "-------------------------";
		cout << "Enter choice : ";
		cin >> loginChoice;

		switch (loginChoice) {

		case '1':
			cout << "================================\n";
			cout << "            Register            \n";
			cout << "================================\n";

			registerUser(&user);
			saveUser(&user);
			break;


		case '2':
			cout << "================================\n";
			cout << "             Login              \n";
			cout << "================================\n";

			loginUser(&user);
			break;

		case '0':
			cout << "Exiting user menu. Thank you.\n";
			break;


		default:
			cout << "Invalid choice. Please try again.\n";

		}
	} while (loginChoice != '0');


}

