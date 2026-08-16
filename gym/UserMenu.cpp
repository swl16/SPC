#include<iostream>
#include<iomanip>
#include<vector>
#include<string>
#include<fstream>
#include<sstream>
#include <regex>

#include "User.hpp"
#include "FitnessTracking.hpp"

using namespace std;

extern string loggedInUser;
extern int userCount;

void displayUserMenu() {
	cout << "==================================================" << endl;
	cout << "                FITNESS MEMBER PORTAL			   " << endl;
	cout << "==================================================" << endl;
	cout << "1. View My Profile\n";
	cout << "2. Edit My Profile\n";
	cout << "3. Buy / Renew Membership Plans\n";
	cout << "4. Book / View Class\n";
	cout << "5. View / Cancel Bookings\n";
	cout << "6. Payment History\n";
	cout << "7. Gym Attendance\n";
	cout << "8. Fitness Progress\n";
	cout << "9. Delete Account\n";
	cout << "0. Logout\n";
	cout << "--------------------------------------------------" << endl;
}

int loggedInMember(Member members[]) {

	for (int i = 0; i < userCount; i++) {
		if(members[i].loginInfo.usernames == loggedInUser) {
			return i; // Return the index of the logged-in member
		}
	}
	return -1; // Return -1 if no logged-in member is found
}

void viewProfile(Member* members) {

	int i = loggedInMember(members);

	if (i == -1) {
		cout << "Error: No user is currently logged in.\n";
		return;
	}
	cout << "================================\n";
	cout << "         USER PROFILE           \n";
	cout << "================================\n";
	cout << "Username    : " << members[i].loginInfo.usernames << endl;
	cout << "Name        : " << members[i].name << endl;
	cout << "Age         : " << members[i].age << endl;
	cout << "Gender      : " << members[i].gender << endl;
	cout << "Phone Number: " << members[i].phNo << endl;
	cout << "Email       : " << members[i].email << endl;
}

void editProfile(Member* members) {

	int i = loggedInMember(members);

	if(i == -1) {
		cout << "Error: No user is currently logged in.\n";
		return;
	}
	char choice;

	do {

		cout << "================================\n";
		cout << "         EDIT PROFILE           \n";
		cout << "================================\n";
		cout << "1. Edit Name\n";
		cout << "2. Edit Age\n";
		cout << "3. Edit Gender\n";
		cout << "4. Edit Phone Number\n";
		cout << "5. Edit Email\n";
		cout << "0. Back to Main Menu\n";
		cout << "--------------------------------\n";
		cout << "Enter your choice: ";
		cin >> choice;

		cin.ignore(); // Clear the input buffer

		switch (choice){
		case '1':

			cout << "Enter Full Name: ";
			getline(cin, members[i].name);
			if (members[i].name.empty()) {
				cout << "Name cannot be empty.\n";
				return;
			}
			cout << "Name updated successfully.\n";
			break;

		case'2':
			cout << "Enter new age: ";
			cin >> members[i].age;
			if (!(cin >> members[i].age && members[i].age > 0 && members[i].age < 120)) {
				cout << "Invalid age. Please enter a whole number between 1 and 119.\n";
				return;
			}

			cout << "Age updated successfully.\n";
			break;

		case '3':

			while (true) {
				char g;
				cout << "Enter new gender (M/F): ";
				cin >> g;
				g = toupper(g);
				if (g == 'M' || g == 'F') {
					members[i].gender = g;
					break;
				}
				cout << "Invalid gender. Please enter M or F.\n";
			}

			cout << "Gender updated successfully.\n";
			break;

		case '4':
			while (true) {
				cout << "Enter New Phone Number (without - ): ";
				cin >> members[i].phNo;

				// Check whether every character is a digit
				if (regex_match(members[i].phNo, regex("[0-9]{10,11}"))) {
					break;
				}

				cout << "Invalid phone number!\n";
				cout << "Phone number must contain 10-11 digits only.\n";
				cout << "Do not use '-' , spaces, or alphabets.\n";
			}

			cout << "Phone number updated successfully.\n";
			break;

		case '5':

			while (true) {
				cout << "Enter Email Address: ";
				cin >> members[i].email;

				regex emailPattern("[a-z0-9]+@gmail\\.com");

				if (regex_match(members[i].email, emailPattern)) {
					break;
				}

				cout << "Invalid email address!\n";
				cout << "Please enter a valid email such as example@gmail.com\n";
			}

			cout << "Email updated successfully.\n";
			break;

		default:
			cout << "Invalid choice. Please try again.\n";
			break;
		}

	}while(choice != '0');

	saveUser(members); // Save the updated profile to the file
	cout << "Profile updated & saved successfully.\n";
}


void userMenu(Member* members) {

	char choice;

	do{

		displayUserMenu();
		cout << "Enter your choice : ";
		cin >> choice;

		switch (choice) {
		case '1':
			viewProfile(members);
			break;

		case '2':
			editProfile(members);
			break;

		case '3':
			membershipPlan(members[loggedInMember(members)]);
			break;

		case '4':
			break;

		case '5':
			break;

		case '6':
			break;

		case '7':
			break;

		case '8':
			break;

		case '9':
			break;

		case '0':
			logoutUser();
			break;

		default:
			cout << "Invalid choice. Please try again.\n";
			break;
		}

	} while (choice != '0');

}