#include<iostream>
#include<iomanip>
#include<vector>
#include<string>
#include<fstream>
#include<sstream>
#include<regex>

#include"User.hpp"

using namespace std;

int userCount = 0;
string loggedInUser = "";

void registerUser(Member* members) {   // user registration 

	Member newMember;

	string username, password;

	cout << "================================\n";
	cout << "            Register            \n";
	cout << "================================\n";

	while (true) {
		cout << "Enter a new username (without space): ";
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
		cout << "\nPress ENTER to continue.";
		cin.ignore(numeric_limits<streamsize>::max(), '\n');    // Proper buffer clearing
		cin.get();
	}
	cout << "Enter a new password: ";
	cin >> password;

	newMember.loginInfo.usernames = username;
	newMember.loginInfo.passwords = password;

	cout << "\n--- Personal Information ---\n";
	cin.ignore(); // Clear the input buffer before getline

	cout << "Enter Full Name: ";
	getline(cin, newMember.name);
	if (!newMember.name.empty()) {
		cout << "Name cannot be empty.\n";
		return;
	}

	cout << "Enter Age: ";
	cin >> newMember.age;
	if (cin >> newMember.age && newMember.age > 0 && newMember.age < 120) {
		cout << "Invalid age. Please enter a whole number between 1 and 119.\n";
		return;
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
		cout << "Enter Phone Number (without -): ";
		cin >> newMember.phNo;
		bool allDigits = !newMember.phNo.empty();
		for (char c : newMember.phNo) {
			if (!isdigit(static_cast<unsigned char>(c))) { allDigits = false; break; }
		}
		if (allDigits && newMember.phNo.length() >= 12) break;
		cout << "Invalid phone number. Digits only, at least 7 characters.\n";
	}

	while (true) {
		cout << "Enter Email Address: ";
		cin >> newMember.email;
		// Minimal sanity check: must contain '@' and a '.' after it
		size_t at = newMember.email.find('@');
		size_t dot = newMember.email.find('.', at == string::npos ? 0 : at);
		if (at != string::npos && at > 0 && dot != string::npos && dot < newMember.email.length() - 1) {
			break;
		}
		cout << "Invalid email address. Please try again.\n";
	}

	members[userCount] = newMember;
	userCount++;

	cout << "Registration successful!\n";

}

int loginUser(Member* members) {    // user login

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

void loadUser(Member * members) {
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

		userInfo.age = stoi(ageStr);
		userInfo.gender = genderStr[0];

		members[userCount] = userInfo;
		userCount++;
	}

	file.close();
}

void saveUser(Member*members) {
	ofstream file("user.txt");

	if (!file) {
		cout << "Error: Could not open file." << endl;
		return;
	}

	for (int i = 0; i < userCount; i++) {
		file << members[i].loginInfo.usernames << "," << members[i].loginInfo.passwords
			<< "," << members[i].name << "," << members[i].age << "," << members[i].gender
			<< "," << members[i].phNo << "," << members[i].email << endl;
	}

	file.close();

}

void resetPassword(Member*members) {   // user reset password
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
	Member members[MAX_USERS];
	string enteredUsername, enteredPassword;
	loadUser(members);


	do {
		displaymenu();
		cout << "Enter choice : ";
		cin >> loginChoice;

		switch (loginChoice) {

		case '1':
			registerUser(members);
			saveUser(members);
			break;

		case '2':
			cout << "================================\n";
			cout << "             Login              \n";
			cout << "================================\n";

			if (loginUser(members)) {
				userMenu(members);
			}
			break;

		case '3':
			resetPassword(members);
			saveUser(members);
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

