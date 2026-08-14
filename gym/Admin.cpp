#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
#include <cstdio>

#include "User.hpp"
#include "ScheduleMenu.hpp"

using namespace std;

extern int userCount;

struct MembershipPlanRecord {
	int id;
	string planName;
	int duration;
	double price;
	string benefits;
};

//int getIntegerInput(const string& message, int min, int max) {
//	int value;
//
//	while (true) {
//		cout << message;
//
//		if (cin >> value && value >= min && value <= max) {
//			return value;
//		}
//
//		cout << "Invalid input. Please enter a number from "
//			<< min << " to " << max << ".\n";
//
//		cin.clear();
//		cin.ignore(numeric_limits<streamsize>::max(), '\n');
//	}
//}

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

vector<MembershipPlanRecord> loadMembershipPlans(const string& filename) {
	vector<MembershipPlanRecord> plans;
	ifstream file(filename);
	if (!file.is_open()) return plans;

	string line;
	while (getline(file, line)) {
		if (line.empty()) continue;
		stringstream ss(line);
		MembershipPlanRecord p;
		string idStr, durationStr, priceStr;

		getline(ss, idStr, ',');
		getline(ss, p.planName, ',');
		getline(ss, durationStr, ',');
		getline(ss, priceStr, ',');
		getline(ss, p.benefits, '\n');

		try {
			p.id = stoi(idStr);
			p.duration = stoi(durationStr);
			p.price = stod(priceStr);
		}
		catch (...) {
			continue; // skip malformed line instead of crashing
		}

		plans.push_back(p);
	}
	file.close();
	return plans;
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

//string getNonEmptyString(const string& message) {
//	string value;
//
//	while (true) {
//		cout << message;
//		getline(cin >> ws, value);
//
//		if (!value.empty()) {
//			return value;
//		}
//
//		cout << "Input cannot be empty. Please try again.\n";
//	}
//}
//
//void pauseScreen() {
//	cout << "\nPress ENTER to continue...";
//	cin.ignore(numeric_limits<streamsize>::max(), '\n');
//	cin.get();
//}


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
	cout << "4. Trainer Management\n";
	cout << "5. View Payment Records\n";
	cout << "6. Attendance";
	cout << "7. Reports and Analytics\n";
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
	cout << "               CLASSES & SCHEDULE				   " << endl;
	cout << "==================================================" << endl;
	cout << "1. Add Class\n";
	cout << "2. View Class Schedule\n";
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

	cout << "Enter plan name (no spaces, e.g. Quarterly / Premium_Pass): ";
	cin >> p.planName;

	do {
		cout << "Enter Duration (in months, e.g., 1, 3, 6, 12): ";
		if (cin >> p.duration && p.duration > 0) break;

		cin.clear();
		cin.ignore(1000, '\n');
		cout << "Invalid duration. Please enter a positive number." << endl;
	} while (true);

	do {
		cout << "Enter Price (RM, e.g., 150.00): ";
		if (cin >> p.price && p.price >= 0) break;

		cin.clear();
		cin.ignore(1000, '\n');
		cout << "Invalid price. Please enter a valid amount." << endl;
	} while (true);
	cin.ignore(1000, '\n');

	cout << "\nEnter Plan Benefits separated by semicolons ';'" << endl;
	cout << "Example: Free locker access; Unlimited gym entrance" << endl;
	cout << "Benefits: ";

	getline(cin, p.benefits);

	if (p.benefits.empty()) {
		p.benefits = "Standard gym access"; // Default fallback if left blank
	}

	plans.push_back(p);
	saveMembershipPlans(filename, plans);

	cout << "The new membership is successfully added." << endl;
	cout << "\n------------------------------------------------" << endl;
	cout << "           CONFIRM NEW PLAN DETAILS             " << endl;
	cout << "------------------------------------------------" << endl;
	cout << "Plan ID   : " << p.id << endl;
	cout << "Plan Name : " << p.planName << endl;
	cout << "Duration  : " << p.duration << (p.duration == 1 ? " Month" : " Months") << endl;
	cout << "Price     : RM " << fixed << setprecision(2) << p.price << endl;
	cout << "Benefits  : " << p.benefits << endl;
	cout << "------------------------------------------------" << endl;
	cout << "\nPress ENTER back to Membership Plan Menu.\n";
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cin.get();
}

