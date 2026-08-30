#include<iostream>
#include<iomanip>
#include<vector>
#include<string>
#include<fstream>
#include<sstream>
#include<regex>
#include <cctype>
#include <limits>
#include <cstdlib>
#include"User.hpp"

using namespace std;

int userCount = 0;
string loggedInUser = "";

void clearInputBuffer() {
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void loadUser(Member* members) {
	ifstream file("user.txt");

	if (!file) {
		return;
	}

	userCount = 0;

	string line;
	while (getline(file, line) && userCount < MAX_USERS) {

		if (line.empty()) continue;

		stringstream ss(line);
		Member userInfo;

		string ageStr, genderStr;

		getline(ss, userInfo.loginInfo.usernames, ',');
		getline(ss, userInfo.loginInfo.passwords, ',');
		getline(ss, userInfo.name, ',');
		getline(ss, ageStr, ',');
		getline(ss, genderStr, ',');
		getline(ss, userInfo.phNo, ',');
		getline(ss, userInfo.email, ',');

		if (userInfo.loginInfo.usernames.empty() || ageStr.empty() || genderStr.empty()) {
			continue; // Skip malformed lines
		}

		try {
			userInfo.age = stoi(ageStr);
			userInfo.gender = genderStr[0];
			members[userCount] = userInfo;
			userCount++;
		}
		catch (...) {
			continue;
		}
	}

	file.close();
}

void saveUser(Member* members) {
	ofstream file("user.txt");

	if (!file) {
		cout << "Error: Could not open file." << endl;
		return;
	}

	for (int i = 0; i < userCount; i++) {
		file << members[i].loginInfo.usernames << ","
			<< members[i].loginInfo.passwords << ","
			<< members[i].name << ","
			<< members[i].age << ","
			<< members[i].gender << ","
			<< members[i].phNo << ","
			<< members[i].email << endl;
	}

	file.close();

}

void registerUser(Member* members) {   // user registration 

	Member newMember;

	string username, password;

	cout << "================================\n";
	cout << "            Register            \n";
	cout << "================================\n";

	while (true) {
		cout << "Enter Username (without space): ";
		cin >> username;

		bool usernameExists = false;
		for (int i = 0; i < userCount; i++) {
			if (members[i].loginInfo.usernames == username) {
				cout << "Username already exists. Please try again.\n";
				usernameExists = true;
				break;
			}
		}

		if (!usernameExists) {
			break; // Valid username found
		}
	}

	cout << "Enter Password: ";
	cin >> password;

	newMember.loginInfo.usernames = username;
	newMember.loginInfo.passwords = password;

	cout << "\n--- Personal Information ---\n";
	cin.ignore(); // Clear the input buffer before getline

	while (true) {
		cout << "Enter Full Name: ";
		getline(cin >> ws, newMember.name);
		if (newMember.name.empty()) {
			cout << "Name cannot be empty.\n";
			return;
		}
		if (newMember.name.find_first_of("0123456789") != string::npos) {
			cout << "Invalid input! Name cannot contain numbers.\n";
			return;
		}
		break;
	}

	while (true) {
		cout << "Enter Age : ";
		if (cin >> newMember.age && newMember.age > 0 && newMember.age < 120) {
			break;
		}
		cout << "Invalid age. Please enter a whole number between 1 and 119.\n";
		clearInputBuffer();
	}

	while (true) {
		char g;
		cout << "Enter Gender (M/F): ";
		cin >> g;
		g = toupper(g);
		if (g == 'M' || g == 'F') {
			newMember.gender = g;
			break;
		}
		cout << "Invalid gender. Please enter M or F.\n";
	}

	while (true) {
		cout << "Enter Phone Number (without - ): ";
		cin >> newMember.phNo;

		// Check whether every character is a digit
		if (regex_match(newMember.phNo, regex("^01[0-9]{8,9}$"))) {
			break;
		}

		cout << "Invalid phone number! Must contain 10-11 digits only.\n";
		cout << "Do not use '-' , spaces, or alphabets.\n";
	}

	while (true) {
		cout << "Enter Email Address: ";
		cin >> newMember.email;

		regex emailPattern("[a-z0-9]+@gmail\\.com");

		if (regex_match(newMember.email, emailPattern)) {
			break;
		}

		cout << "Invalid email address!\n";
		cout << "Please enter a valid email such as example@gmail.com\n";
	}

	members[userCount] = newMember;
	userCount++;

	saveUser(members);

	cout << "\nRegistration successful!\n";

}

int loginUser(Member* members, Member& activeMember) {    // user login

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
		if (members[i].loginInfo.usernames == username && members[i].loginInfo.passwords == password) {
			loggedInUser = username;
			activeMember = members[i];
			found = true;
			break;
		}
	}

	if (found) {
		cout << "Login successful. Welcome, " << loggedInUser << "!\n";
		cout << "\nPress enter to continue.\n";
		clearInputBuffer();
		cin.get();
		return 1;
	}
	else {
		cout << "Invalid username or password. Please try again.\n\n";
		pauseScreen();
		return 0;
	}
}



void resetPassword(Member* members) {   // user reset password
	cout << "\n--- Password Reset ---\n";

	string username;
	cout << "Enter your username to reset the password: ";
	cin >> username;

	int userIndex = -1;
	for (int i = 0; i < userCount; ++i) {
		if (members[i].loginInfo.usernames == username) {
			userIndex = i;
			break;
		}
	}

	if (userIndex != -1) {
		string newPassword;
		cout << "Enter your new password: ";
		cin >> newPassword;
		members[userIndex].loginInfo.passwords = newPassword;

		saveUser(members);

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
		cout << "\nStatus: Not Logged In\n";
	}
	else {
		cout << "\nStatus: Logged in as " << loggedInUser << "\n";
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
	Member members[MAX_USERS];
	loadUser(members);


	do {
		clearScreen();
		displaymenu();
		cout << "Enter choice : ";
		cin >> loginChoice;

		Member currentMember;

		switch (loginChoice) {

		case '1':
			registerUser(members);
			pauseScreen();
			break;

		case '2':
			cout << "================================\n";
			cout << "             Login              \n";
			cout << "================================\n";

			if (loginUser(members, currentMember)) {
				userMenu(members);
			}
			break;

		case '3':
			resetPassword(members);
			pauseScreen();
			break;

		case '4':

			logoutUser();
			pauseScreen();
			break;

		case '0':
			cout << "Thank you for using Fitness Gym System.\n";
			exit(0);
			break;


		default:
			cout << "Invalid choice. Please try again.\n";

		}
	} while (loginChoice != '0');


}

