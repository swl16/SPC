#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
#include <cstdio>
#include <algorithm>

#include "User.hpp"
#include "ScheduleMenu.hpp"

using namespace std;

extern int userCount;

int generatePlanID(const string& filename) {
	ifstream file(filename);
	if (!file.is_open()) return 0001; // Default start ID

	int lastID = 0000;
	string line;
	while (getline(file, line)) {
		if (line.empty()) continue;
		stringstream ss(line);
		string strID;
		getline(ss, strID, ',');
		try {
			int currentID = stoi(strID);
			if (currentID > lastID) lastID = currentID;
		}
		catch (...) {
			//skip malformed line
		}
	}
	file.close();
	return lastID + 1;
}

void saveMembershipPlans(const string& filename, const vector<MembershipPlanRecord>& plans) {
	ofstream file(filename);
	if (!file) {
		cout << "Error: Could not open file." << endl;
		return;
	}
	for (const auto& p : plans) {
		file << p.id << "," << p.planName << "," << p.duration << ","
			<< fixed << setprecision(2) << p.price << "," << p.benefits << "\n";
	}
	file.close();
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


void adminLogin() {

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

		Member* members = new Member[MAX_USERS];
		loadUser(members);

		adminMenu(members, userCount);

	}
	else {
		cout << "Invalid credentials. Access denied." << endl;
		return;
	}
}

void displayadminMenu(){

	cout << "==================================================" << endl;
	cout << "                FITNESS ADMIN PORTAL			   " << endl;
	cout << "==================================================" << endl;
	cout << "1. View All Members\n";
	cout << "2. Membership Plans\n";
	cout << "3. Classes & Schedule\n";
	cout << "4. View Payment Records\n";
	cout << "5. Attendance";
	cout << "6. Reports and Analytics\n";
	cout << "0. Logout\n";
	cout << "--------------------------------------------------" << endl;

}

void displayMembershipPlanMenu() {
	cout << "==================================================" << endl;
	cout << "                 MEMBERSHIP PLANS			       " << endl;
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
	cout << "               CLASSES SCHEDULE				   " << endl;
	cout << "==================================================" << endl;
	cout << "1. Add Class Schedule\n";
	cout << "2. View Class Schedule\n";
	cout << "3. Update Class Schedule\n";
	cout << "4. Search Class Schedule by Date\n";
	cout << "5. Cancel Class Schedule\n";
	cout << "6. Delete Class Schedule\n";
	cout << "7. Check Class Capacity\n";
	cout << "0. Back to Admin Menu\n";
	cout << "--------------------------------------------------" << endl;
}