void viewMembershipPlans() {
	ifstream membershipFile("membershipPlan.txt");

	if (!membershipFile.is_open()) {
		cerr << "Error opening file!" << endl;
		return;
	}

	int no, duration;
	string planName;
	double price;
	string description;

	cout << "================================================" << endl;
	cout << "                MEMBERSHIP PLANS                " << endl;
	cout << "================================================" << endl << endl;

	cout << left << setw(5) << "No."
		<< left << setw(15) << "Plan Name"
		<< left << setw(15) << "Duration"
		<< left << setw(15) << "Price" << endl;
	cout << "------------------------------------------------" << endl;

	cout << fixed << setprecision(2);

	while (membershipFile >> no >> planName >> duration >> price) {
		string durationStr = to_string(duration) + (duration == 1 ? " Month" : " Months");
		string priceStr = "RM " + to_string(price).substr(0, to_string(price).find('.') + 3);

		cout << left << setw(5) << no
			<< left << setw(15) << planName
			<< left << setw(15) << durationStr
			<< left << setw(15) << priceStr << endl;

		string ignoreBenefits;
		getline(membershipFile, ignoreBenefits);
	}

	cout << "------------------------------------------------" << endl;

	membershipFile.clear();
	membershipFile.seekg(0);

	while (membershipFile >> no >> planName >> duration >> price) {
		string benefits;
		getline(membershipFile >> ws, benefits);

		cout << planName << " Plan Benefits:" << endl;

		stringstream ss(benefits);
		string item;

		while (getline(ss, item, ';')) {
			cout << "- " << item << endl;
		}

		cout << "------------------------------------------------" << endl;
	}

	membershipFile.close();

	cout << "\nPress ENTER back to Membership Plan Menu.\n";
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cin.get();
}

void updateMembershipPlan() {
	ifstream membershipFile("membershipPlan.txt");

	if (!membershipFile.is_open()) {
		cerr << "Error: Could not open membershipPlan.txt!" << endl;
		return;
	}

	cout << "\n================================================" << endl;
	cout << "              UPDATE MEMBERSHIP PLAN            " << endl;
	cout << "================================================" << endl << endl;

	int planID, duration;
	string planName, benefits;
	double price;

	cout << left << setw(8) << "ID"
		<< left << setw(18) << "Plan Name"
		<< left << setw(12) << "Duration"
		<< right << setw(10) << "Price (RM)" << endl;
	cout << "------------------------------------------------" << endl;

	bool hasPlans = false;
	while (membershipFile >> planID >> planName >> duration >> price) {
		getline(membershipFile >> ws, benefits); // Read benefits string
		hasPlans = true;

		string durationStr = to_string(duration) + (duration == 1 ? " Mo" : " Mos");

		cout << fixed << setprecision(2);
		cout << left << setw(8) << planID
			<< left << setw(18) << planName
			<< left << setw(12) << durationStr
			<< right << setw(10) << price << endl;
	}
	membershipFile.close();

	if (!hasPlans) {
		cout << "No membership plans found in database." << endl;
		return;
	}

	int targetID;
	cout << "\nEnter Plan ID to update (or 0 to cancel): ";
	if (!(cin >> targetID) || targetID == 0) {
		cout << "Update cancelled." << endl;
		return;
	}

	string newName, newBenefits;
	int newDuration;
	double newPrice;

	cout << "\n--- ENTER NEW PLAN DETAILS ---" << endl;
	cout << "Enter New Plan Name (no spaces, e.g. Gold_Pass): ";
	cin >> newName;

	do {
		cout << "Enter New Duration (in months): ";
		if (cin >> newDuration && newDuration > 0) break;

		cin.clear();
		cin.ignore(1000, '\n');
		cout << "Invalid input. Please enter a positive number." << endl;
	} while (true);

	do {
		cout << "Enter New Price (RM): ";
		if (cin >> newPrice && newPrice >= 0) break;

		cin.clear();
		cin.ignore(1000, '\n');
		cout << "Invalid input. Please enter a valid price." << endl;
	} while (true);

	cin.ignore(1000, '\n'); // Clear buffer before getline
	cout << "Enter New Benefits (separated by ';'): ";
	getline(cin, newBenefits);

	if (newBenefits.empty()) {
		newBenefits = "Standard gym access";
	}

	ifstream srcFile("membershipPlan.txt");
	ofstream tempFile("temp.txt");

	if (!srcFile.is_open() || !tempFile.is_open()) {
		cerr << "Error updating membership database!" << endl;
		return;
	}

	bool updated = false;
	while (srcFile >> planID >> planName >> duration >> price) {
		getline(srcFile >> ws, benefits);

		if (planID == targetID) {
			// Write NEW updated record
			tempFile << planID << ","
				<< newName << ","
				<< newDuration << " "
				<< fixed << setprecision(2) << newPrice << ","
				<< newBenefits << "\n";
			updated = true;
		}
		else {
			// Write existing unchanged record
			tempFile << planID << ","
				<< planName << ","
				<< duration << ","
				<< fixed << setprecision(2) << price << ","
				<< benefits << "\n";
		}
	}

	srcFile.close();
	tempFile.close();

	// Replace original file with updated file
	remove("membershipPlan.txt");
	rename("temp.txt", "membershipPlan.txt");

	if (updated) {
		cout << "\n================================================" << endl;
		cout << "     MEMBERSHIP PLAN UPDATED SUCCESSFULLY!      " << endl;
		cout << "================================================" << endl;
	}
	else {
		cout << "\n[ERROR] Plan ID " << targetID << " not found!" << endl;
	}
}

