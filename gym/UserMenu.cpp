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

	clearScreen();
	cout << "            -------           --------            \n";
	cout << "            |     |---------- |      |            \n";
	cout << "            |                        |            \n";
	cout << "            |     |---------- |      |            \n";
	cout << "            -------           --------            \n";

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
	cout << "0. Return to Login Page\n";
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

	string planName = "-", startDate = "-", endDate = "-", status = "Inactive";
	int targetPlanId = -1;

	ifstream memFile("UserMembership.txt");

	string line;

	if (memFile.is_open()) {
		while (getline(memFile, line)) {

			if (line.empty()) continue;

			stringstream ss(line);
			string uName, pID, sDate, eDate;

			getline(ss, uName, ',');
			getline(ss, pID, ',');
			getline(ss, sDate, ',');
			getline(ss, eDate, ',');

			// If it matches the current user, update the details
			// (It will keep reading to the end, ensuring it grabs the latest record)
			if (uName == members[i].loginInfo.usernames && !pID.empty()) {
				try {
					targetPlanId = stoi(pID);
					startDate = sDate;
					endDate = eDate;
				}
				catch (...) {
					continue;
				}
			}
		}
		memFile.close();
	}

	if (targetPlanId != -1) {
		vector<MembershipPlanRecord> plans = loadMembershipPlans("membershipPlan.txt");
		for (const auto& p : plans) {
			if (p.id == targetPlanId) {
				planName = p.planName;
				break;
			}
		}

		tm expTm = { 0 };
		int year, month, day;
		char sep1, sep2;
		stringstream dStream(endDate);

		if (dStream >> year >> sep1 >> month >> sep2 >> day) {
			expTm.tm_year = year - 1900;
			expTm.tm_mon = month - 1;
			expTm.tm_mday = day;
			expTm.tm_isdst = -1;

			time_t expireTime = mktime(&expTm);
			time_t now = time(nullptr);
			double secondsLeft = difftime(expireTime, now);
			int daysLeft = static_cast<int>(secondsLeft / 86400);

			if (daysLeft >= 0) {
				status = "Active (" + to_string(daysLeft) + " days remaining)";
			}
			else {
				status = "Expired (" + to_string(abs(daysLeft)) + " days ago)";
			}
		}
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

		string ageInput;
		string genderInput;

		switch (choice){
		case '1':

			while (true) {
				cout << "Enter Full Name: ";
				getline(cin, members[i].name);
				if (members[i].name.empty()) {
					cout << "Name cannot be empty.\n";
					continue;
				}
				if (any_of(members[i].name.begin(), members[i].name.end(), ::isdigit)) {
					cout << "Invalid input! Name cannot contain numbers. Please try again.\n";
					continue; // Use continue to let them retry
				}

				saveUser(members); // Save the updated profile to the file
				cout << "Name updated successfully.\n";
				break;
			}
			break;

		case'2':
			while (true) {
				cout << "Enter new age : ";
				cin >> ageInput;

				// Check if the input consists purely of numeric digits (no decimals or text)
				if (all_of(ageInput.begin(), ageInput.end(), ::isdigit)) {
					try {
						members[i].age = stoi(ageInput);

						if (members[i].age > 0 && members[i].age < 120) {

							saveUser(members); // Save the updated profile to the file
							cout << "Age updated successfully.\n";
							break; // Valid age found, break the loop
						}
					}
					catch (...) {
						// Silently catch if they enter a massive number that crashes stoi
					}
				}

				cout << "Invalid age. Please enter a whole number between 1 and 119.\n";
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
			}
			break;

		case '3':

			while (true) {
				cout << "Enter new gender (M/F): ";
				cin >> genderInput;

				// Ensure the user typed exactly ONE character (e.g. "M", not "Male")
				if (genderInput.length() == 1) {
					char g = toupper(genderInput[0]);

					if (g == 'M' || g == 'F') {
						members[i].gender = g;

						saveUser(members); // Save the updated profile to the file
						cout << "Gender updated successfully.\n";
						break; // Valid input, break the loop
					}
				}

				cout << "Invalid gender. Please enter strictly M or F.\n";
			}

			break;

		case '4':
			while (true) {
				cout << "Enter New Phone Number (without - ): ";
				cin >> members[i].phNo;

				// Check whether every character is a digit
				if (regex_match(members[i].phNo, regex("^01[0-9]{8,9}$"))) {
					break;
				}

				cout << "Invalid phone number!\n";
				cout << "Phone number must contain 10-11 digits only.\n";
				cout << "Do not use '-' , spaces, or alphabets.\n";
			}

			saveUser(members); // Save the updated profile to the file
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
			saveUser(members); // Save the updated profile to the file
			cout << "Email updated successfully.\n";
			break;

		case '0':
			cout << "Returning Main Menu ....";
			break;

		default:
			cout << "Invalid choice. Please try again.\n";
			break;
		}

	}while(choice != '0');

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
	cin.ignore(1000, '\n');
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
			clearScreen();
			viewProfile(members);
			pauseScreen();
			break;

		case '2':
			clearScreen();
			editProfile(members);
			pauseScreen();
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
			clearScreen();
			viewPaymentHistory(members);
			break;

		case '7':
			clearScreen();
			attendanceMenu(currentMember);
			break;

		case '8':
			clearScreen();
			fitnessMenu(currentMember);
			break;

		case '9':
			clearScreen();
			deleteAccount(members);
			userLogin();
			break;

		case '0':
			userLogin();
			break;

		default:
			cout << "Invalid choice. Please try again.\n";
			break;
		}

	} while (choice != '0');

}