void displayAttendanceMenu() {
	cout << "======================================" << endl;
	cout << "              ATTENDANCE			   " << endl;
	cout << "======================================" << endl;
	cout << "1. View Today's Attendance Records\n";
	cout << "2. View Weekly Attendance\n";
	cout << "0. Back to Admin Menu\n";
	cout << "--------------------------------------" << endl;
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


void viewAllMembers() {

	Member members[MAX_USERS];

	loadUser(members);

	cout << "==================================================" << endl;
	cout << "                VIEW ALL MEMBERS				   " << endl;
	cout << "==================================================" << endl;

	if (userCount == 0) {
		cout << "No member records found.\n";
		pauseScreen();
		return;
	}


	cout << left << setw(15) << "Username" << setw(20) << "Name" << setw(6) << "Age"
		<< setw(8) << "Gender" << setw(15) << "Phone" << "Email" << endl;
	cout << "--------------------------------------------------------------------------\n";

	for (int i = 0; i < userCount; i++) {
		cout << left << setw(15) << members[i].loginInfo.usernames
			<< setw(20) << members[i].name
			<< setw(6) << members[i].age
			<< setw(8) << members[i].gender
			<< setw(15) << members[i].phNo
			<< members[i].email << endl;

	}

	cout << "--------------------------------------------------" << endl;

	cout << "Total Members : " << userCount << endl;

	cout << "\nPress ENTER back to Admin Menu.\n";
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cin.get();
	
}

void addMembershipPlan() {
	const string filename = "membershipPlan.txt";
	vector<MembershipPlanRecord> plans = loadMembershipPlans(filename);

	MembershipPlanRecord p;
	p.id = generatePlanID(filename);

	cout << "\n================================================" << endl;
	cout << "            ADD NEW MEMBERSHIP PLAN             " << endl;
	cout << "================================================" << endl << endl;

	cout << "Generated Plan ID: " << p.id << endl;
	cout << "Enter plan name (e.g. Quarterly / Premium  Pass): ";
	getline(cin >> ws, p.planName);

	do {
		cout << "Enter Duration (in months, e.g., 1, 3, 6, 12): ";
		if (cin >> p.duration && p.duration > 0) break;

		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Invalid duration. Please enter a positive number." << endl;
	} while (true);

	do {
		cout << "Enter Price (RM, e.g., 150.00): ";
		if (cin >> p.price && p.price >= 0) break;

		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Invalid price. Please enter a valid amount." << endl;
	} while (true);
	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	cout << "\nEnter Plan Benefits separated by semicolons ';'" << endl;
	cout << "Example: Free locker access; Unlimited gym entrance" << endl;
	cout << "Benefits: ";

	getline(cin, p.benefits);

	if (p.benefits.empty()) {
		p.benefits = "Standard gym access";
	}

	cout << "\n------------------------------------------------" << endl;
	cout << "           CONFIRM NEW PLAN DETAILS             " << endl;
	cout << "------------------------------------------------" << endl;
	cout << "Plan ID   : " << p.id << endl;
	cout << "Plan Name : " << p.planName << endl;
	cout << "Duration  : " << p.duration << (p.duration == 1 ? " Month" : " Months") << endl;
	cout << "Price     : RM " << fixed << setprecision(2) << p.price << endl;
	cout << "Benefits  : " << p.benefits << endl;
	cout << "------------------------------------------------" << endl;

	char confirm;
	cout << "Save this membership plan to database? (Y/N): ";
	cin >> confirm;

	if (confirm == 'Y' || confirm == 'y') {
		plans.push_back(p);
		saveMembershipPlans(filename, plans);

		cout << "The new membership is successfully added." << endl;
	} else {
		cout << "\nOperation cancelled. Plan was not saved." << endl;
	}

	pauseScreen();
}

void viewMembershipPlans() {
	vector<MembershipPlanRecord> plans = loadMembershipPlans("membershipPlan.txt");

	cout << "================================================" << endl;
	cout << "                MEMBERSHIP PLANS                " << endl;
	cout << "================================================" << endl << endl;

	if (plans.empty()) {
		cout << "No membership plans found.\n";
		pauseScreen();
		return;
	}

	cout << left << setw(5) << "No."
		<< left << setw(15) << "Plan Name"
		<< left << setw(15) << "Duration"
		<< left << setw(15) << "Price" << endl;
	cout << "------------------------------------------------" << endl;

	cout << fixed << setprecision(2);

	for (const auto& p : plans) {
		string durationStr = to_string(p.duration) + (p.duration == 1 ? " Month" : " Months");
		cout << left << setw(5) << p.id
			<< left << setw(15) << p.planName
			<< left << setw(15) << durationStr
			<< "RM " << p.price << endl;
	}
	cout << "------------------------------------------------" << endl;

	for (const auto& p : plans) {
		cout << p.planName << " Plan Benefits:" << endl;
		stringstream ss(p.benefits);
		string item;
		while (getline(ss, item, ';')) {
			cout << "- " << item << endl;
		}

		cout << "------------------------------------------------" << endl;
	}
	pauseScreen();
}

void updateMembershipPlan() {
	const string filename = "membershipPlan.txt";
	vector<MembershipPlanRecord> plans = loadMembershipPlans(filename);


	cout << "\n================================================" << endl;
	cout << "              UPDATE MEMBERSHIP PLAN            " << endl;
	cout << "================================================" << endl << endl;

	if (plans.empty()) {
		cout << "No membership plans found in database." << endl;
		return;
	}

	cout << left << setw(8) << "ID"
		<< left << setw(18) << "Plan Name"
		<< left << setw(12) << "Duration"
		<< right << setw(10) << "Price (RM)" << endl;
	cout << "------------------------------------------------" << endl;

	cout << fixed << setprecision(2);

	for (const auto& p : plans) {
		string durationStr = to_string(p.duration) + (p.duration == 1 ? " Mo" : " Mos");
		cout << left << setw(8) << p.id
			<< left << setw(18) << p.planName
			<< left << setw(12) << durationStr
			<< right << setw(10) << p.price << endl;
	}


	int targetID = getIntegerInput("\nEnter Plan ID to update (or 0 to cancel): ", 0, 999999);
	if (targetID == 0) {
		cout << "Update cancelled." << endl;
		return;
	}

	bool updated = false;

	for (auto& p : plans) {
		if (p.id == targetID) {
			cout << "\n--- ENTER NEW PLAN DETAILS ---" << endl;
			cout << "Enter New Plan Name (e.g. Gold Pass): ";
			getline(cin >> ws, p.planName);

			do {
				cout << "Enter New Duration (in months): ";
				if (cin >> p.duration && p.duration > 0) break;

				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Invalid input. Please enter a positive number." << endl;
			} while (true);

			do {
				cout << "Enter New Price (RM): ";
				if (cin >> p.price && p.price >= 0) break;

				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Invalid input. Please enter a valid price." << endl;
			} while (true);

			cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear buffer before getline
			cout << "Enter New Benefits (separated by ';'): ";
			getline(cin, p.benefits);

			if (p.benefits.empty()) {
				p.benefits = "Standard gym access";
			}
			updated = true;
			break;
		}
	}

	if (updated) {
		saveMembershipPlans(filename, plans);
		cout << "\n==============================================" << endl;
		cout << "     MEMBERSHIP PLAN UPDATED SUCCESSFULLY!      " << endl;
		cout << "================================================" << endl;
	}
	else {
		cout << "\n[ERROR] Plan ID " << targetID << " not found!" << endl;
	}

	pauseScreen();
}

void deleteMembershipPlan() {
	const string filename = "membershipPlan.txt";
	vector<MembershipPlanRecord> plans = loadMembershipPlans(filename);

	cout << "\n================================================" << endl;
	cout << "             DELETE MEMBERSHIP PLAN             " << endl;
	cout << "================================================" << endl << endl;

	if (plans.empty()) {
		cout << "No membership plans found in database." << endl;
		return;
	}

	/*int planID, duration;
	string planName, benefits;
	double price;*/

	cout << left << setw(8) << "ID"
		<< left << setw(18) << "Plan Name"
		<< left << setw(12) << "Duration"
		<< right << setw(10) << "Price (RM)" << endl;
	cout << "------------------------------------------------" << endl;

	cout << fixed << setprecision(2);
	for (const auto& p : plans) {
		string durationStr = to_string(p.duration) + (p.duration == 1 ? " Mo" : " Mos");
		cout << left << setw(8) << p.id
			<< left << setw(18) << p.planName
			<< left << setw(12) << durationStr
			<< right << setw(10) << p.price << endl;
	}

	int targetID = getIntegerInput("\nEnter Plan ID to DELETE (or 0 to cancel): ", 0, 999999);
	if (targetID == 0) {
		cout << "Deletion cancelled." << endl;
		return;
	}

	char confirm;
	cout << "Are you sure you want to delete Plan ID " << targetID << "? (Y/N): ";
	cin >> confirm;

	if (confirm != 'Y' && confirm != 'y') {
		cout << "Deletion cancelled." << endl;
		return;
	}

	// [auto] automatically deduces the data type of a variable at compile time based on its initialization value
	auto it = remove_if(plans.begin(), plans.end(), [&](const MembershipPlanRecord& p) {
		return p.id == targetID;
		});


	if (it != plans.end()) {
		plans.erase(it, plans.end()); // erase it from the vector
		saveMembershipPlans(filename, plans); //Save the updated vector to file

		cout << "\n================================================" << endl;
		cout << "     MEMBERSHIP PLAN DELETED SUCCESSFULLY!      " << endl;
		cout << "================================================" << endl;
	}
	else {
		cout << "\n[ERROR] Plan ID " << targetID << " not found!" << endl;
	}

	pauseScreen();
}



void checkClassCapacity(){

}

void viewTodayAttendance(){}
void viewWeeklyAttandance(){}

void membershipReport() {}
void classPopularReport() {}
void monthlyReport() {}


void adminMenu(Member* members, int userCount) {

	char choice, choose;
	vector<Schedule> schedules;
	loadSchedulesFromFile(schedules);

	do {
		clearScreen();
		displayadminMenu();
		cout << "Enter your choice: ";
		cin >> choice;

		if (cin.fail()) {
			if (cin.eof()) {
				cout << "\nInput closed. Exiting admin menu." << endl;
				break;
			}
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Invalid input. Please try again." << endl;
			continue;
		}


		switch (choice) {
		case '1': // View All Members
			viewAllMembers();
			break;

		case '2': // Membership Plan Management
			do {
				displayMembershipPlanMenu();
				cout << "Enter your choice: ";
				cin >> choose;

				switch (choose){
				case '1': // Call function to add membership plan
					addMembershipPlan();
					break;

				case '2': // Call function to view membership plans
					viewMembershipPlans();
					break;

				case '3': // Call function to update membership plan
					updateMembershipPlan();
					break;

				case '4': // Call function to delete membership plan
					deleteMembershipPlan();
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

				switch (choose) {
				case '1': // add class
					break;
				case '2':   //view class schedule
					break;
				case '3': //update class
					break;
				default:
					
				}

			} while (choose != '0');
			break;

		case '4':
		
			break;

		case '5':
			
			break;

		case '6': // Attendance Management

			break;

		case '0':
			cout << "Logging out..." << endl;
			break;
		default:
			cout << "Invalid choice. Please try again." << endl;
		}

	} while (choice != '0');
}