void deleteMembershipPlan() {
	ifstream membershipFile("membershipPlan.txt");

	if (!membershipFile.is_open()) {
		cerr << "Error: Could not open membershipPlan.txt!" << endl;
		return;
	}

	cout << "\n================================================" << endl;
	cout << "             DELETE MEMBERSHIP PLAN             " << endl;
	cout << "================================================" << endl << endl;

	int planID, duration;
	string planName, benefits;
	double price;

	cout << left << setw(8) << "ID"
		<< left << setw(18) << "Plan Name"
		<< left << setw(12) << "Duration"
		<< right << setw(10) << "Price (RM)" << endl;
	cout << "------------------------------------------------" << endl;

	bool hasPlans = false;
	while (membershipFile >> planID >> planName >> duration >> price) {
		getline(membershipFile >> ws, benefits);
		hasPlans = true;

		string durationStr = to_string(duration) + (duration == 1 ? " Mo" : " Mos");

		cout << fixed << setprecision(2);
		cout << left << setw(8) << planID
			<< left << setw(18) << planName
			<< left << setw(12) << durationStr
			<< right << setw(10) << price << endl;
	}
	membershipFile.close();

	if (!hasPlans) {
		cout << "No membership plans found in database." << endl;
		return;
	}

	int targetID;
	cout << "\nEnter Plan ID to DELETE (or 0 to cancel): ";
	if (!(cin >> targetID) || targetID == 0) {
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

	ifstream srcFile("membershipPlan.txt");
	ofstream tempFile("temp.txt");

	if (!srcFile.is_open() || !tempFile.is_open()) {
		cerr << "Error updating membership database!" << endl;
		return;
	}

	bool deleted = false;
	while (srcFile >> planID >> planName >> duration >> price) {
		getline(srcFile >> ws, benefits);

		if (planID == targetID) {
			deleted = true;
		}
		else {
			// Write existing unchanged record
			tempFile << planID << " "
				<< planName << " "
				<< duration << " "
				<< fixed << setprecision(2) << price << " "
				<< benefits << "\n";
		}
	}

	srcFile.close();
	tempFile.close();

	// Replace original file with updated temp file
	remove("membershipPlan.txt");
	rename("temp.txt", "membershipPlan.txt");

	if (deleted) {
		cout << "\n================================================" << endl;
		cout << "     MEMBERSHIP PLAN DELETED SUCCESSFULLY!      " << endl;
		cout << "================================================" << endl;
	}
	else {
		cout << "\n[ERROR] Plan ID " << targetID << " not found!" << endl;
	}
}


void adminMenu(Member* members, int userCount) {

	char choice, choose;
	vector<Schedule> schedules;
	loadSchedulesFromFile(schedules);

	do {
		displayadminMenu();
		cout << "Enter your choice: ";
		cin >> choice;


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

		case '4': // Trainer Management
			do {
				displayTrainerMenu();
				cout << "Enter your choice: ";
				cin >> choose;

			} while (choose != '0');
			break;

		case '5': // View Payment Records
			
			break;

		case '6': // Attendance Management
			
			do {
				displayAttendanceMenu();
				cout << "Enter your choice: ";
				cin >> choose;


			} while (choose != '0');
			break;

		case '7': // Reports and Analytics

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