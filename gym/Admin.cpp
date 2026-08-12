#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>

#include "User.hpp"
#include "ScheduleMenu.hpp"

using namespace std;

int getIntegerInput(const string& message, int min, int max) {
	int value;

	while (true) {
		cout << message;

		if (cin >> value && value >= min && value <= max) {
			return value;
		}

		cout << "Invalid input. Please enter a number from "
			<< min << " to " << max << ".\n";

		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
}

double getPositiveDouble(const string& message) {
	double value;

	while (true) {
		cout << message;

		if (cin >> value && value > 0) {
			return value;
		}

		cout << "Invalid amount. Please enter a value greater than 0.\n";

		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
}

string getNonEmptyString(const string& message) {
	string value;

	while (true) {
		cout << message;
		getline(cin >> ws, value);

		if (!value.empty()) {
			return value;
		}

		cout << "Input cannot be empty. Please try again.\n";
	}
}

void pauseScreen() {
	cout << "\nPress ENTER to continue...";
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cin.get();
}

void adminLogin(Member* members, int userCount) {

	string adminUsername, adminPassword;

	cout << "=================================" << endl;
	cout << "           ADMIN LOGIN           " << endl;
	cout << "=================================" << endl;
	cout << "Enter Username: ";
	cin >> adminUsername;
	cout << "Enter Password: ";
	cin >> adminPassword;

	if (adminUsername == "gymAdmin" && adminPassword == "gymAdmin123") {
		cout << "Login successful! Welcome, Admin." << endl;
		
		pauseScreen();

		adminMenu(members, userCount);

	}
	else {
		cout << "Invalid credentials. Access denied." << endl;
	}
}

void displayadminMenu(){

	cout << "==================================================" << endl;
	cout << "                FITNESS ADMIN PORTAL			   " << endl;
	cout << "==================================================" << endl;
	cout << "1. View All Members\n";
	cout << "2. Membership Plan Management\n";
	cout << "3. Class Management\n";
	cout << "4. Trainer Management\n";
	cout << "5. Schedule Management\n";
	cout << "6. View Payment Records\n";
	cout << "7. Attendance Management\n";
	cout << "8. Reports and Analytics\n";
	cout << "0. Logout\n";
	cout << "--------------------------------------------------" << endl;

}

void displayMembershipPlanMenu() {
	cout << "==================================================" << endl;
	cout << "           MEMBERSHIP PLAN MANAGEMENT			   " << endl;
	cout << "==================================================" << endl;
	cout << "1. Add Membership Plan\n";
	cout << "2. View Membership Plans\n";
	cout << "3. Update Membership Plan\n";
	cout << "4. Delete Membership Plan\n";
	cout << "0. Back to Admin Menu\n";
	cout << "--------------------------------------------------" << endl;
}

void displayClassMenu() {
	cout << "==================================================" << endl;
	cout << "               CLASS MANAGEMENT				   " << endl;
	cout << "==================================================" << endl;
	cout << "1. Add Class\n";
	cout << "2. View Classes\n";
	cout << "3. Update Class\n";
	cout << "4. Cancel Class\n";
	cout << "5. Delete Class\n";
	cout << "6. Check Class Capacity\n";
	cout << "0. Back to Admin Menu\n";
	cout << "--------------------------------------------------" << endl;
}

void displayTrainerMenu() {
	cout << "==================================================" << endl;
	cout << "              TRAINER MANAGEMENT				   " << endl;
	cout << "==================================================" << endl;
	cout << "1. Add Trainer\n";
	cout << "2. View Trainers\n";
	cout << "3. Update Trainer\n";
	cout << "4. View Trainer Schedule\n";
	cout << "5. Remove Trainer\n";
	cout << "0. Back to Admin Menu\n";
	cout << "--------------------------------------------------" << endl;
}

void displayScheduleMenu() {
	cout << "==================================================" << endl;
	cout << "              SCHEDULE MANAGEMENT				   " << endl;
	cout << "==================================================" << endl;
	cout << "1. Add Class Schedule\n";
	cout << "2. View All Class Schedules\n";
	cout << "3. Search Schedule by Date\n";
	cout << "4. Update Class Schedule\n";
	cout << "5. Assign Trainer to Schedule\n";
	cout << "6. Cancel Class Schedule\n";
	cout << "0. Back to Admin Menu\n";
	cout << "--------------------------------------------------" << endl;
}

void displayAttendanceMenu() {
	cout << "==================================================" << endl;
	cout << "              ATTENDANCE MANAGEMENT			   " << endl;
	cout << "==================================================" << endl;
	cout << "1. View Today's Attendance Records\n";
	cout << "2. View Weekly Attendance\n";
	cout << "0. Back to Admin Menu\n";
	cout << "--------------------------------------------------" << endl;
}

void displayReportsMenu() {
	cout << "==================================================" << endl;
	cout << "            REPORTS AND ANALYTICS				   " << endl;
	cout << "==================================================" << endl;
	cout << "1. Membership Summary Report\n";
	cout << "2. Class Popularity Report\n";
	cout << "3. Monthly Revenue Report\n";
	cout << "0. Back to Admin Menu\n";
	cout << "--------------------------------------------------" << endl;
}

void viewAllMembers(Member* members, int userCount) {

	loadUser(members);

	cout << "==================================================" << endl;
	cout << "                VIEW ALL MEMBERS				   " << endl;
	cout << "==================================================" << endl;

	if (userCount == 0) {
		cout << "No member records found.\n";
		pauseScreen();
		return;
	}


	cout << "Username" << "\tName" << "\tAge" << "\tGender" << "\tPhone" << "\tEmail" << endl;
	cout << "--------------------------------------------------------------------------\n";

	for (int i = 0; i < userCount; i++) {
		cout << members[i].loginInfo.usernames << "\t" << members[i].name << "\t" << members[i].age << "\t" << members[i].gender << "\t" 
			 << members[i].phNo << "\t" << members[i].email << endl;

	}

	cout << "--------------------------------------------------" << endl;

	cout << "Total Members : " << userCount << endl;

	cout << "\nPress ENTER back to Admin Menu.\n";
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cin.get();
	
}

void addMembershipPlan() {
	
	string planName, planDuration;
	double price;

	cout << "Enter plan name: ";
	cin >> planName;
	cout << "Enter plan duration : ";
	cin >> planDuration;
	cout << "Enter plan price: ";
	cin >> price;

	ofstream membershipFile("membershipPlan.txt");

	if (!membershipFile) {
		cout << "Error: Could not open file." << endl;
		return;
	}

	membershipFile << planName << "," << planDuration << "," << price << endl;
	membershipFile.close();

	cout << "\nPress ENTER back to Membership Plan Menu.\n";
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cin.get();


}

void viewMembershipPlans() {
	// Implementation for viewing membership plans
}

void updateMembershipPlan() {
	// Implementation for updating a membership plan
}

void deleteMembershipPlan() {
	// Implementation for deleting a membership plan
}


void adminMenu(Member* members, int userCount) {

	char choice, choose;

	do {
		displayadminMenu();
		cout << "Enter your choice: ";
		cin >> choice;

		vector<Schedule> schedules;

		switch (choice) {
		case '1': // View All Members
			viewAllMembers(members, userCount);
			break;

		case '2': // Membership Plan Management
			do {
				displayMembershipPlanMenu();
				cout << "Enter your choice: ";
				cin >> choose;

				switch (choose){
					case '1': // Call function to add membership plan

						break;

					case '2': // Call function to view membership plans

						break;

					case '3': // Call function to update membership plan

						break;

					case '4': // Call function to delete membership plan
						
						break;

					case '0':
						cout << "Returning to Admin Menu...\n";
						break;
				default:
					cout << "Invalid choice. Please try again." << endl;
					break;
				}

			} while (choose != '0');

			break;

		case '3': // Class Management
			do {
				displayClassMenu();
				cout << "Enter your choice: ";
				cin >> choose;

			} while (choose != '0');
			break;

		case '4': // Trainer Management
			do {
				displayTrainerMenu();
				cout << "Enter your choice: ";
				cin >> choose;

			} while (choose != '0');
			break;

		case '5': // Schedule Management

			loadSchedulesFromFile(schedules);

			do {
				displayScheduleMenu();
				cout << "Enter your choice: ";
				cin >> choose;
				cin.ignore(numeric_limits<streamsize>::max(), '\n');

				switch (choose) {
				case '1':
					// Call function to add class schedule
					addschedule(schedules);
					pauseScreen();
					break;

				case '2':
					// Call function to view class schedule
					displayschedule(schedules);
					pauseScreen();
					break;

				case '3':
					// Call function to search class schedule
					searchschedule(schedules);
					pauseScreen();
					break;

				case '4':
					// Call function to update class schedule
					updateschedule(schedules);
					pauseScreen();
					break;

				case '5':
					// Call function to assign trainer
					assigntrainer(schedules);
					pauseScreen();
					break;
				case '6':
					cancelschedule(schedules);
					pauseScreen();
					break;

				case '0':
					cout << "Returning to Admin Menu...\n";
					break;
				default:
					cout << "Invalid choice. Please try again." << endl;
				}
			} while (choose != '0');
			break;

		case '6': // View Payment Records
			
			break;

		case '7': // Attendance Management
			
			do {
				displayAttendanceMenu();
				cout << "Enter your choice: ";
				cin >> choose;


			} while (choose != '0');
			break;

		case '8': // Reports and Analytics

			do {
				displayReportsMenu();
				cout << "Enter your choice: ";
				cin >> choose;

			} while (choose != '0');
			
			break;
		case '0':
			cout << "Logging out..." << endl;
			break;
		default:
			cout << "Invalid choice. Please try again." << endl;
		}

	} while (choice != '0');
}