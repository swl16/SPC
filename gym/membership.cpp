#include<iostream>
#include<iomanip>
#include<vector>
#include<string>
#include<fstream>
#include <sstream>
#include <ctime>

#include"User.hpp"

using namespace std;

//Function to get all membership plan
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

		if (idStr.empty()) continue;

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

string getCurrentDate() {
	time_t now = time(nullptr);
	tm ltm = {};
#ifdef _WIN32
	localtime_s(&ltm, &now);
#else
	ltm = *localtime(&now);
#endif
	char buffer[20];
	strftime(buffer, sizeof(buffer), "%Y/%m/%d", &ltm);
	return string(buffer);
}

// check if the user has a active membership
bool hasActiveMembership(const string& username) {
	ifstream memberFile("UserMembership.txt");
	if (!memberFile.is_open()) return false;

	string fileUser, planIDStr, datePurchase, expDate;
	string latestExpDate = "";
	bool recordFound = false;

	string line;
	while (getline(memberFile, line)) {
		if (line.empty()) continue;
		stringstream ss(line);

		getline(ss, fileUser, ',');
		getline(ss, planIDStr, ',');
		getline(ss, datePurchase, ',');
		getline(ss, expDate, ',');

		if (fileUser == username && !planIDStr.empty()) {
			try {
				latestExpDate = expDate;
				recordFound = true;
			}
			catch (...) {
				continue;
			}
			break;
		}
	}
	memberFile.close();

	if (!recordFound || latestExpDate.empty()) return false;

	tm expTm = { 0 };
	int year, month, day;
	char sep1, sep2;
	stringstream dateStream(latestExpDate);

	if (dateStream >> year >> sep1 >> month >> sep2 >> day) {
		expTm.tm_year = year - 1900;
		expTm.tm_mon = month - 1;
		expTm.tm_mday = day;
		expTm.tm_isdst = -1;

		time_t expiredTimestamp = mktime(&expTm);
		time_t now = time(nullptr);

		return difftime(expiredTimestamp, now) > 0;
	}

	return false;
}

//display available membership plan and the status of membership of the user 
void viewMembershipPlan(Member member) {
	vector<MembershipPlanRecord>membership = loadMembershipPlans("membershipPlan.txt");
	ifstream memberFile("UserMembership.txt");

	if (!memberFile.is_open()) {
		cerr << "Error opening UserMembership.txt file!" << endl;
		return;
	}

	if (membership.empty()) {
		cout << "No membership plans found in the database.";
		pauseScreen();
		return;
	}

	string username, datePurchase, expiredDate;
	int targetPlanId = -1;
	bool hasSubscription = false;

	string line;
	while (getline(memberFile, line)) {
		if (line.empty()) continue;
		stringstream ss(line);
		string planID;

		getline(ss, username, ',');
		getline(ss, planID, ',');
		getline(ss, datePurchase, ',');
		getline(ss, expiredDate, ',');
	
		if (planID.empty()) continue;

		try {
			int parsedId = stoi(planID);

			if (username == member.loginInfo.usernames) {
				targetPlanId = parsedId;
				hasSubscription = true;
			}
		}
		catch (const invalid_argument&) {
			continue; // Skip malformed lines safely without crashing
		}
	}
	memberFile.close();

	cout << "================================================" << endl;
	cout << "                  USER DETAILS                  " << endl;
	cout << "================================================" << endl << endl;
	cout << "Name: " << member.name << endl;

	if (hasSubscription && targetPlanId != -1) {
		tm expiredTm = {0};
		int year, month, day;
		char slash1, slash2;
		stringstream ss(expiredDate);

		if (ss >> year >> slash1 >> month >> slash2 >> day) {
			expiredTm.tm_year = year - 1900; // Years since 1900
			expiredTm.tm_mon = month - 1;   // Months 0-11
			expiredTm.tm_mday = day;
			expiredTm.tm_isdst = -1;

			for (const auto& p : membership) {
				if (p.id == targetPlanId) {
					
					time_t expireTime = mktime(&expiredTm);
					time_t now = time(nullptr);

					double secondsLeft = difftime(expireTime, now);
					int daysLeft = static_cast<int>(secondsLeft / 86400);

					if (daysLeft >= 0) {
						cout << "Membership Plan Subscribed: " << p.planName << endl;
						cout << "Date Subscribed: " << datePurchase << endl;
						cout << "Status: Active" << endl;
						cout << "Expiry Date: " << expiredDate << "  (" << daysLeft << " days remaining.) " << endl << endl;
					}
					else {
						cout << "Membership Plan Subscribed: - " << endl;
						cout << "Membership plan " << p.planName << " EXPIRED " << abs(daysLeft) << " days ago." << endl << endl;
					}
					break;
				}
			}
		}
	}
	else {
		cout << "Membership Plan Subscribed: - " << endl << endl;
	}
	
	cout << "================================================" << endl;
	cout << "                MEMBERSHIP PLANS                " << endl;
	cout << "================================================" << endl << endl;

	cout << fixed << setprecision(2);

	for (const auto& p : membership) {
		string durationStr = to_string(p.duration) + (p.duration == 1 ? " Month" : " Months");
	
		cout << "\nPlan ID   : " << p.id << endl;
		cout << "Plan Name : " << p.planName << endl;
		cout << "Duration  : " << durationStr << endl;
		cout << "Price     : RM " << fixed << setprecision(2) << p.price << endl;
		cout << "Benefits  :" << endl;

		stringstream benefitStream(p.benefits);
		string singleBenefit;
		while (getline(benefitStream, singleBenefit, ';')) {
			if (!singleBenefit.empty()) {
				cout << "  - " << singleBenefit << endl;
			}
		}

		cout << "------------------------------------------------" << endl;
	}
}

