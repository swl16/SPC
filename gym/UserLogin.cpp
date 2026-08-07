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

	cout << "================================\n";
	cout << "            Register            \n";
	cout << "================================\n";

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
		cin.ignore();
		cout << "\nPress enter to continue.\n";
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
		return;
	}

	userCount = 0;

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

void resetPassword(User* users) {   // user reset password
	cout << "\n--- Password Reset ---\n";

	string username;
	cout << "Enter your username to reset the password: ";
	cin >> username;

	int userIndex = -1;
	for (int i = 0; i < userCount; ++i) {
		if (users[i].usernames == username) {
			userIndex = i;
			break;
		}
	}

	if (userIndex != -1) {
		string newPassword;
		cout << "Enter your new password: ";
		cin >> newPassword;
		users[userIndex].passwords = newPassword;
		cout << "Password for user '" << username << "' has been reset successfully.\n";
	}
	else {
		cout << "Username not found. Cannot reset password.\n";
	}
}


void logoutUser() {    //user logout
	cout << "\n--- User Logout ---\n";

	if (loggedInUser.empty()) {
		cout << "No user is currently logged in.\n";
	}
	else {
		cout << "User " << loggedInUser << " has been logged out successfully.\n";
		loggedInUser = "";
	}
}

void displaymenu() {
	if (loggedInUser.empty()) {
		cout << "Status: Not Logged In\n";
	}
	else {
		cout << "Status: Logged in as " << loggedInUser << "\n";
	}

	cout << "=================================" << endl;
	cout << "       USER AUTHENTICATION       " << endl;
	cout << "=================================" << endl;
	cout << "1. Register\n";
	cout << "2. Login\n";
	cout << "3. Forgot Password\n";
	cout << "4. Logout\n";
	cout << "0. Exit\n";
	cout << "-----------------------------------\n";
}

void clearScreen() {
#ifdef _WIN32 // Check if the system is Windows
	system("cls");
#else // Otherwise, assume it's a POSIX-compliant system (like Linux or macOS)
	system("clear");
#endif
}


void userLogin() {

	char loginChoice;
	User users[MAX_USERS];
	string enteredUsername, enteredPassword;
	loadUser(users);


	do {
		displaymenu();
		cout << "Enter choice : ";
		cin >> loginChoice;

		switch (loginChoice) {

		case '1':
			registerUser(users);
			saveUser(users);


		case '2':
			cout << "================================\n";
			cout << "             Login              \n";
			cout << "================================\n";

			loginUser(users);
			break;

		case '3':
			resetPassword(users);
			saveUser(users);
			break;

		case '4':

			logoutUser();
			break;

		case '0':
			cout << "Thank you for using Fitness Gym System.\n";
			break;


		default:
			cout << "Invalid choice. Please try again.\n";

		}
	} while (loginChoice != '0');


}

