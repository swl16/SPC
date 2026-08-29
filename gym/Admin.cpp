#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
#include <cstdio>
#include <algorithm>
#include <map>

#include "User.hpp"
#include "ScheduleMenu.hpp"
#include "AttendanceTracker.hpp"

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
	clearScreen();

	cout << "\n=================================" << endl;
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

		pauseScreen();
	}
}

void displayadminMenu() {

	cout << "==================================================" << endl;
	cout << "                FITNESS ADMIN PORTAL			   " << endl;
	cout << "==================================================" << endl;
	cout << "1. View All Members\n";
	cout << "2. Membership Plans\n";
	cout << "3. Classes & Schedule\n";
	cout << "4. View Payment Records\n";
	cout << "5. Attendance\n";
	cout << "6. Reports\n";
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
	cout << "1. Calculate Daily Attendance\n";
	cout << "2. Identify Peak Hours\n";
	cout << "0. Back to Admin Menu\n";
	cout << "--------------------------------------" << endl;
}



void displayReportsMenu() {
	cout << "==================================================" << endl;
	cout << "                    REPORTS 			   " << endl;
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

	cout << "==================================================================================" << endl;
	cout << "                                  VIEW ALL MEMBERS				   " << endl;
	cout << "==================================================================================" << endl;

	if (userCount == 0) {
		cout << "No member records found.\n";
		pauseScreen();
		return;
	}


	cout << left << setw(15) << "Username" << setw(20) << "Name" << setw(6) << "Age"
		<< setw(8) << "Gender" << setw(15) << "Phone" << "Email" << endl;
	cout << "----------------------------------------------------------------------------------\n";

	for (int i = 0; i < userCount; i++) {
		cout << left << setw(15) << members[i].loginInfo.usernames
			<< setw(20) << members[i].name
			<< setw(6) << members[i].age
			<< setw(8) << members[i].gender
			<< setw(15) << members[i].phNo
			<< members[i].email << endl;

	}

	cout << "----------------------------------------------------------------------------------" << endl;

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
	}
	else {
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

	cout << left << setw(5) << "ID "
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
		pauseScreen();
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
		pauseScreen();
		return;
	}

	bool planFound = false;

	for (auto& p : plans) {
		if (p.id == targetID) {
			planFound = true;
			char editChoice;
			bool isModified = false;

			do {
				cout << "\n================================================" << endl;
				cout << "       EDITING PLAN ID: " << p.id << endl;
				cout << "================================================" << endl;
				cout << "1. Plan Name : " << p.planName << endl;
				cout << "2. Duration  : " << p.duration << (p.duration == 1 ? " Month" : " Months") << endl;
				cout << "3. Price     : RM " << fixed << setprecision(2) << p.price << endl;
				cout << "4. Benefits  : " << p.benefits << endl;
				cout << "0. Save & Exit" << endl;
				cout << "------------------------------------------------" << endl;
				cout << "Select field to edit (or '0' to save & exit): ";
				cin >> editChoice;

				switch (editChoice) {
				case '1':
					cout << "Enter New Plan Name (e.g. Gold Pass): ";
					getline(cin >> ws, p.planName);
					isModified = true;
					cout << "Plan Name updated successfully!" << endl;
					break;

				case '2':
					do {
						cout << "Enter New Duration (in months): ";
						if (cin >> p.duration && p.duration > 0) break;

						cin.clear();
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
						cout << "Invalid duration. Please enter a positive number." << endl;
					} while (true);
					isModified = true;
					cout << "Duration updated successfully!" << endl;
					break;

				case '3':
					do {
						cout << "Enter New Price (RM): ";
						if (cin >> p.price && p.price >= 0) break;

						cin.clear();
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
						cout << "Invalid price. Please enter a valid amount." << endl;
					} while (true);
					isModified = true;
					cout << "Price updated successfully!" << endl;
					break;

				case '4':
					cout << "Enter New Benefits (separated by ';'): ";
					getline(cin >> ws, p.benefits);

					if (p.benefits.empty()) {
						p.benefits = "Standard gym access";
					}
					isModified = true;
					cout << "Benefits updated successfully!" << endl;
					break;

				case '0':
					if (isModified) {
						saveMembershipPlans(filename, plans);
						cout << "\n================================================" << endl;
						cout << "     MEMBERSHIP PLAN UPDATED SUCCESSFULLY!      " << endl;
						cout << "================================================" << endl;
					}
					else {
						cout << "\nNo changes were made." << endl;
					}
					break;

				default:
					cout << "Invalid choice! Please select an option from 0 to 4." << endl;
					break;
				}

			} while (editChoice != '0');

			break; // Finished updating target plan
		}
	}
	
	if (!planFound) {
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

void viewPaymentRecords() {

	ifstream file("UserPayment.txt");

	cout << "\n==========================================================================================\n";
	cout << "                                  ALL PAYMENT RECORDS                                     \n";
	cout << "==========================================================================================\n";

	if (!file.is_open()) {
		cout << "No payment records found in the system.\n";
		cout << "==========================================================================================\n";
		pauseScreen();
		return;
	}

	cout << left << setw(12) << "Payment ID"
		<< left << setw(15) << "Username"
		<< left << setw(22) << "Description"
		<< left << setw(12) << "Amount"
		<< left << setw(22) << "Date & Time"
		<< left << setw(15) << "Method" << endl;
	cout << "------------------------------------------------------------------------------------------\n";

	string line;
	double totalRevenue = 0.0;
	bool hasRecords = false;

	while (getline(file, line)) {
		if (line.empty()) continue;
		stringstream ss(line);
		string pID, uName, refID, desc, amtStr, pDate, pMethod;

		// Extract data separated by commas
		getline(ss, pID, ',');
		getline(ss, uName, ',');
		getline(ss, desc, ',');
		getline(ss, amtStr, ',');
		getline(ss, pDate, ',');
		getline(ss, pMethod, ',');

		hasRecords = true;

		// Safely add to total revenue
		try {
			totalRevenue += stod(amtStr);
		}
		catch (...) {}

		// Print the row neatly (truncate long descriptions so they don't break the table)
		cout << left << setw(12) << pID
			<< left << setw(15) << uName
			<< left << setw(22) << (desc.length() > 20 ? desc.substr(0, 19) + "." : desc)
			<< "RM " << left << setw(9) << amtStr
			<< left << setw(22) << pDate
			<< left << setw(15) << pMethod << endl;
	}
	file.close();

	if (!hasRecords) {
		cout << "No transaction records found.\n";
	}

	cout << "------------------------------------------------------------------------------------------\n";
	cout << "Total Revenue: RM " << fixed << setprecision(2) << totalRevenue << endl;
	cout << "==========================================================================================\n";

	pauseScreen();

}


void membershipReport() {

	cout << "\n==================================================\n";
	cout << "            MEMBERSHIP SUMMARY REPORT             \n";
	cout << "==================================================\n";

	ifstream file("UserMembership.txt");
	if (!file.is_open()) {
		cout << "[Error] Could not open UserMembership.txt\n";
		return;
	}

	vector<MembershipPlanRecord> plans = loadMembershipPlans("membershipPlan.txt");
	map<int, string> planNameMap;
	for (const auto& p : plans) {
		planNameMap[p.id] = p.planName;
	}

	map<string, int> planCounts;
	int totalActive = 0;
	string line;
	time_t now = time(nullptr);

	while (getline(file, line)) {
		if (line.empty()) continue;
		stringstream ss(line);
		string uName, pIDStr, sDate, eDate;

		getline(ss, uName, ',');
		getline(ss, pIDStr, ',');
		getline(ss, sDate, ',');
		getline(ss, eDate, ',');

		if (pIDStr.empty() || eDate.empty()) continue;

		try {
			int pID = stoi(pIDStr);

			// Parse expiration date
			tm expTm = { 0 };
			int year, month, day;
			char sep1, sep2;
			stringstream dStream(eDate);

			if (dStream >> year >> sep1 >> month >> sep2 >> day) {
				expTm.tm_year = year - 1900;
				expTm.tm_mon = month - 1;
				expTm.tm_mday = day;
				expTm.tm_isdst = -1;

				time_t expireTime = mktime(&expTm);

				// Active if expire date is in the future
				if (difftime(expireTime, now) > 0) {
					string pName = planNameMap.count(pID) ? planNameMap[pID] : ("Plan " + to_string(pID));
					planCounts[pName]++;
					totalActive++;
				}
			}
		}
		catch (...) {
			continue;
		}
	}
	file.close();

	cout << left << setw(28) << "Plan Name" << "Total Active Members" << endl;
	cout << "--------------------------------------------------\n";

	if (planCounts.empty()) {
		cout << "No active memberships found.\n";
	}
	else {
		for (const auto& pair : planCounts) {
			cout << left << setw(28) << pair.first << pair.second << endl;
		}
	}
	cout << "--------------------------------------------------\n";
	cout << "Total Active Members Across All Plans: " << totalActive << endl;
	cout << "==================================================\n";
}


void classPopularReport() {

	cout << "\n==================================================\n";
	cout << "             CLASS POPULARITY REPORT              \n";
	cout << "==================================================\n";

	vector<Schedule> schedules;
	loadSchedulesFromFile(schedules);

	ifstream file("classBookings.txt");
	if (!file.is_open()) {
		cout << "[Error] Could not open classBookings.txt\n";
		return;
	}

	map<int, int> bookingCounts;
	string line;
	int totalBookings = 0;

	while (getline(file, line)) {
		if (line.empty()) continue;
		stringstream ss(line);
		string bID, uName, cIDStr, dateB;

		getline(ss, bID, ',');
		getline(ss, uName, ',');
		getline(ss, cIDStr, ',');
		getline(ss, dateB, ',');

		try {
			int scheduleID = stoi(cIDStr);
			bookingCounts[scheduleID]++;
			totalBookings++;
		}
		catch (...) {}
	}
	file.close();

	cout << left << setw(25) << "Class Name" << "Total Bookings" << endl;
	cout << "--------------------------------------------------\n";

	if (bookingCounts.empty()) {
		cout << "No class bookings found.\n";
	}
	else {
		for (const auto& pair : bookingCounts) {
			string className = "Unknown Class (ID: " + to_string(pair.first) + ")";
			// Find the class name from the schedules vector
			for (const auto& s : schedules) {
				if (s.scheduleID == pair.first) {
					className = s.className;
					break;
				}
			}
			cout << left << setw(25) << className << pair.second << endl;
		}
	}
	cout << "--------------------------------------------------\n";
	cout << "Total Bookings Across All Classes: " << totalBookings << endl;
	cout << "==================================================\n";

}

void monthlyReport() {

	cout << "\n==================================================\n";
	cout << "              MONTHLY REVENUE REPORT              \n";
	cout << "==================================================\n";

	ifstream file("UserPayment.txt");
	if (!file.is_open()) {
		cout << "[Error] Could not open UserPayment.txt\n";
		return;
	}

	map<string, double> monthlyRevenue;
	double totalRevenue = 0.0;
	string line;

	while (getline(file, line)) {
		if (line.empty()) continue;
		stringstream ss(line);
		string pID, uName, desc, amtStr, datetime, method;

		getline(ss, pID, ',');
		getline(ss, uName, ',');
		getline(ss, desc, ',');
		getline(ss, amtStr, ',');
		getline(ss, datetime, ',');
		getline(ss, method, ',');

		try {
			double amt = stod(amtStr);
			// Extract just the "YYYY/MM" part from "YYYY/MM/DD HH:MM:SS"
			string monthStr = datetime.substr(0, 7);

			monthlyRevenue[monthStr] += amt;
			totalRevenue += amt;
		}
		catch (...) {}
	}
	file.close();

	cout << left << setw(20) << "Month (YYYY/MM)" << "Revenue (RM)" << endl;
	cout << "--------------------------------------------------\n";
	if (monthlyRevenue.empty()) {
		cout << "No revenue data found.\n";
	}
	else {
		for (const auto& pair : monthlyRevenue) {
			cout << left << setw(20) << pair.first
				<< "RM " << fixed << setprecision(2) << pair.second << endl;
		}
	}
	cout << "--------------------------------------------------\n";
	cout << "Total Revenue: RM " << fixed << setprecision(2) << totalRevenue << endl;
	cout << "==================================================\n";
}


void adminMenu(Member* members, int userCount) {

	char choice, choose;
	vector<Schedule> schedules;
	loadSchedulesFromFile(schedules);

	do {
		clearScreen();
		displayadminMenu();
		cout << "Enter your choice: ";
		cin >> choice;

		switch (choice) {
		case '1': // View All Members
			clearScreen();
			viewAllMembers();
			break;

		case '2': // Membership Plan Management
			do {
				clearScreen();
				displayMembershipPlanMenu();
				cout << "Enter your choice: ";
				cin >> choose;

				switch (choose) {
				case '1': // Call function to add membership plan
					clearScreen();
					addMembershipPlan();
					break;

				case '2': // Call function to view membership plans
					clearScreen();
					viewMembershipPlans();
					break;

				case '3': // Call function to update membership plan
					clearScreen();
					updateMembershipPlan();
					break;

				case '4': // Call function to delete membership plan
					clearScreen();
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
				clearScreen();
				displayClassMenu();
				cout << "Enter your choice: ";
				cin >> choose;

				switch (choose) {
				case '1': // add class
					clearScreen();
					addschedule(schedules);
					pauseScreen();
					break;
				case '2':   //view class schedule
					clearScreen();
					displayschedule(schedules);
					pauseScreen();
					break;
				case '3': //update class
					clearScreen();
					updateschedule(schedules);
					pauseScreen();
					break;
				case '4': //search
					clearScreen();
					searchschedule(schedules);
					pauseScreen();
					break;
				case '5': //cancel
					clearScreen();
					cancelschedule(schedules);
					break;

				case '6': //delete
					clearScreen();
					deleteSchedule(schedules);
					break;
				case '7': //check capacity
					clearScreen();
					checkClassCapacity(schedules);
					pauseScreen();
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

		case '4': //payment 
			clearScreen();
			viewPaymentRecords();
			break;

		case '5':
			do {
				clearScreen();
				displayAttendanceMenu();
				cout << "Enter your choice: ";
				cin >> choose;

				switch (choose) {
				case '1':
					clearScreen();
					calculateDailyAttendance();
					pauseScreen(); // Pause so admin can read the report
					break;

				case '2':
					clearScreen();
					identifyPeakHours();
					pauseScreen(); // Pause so admin can read the report
					break;

				case '0':
					cout << "Returning to Admin Menu...\n";
					break;
				default:
					cout << "Invalid choice.Please try again." << endl;
				}

			} while (choose != '0');

			break;

		case '6': //report

			do {

				clearScreen();
				displayReportsMenu();
				cout << "Enter your choice: ";
				cin >> choose;

				switch (choose) {
				case '1':
					clearScreen();
					membershipReport();
					pauseScreen();
					break;
				case '2':
					clearScreen();
					classPopularReport();
					pauseScreen();
					break;
				case '3':
					clearScreen();
					monthlyReport();
					pauseScreen();
					break;
				case '0':
					cout << "Returning to Admin Menu...\n";
					break;
				default:
					cout << "Invalid choice. Please try again." << endl;
					pauseScreen();
					break;
				}

			} while (choose != '0');

			break;

		case '0':
			cout << "Logging out..." << endl;
			exit(0);
			break;
		default:
			cout << "Invalid choice. Please try again." << endl;
		}

	} while (choice != '0');
}