//Let user to register a new membership plan (with no active membership plan)
void registerMembershipPlan(Member member) {

	if (hasActiveMembership(member.loginInfo.usernames)) {
		cout << "\n=================================================" << endl;
		cout << "[NOTICE] You already have an ACTIVE membership!" << endl;
		cout << "Please select Option 2 (Renew) to extend it or wait for the plan expired to register a new membership." << endl;
		cout << "=================================================" << endl;
		pauseScreen();
		return;
	}

	vector<MembershipPlanRecord>membership = loadMembershipPlans("membershipPlan.txt");

	if (membership.empty()) {
		cout << "No membership plans available to register." << endl;
		return;
	}

	int choice;
	cout << "================================================" << endl;
	cout << "            REGISTER MEMBERSHIP PLAN            " << endl;
	cout << "================================================" << endl << endl;
	
	do {
		cout << "Enter the plan No. you want to subscribe (or '0' to return to menu): ";
		if (!(cin >> choice)) { // Input validation for non-numeric input
			cin.clear();
			cin.ignore(1000, '\n');
			cout << "Invalid input. Please enter a number." << endl;
			continue;
		}

		if (choice == 0) { 
			cout << "Returning to menu..." << endl;
			// call main menu func
			break;
		}

		int selectedIndex = -1;
		for (size_t i = 0; i < membership.size(); ++i) {
			if (membership[i].id == choice) {
				selectedIndex = static_cast<int>(i);
				break;
			}
		}

		if (selectedIndex != -1) {
			cout << "\n------------------------------------------------" << endl;
			cout << "           SELECTED PLAN CONFIRMATION           " << endl;
			cout << "------------------------------------------------" << endl;
			cout << "Plan Name : " << membership[selectedIndex].planName << endl;
			cout << "Duration  : " << membership[selectedIndex].duration << (membership[selectedIndex].duration == 1 ? " Month" : " Months") << endl;
			cout << "Price     : RM" << fixed << setprecision(2) << membership[selectedIndex].price << endl;

			char confirm;
			cout << "\nConfirm subscription? (Y/N): ";
			cin >> confirm;

			if (confirm == 'Y' || confirm == 'y') {
				//process to payment
				membershipPaymentProcess(member, membership[selectedIndex]);
				
			}
			else {
				cout << "Subscription cancelled." << endl;
			}

			cout << "\nPress Enter to return";
			cin.ignore(1000, '\n');
			cin.get();
			return;
		}
		else {
			cout << "Invalid Plan No. Please try again." << endl;
			continue;
		}
	} while (true);
}

