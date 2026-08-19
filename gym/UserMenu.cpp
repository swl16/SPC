#include<iostream>
#include<iomanip>
#include<vector>
#include<string>
#include<fstream>
#include<sstream>
#include <regex>

#include "User.hpp"
#include "FitnessTracking.hpp"
#include "AttendanceTracker.hpp"

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
	cout << "8. Fitness Tracker\n";
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

	string planName = "-", startDate = "-", endDate = "-", status = "-";

	ifstream memFile("UserMembership.txt");

	string line;

	if (memFile.is_open()) {
		while (getline(memFile, line)) {

			if (line.empty()) continue;

			stringstream ss(line);
			string uName, pName, sDate, eDate, stat;

			getline(ss, uName, ',');
			getline(ss, pName, ',');
			getline(ss, sDate, ',');
			getline(ss, eDate, ',');
			getline(ss, stat, ',');

			// If it matches the current user, update the details
			// (It will keep reading to the end, ensuring it grabs the latest record)
			if (uName == members[i].loginInfo.usernames) {
				planName = pName;
				startDate = sDate;
				endDate = eDate;
				status = stat;
			}
		}
		memFile.close();
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

	cout << "--------------------------------\n";
	cout << "       MEMBERSHIP DETAILS       \n";
	cout << "--------------------------------\n";
	cout << "Plan Name   : " << planName << endl;
	cout << "Start Date  : " << startDate << endl;
	cout << "End Date    : " << endDate << endl;
	cout << "Status      : " << status << endl;
	cout << "================================\n";

	cout << "\nPress Enter to return to the User Menu...";
	cin.get();
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

void viewPaymentHistory(Member* members) {

	int i = loggedInMember(members);

	if (i == -1) {
		cout << "Error: No user is currently logged in.\n";
		return;
	}


	ifstream file("UserPayment.txt");
	if (!file.is_open()) {
		cout << "No payment history found.\n";
		cout << "\nPress Enter to return to the User Menu...";
		cin.get();
		return;
	}

	cout << "\n===============================================================\n";
	cout << "                      PAYMENT HISTORY                           \n";
	cout << "================================================================\n";

	string line;
	bool hasHistory = false;

	while (getline(file, line)) {

		if (line.empty()) continue;

		stringstream ss(line);

		string pID, uName, pName, amt, pDate, pMethod;

		getline(ss, pID, ',');
		getline(ss, uName, ',');
		getline(ss, pName, ',');
		getline(ss, amt, ',');
		getline(ss, pDate, ',');
		getline(ss, pMethod, ',');

		if (uName == members[i].loginInfo.usernames) {
			hasHistory = true;

			cout << "Payment ID     : " << pID << endl;
			cout << "Description    : " << pName << endl;
			cout << "Amount         : RM " << amt << endl;
			cout << "Date and Time  : " << pDate << endl;
			cout << "Payment Method : " << pMethod << endl;
			cout << "-----------------------------------------\n";
		}
	}

	file.close();

	if (!hasHistory) {
		cout << "No transaction records for this account.\n";
	}

	cout << "================================================================\n";

	// Pause before returning to the menu
	cout << "\nPress Enter to return to the User Menu...";
	cin.get();

}

bool deleteAccount(Member* members) {
	int i = loggedInMember(members);

	if (i == -1) {
		cout << "Error: No user is currently logged in.\n";
		return false;
	}

	char confirm;

	cout << "\n==================================================\n";
	cout << "               DELETE ACCOUNT WARNING             \n";
	cout << "==================================================\n";
	cout << "Are you sure you want to permanently delete your account?\n";
	cout << "This action CANNOT be undone.\n";
	cout << "Enter 'Y' to confirm, 'N' to cancel: ";
	cin >> confirm;

	if (confirm == 'Y' || confirm == 'y') {
		string deletedUsername = members[i].loginInfo.usernames;

		for (int j = i; j < userCount; j++) {
			members[j] = members[j + 1];
		}
		userCount--;
		saveUser(members);

		string line;

		vector<string> memLines;
		ifstream memFileIn("UserMembership.txt");
		string line;
		if (memFileIn.is_open()) {
			while (getline(memFileIn, line)) {
				if (line.empty()) continue;
				stringstream ss(line);
				string uName;
				getline(ss, uName, ','); // Read username up to the comma

				if (uName != deletedUsername) {
					memLines.push_back(line);
				}
			}
			memFileIn.close();

			ofstream memFileOut("UserMembership.txt");
			for (const auto& l : memLines) memFileOut << l << "\n";
			memFileOut.close();
		}

		vector<string> bookLines;
		ifstream bookFileIn("classBookings.txt");
		if (bookFileIn.is_open()) {
			while (getline(bookFileIn, line)) {
				if (line.empty()) continue;
				stringstream ss(line);
				string bID, uName;
				getline(ss, bID, ',');
				getline(ss, uName, ',');

				if (uName != deletedUsername) {
					bookLines.push_back(line);
				}
			}
			bookFileIn.close();

			ofstream bookFileOut("classBookings.txt");
			for (const auto& l : bookLines) bookFileOut << l << "\n";
			bookFileOut.close();
		}

		vector<string> loginLines;
		ifstream loginFileIn("user.txt");

		if (loginFileIn.is_open()) {
			while (getline(loginFileIn, line)) {
				if (line.empty()) continue;
				stringstream ss(line);
				string uName;
				getline(ss, uName, ',');

				// Keep everyone EXCEPT the deleted user
				if (uName != deletedUsername) {
					loginLines.push_back(line);
				}
			}

			loginFileIn.close();

			ofstream loginFileOut("user.txt");
			for (const auto& l : loginLines) loginFileOut << l << "\n";
			loginFileOut.close();
		}

		cout << "\nAccount successfully deleted. We are sad to see you go!\n";
		cout << "Press Enter to return to the Main Menu...";
		cin.ignore(1000, '\n');
		cin.get();

		logoutUser(); // Logs the user out securely
		return true;

	}
	else {
		cout << "\nAccount deletion cancelled.\n";
		cout << "Press Enter to return to the User Menu...";
		cin.ignore(1000, '\n');
		cin.get();
		return false;
	}

}


void userMenu(Member* members) {

	char choice;

	do{

		displayUserMenu();
		cout << "Enter your choice : ";
		cin >> choice;

		int memberIndex = loggedInMember(members);
		Member currentMember;
		if (memberIndex != -1) {
			currentMember = members[memberIndex];
		}

		switch (choice) {
		case '1':
			viewProfile(members);
			break;

		case '2':
			editProfile(members);
			break;

		case '3':
			membershipPlan(currentMember);
			break;

		case '4':	//book/view class
			bookingClass(currentMember);
			break;

		case '5':	//view/cancel bookings
			viewingBooking(currentMember);
			break;

		case '6':
			viewPaymentHistory(members);
			break;

		case '7':
			attendanceMenu(currentMember);
			break;

		case '8':
			fitnessMenu(currentMember);
			break;

		case '9':
			deleteAccount(members);
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