//Let users with active plan to extend their membership plan 
void renewMembership(Member member) {
	vector<MembershipPlanRecord>membership = loadMembershipPlans("membershipPlan.txt");
	ifstream memberFile("UserMembership.txt");

	if (!memberFile.is_open()) {
		cerr << "Error opening file!" << endl;
		return;
	}

	string username, datePurchase, expiredDate;
	int currentPlanId = -1;
	bool hasSubscription = false;
	
	string line;
	while (getline(memberFile, line)) {
		if (line.empty()) continue;
		stringstream ss(line);
		string planID;

		getline(ss, username, ',');
		getline(ss, planID, ',');
		getline(ss, datePurchase, ',');
		getline(ss, expiredDate, ',');

		if (planID.empty()) continue;

		try {
			int parsedId = stoi(planID);

			if (username == member.loginInfo.usernames) {
				currentPlanId = parsedId;
				hasSubscription = true;
			}
		}
		catch (const invalid_argument&) {
			continue; // Skip malformed lines safely without crashing
		}
	}
	memberFile.close();

	if (!hasSubscription || currentPlanId == -1 ) {
		cout << "\nNo existing subscription found. " << endl;
		cout << "Please use Option 1 ('Subscribe a membership plan') option first." << endl;
		return;
	}
	
	MembershipPlanRecord plan = {0,"",0,0.0,""};
	for (const auto& p : membership) {
		if (p.id == currentPlanId) {
			plan = p;
			break;
		}
	}
	
	if (plan.id == 0) {
		cout << "\nError: Your subscribed plan is no longer available in the catalog.\n";
		return;
	}

	tm expiredTm = { 0 };
	int year, month, day;
	char sep1, sep2;
	stringstream ss(expiredDate);

	ss >> year >> sep1 >> month >> sep2 >> day;
	expiredTm.tm_year = year - 1900;
	expiredTm.tm_mon = month - 1;
	expiredTm.tm_mday = day;
	expiredTm.tm_isdst = -1;

	time_t expireTime = mktime(&expiredTm);
	time_t now = time(nullptr);

	if (difftime(expireTime, now) <= 0) {
		cout << "\nYour subscription has already expired." << endl;
		cout << "Please choose Option 1 ('Subscribe a membership plan') to start a new subscription." << endl;
		return;
	}

	string newExpireDateStr = addMonths(plan.duration, expiredDate);

	cout << "\n================================================" << endl;
	cout << "              RENEW MEMBERSHIP PLAN             " << endl;
	cout << "================================================" << endl << endl;
	
	string durationStr = to_string(plan.duration) + (plan.duration == 1 ? " Month" : " Months");
	

	cout << "Current Plan    : " << plan.planName << endl;
	cout << "Renew Duration  : " << durationStr << endl;
	cout << "Current Expiry  : " << expiredDate << endl;
	cout << "New expiry date : " << newExpireDateStr << endl;
	cout << "Amount due      : RM" << fixed << setprecision(2) << plan.price << endl;

	char confirm;
	cout << "\nConfirm renewal? (Y/N): ";
	cin >> confirm;

	if (confirm == 'Y' || confirm == 'y') {
		// process to payment
		membershipPaymentProcess(member, plan, datePurchase, newExpireDateStr);

	}
	else {
		cout << "Renewal cancelled." << endl;
	}

	pauseScreen();	
}

//Menu for membership plan module
void membershipPlan(Member member) {
	clearScreen();

	int choice; 
	do {
		viewMembershipPlan(member);

		cout << endl;
		cout << "1. Subscribe a membership plan" << endl;
		cout << "2. Renew your membership" << endl;
		cout << "0. Return to Member Menu" << endl << endl;
		cout << "Enter your choice: ";

		if (!(cin >> choice)) { // Input validation for non-numeric input
			cin.clear();
			cin.ignore(1000, '\n');
			cout << "Invalid input. Please enter a number." << endl;
			continue;
		}

		switch (choice) {
		case 1:
			registerMembershipPlan(member);
			break;

		case 2:
			renewMembership(member);
			break;

		case 0:
			cout << "Returning to menu..." << endl;
			return;
			// call main menu func
			break;
		
		default:
			cout << "Invalid Plan No. Please try again." << endl;
		}
	} while (true);
}

