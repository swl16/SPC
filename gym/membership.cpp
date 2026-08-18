#include<iostream>
#include<iomanip>
#include<vector>
#include<string>
#include<fstream>
#include <sstream>
#include <ctime>

#include"User.hpp"

using namespace std;

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
	localtime_s(&ltm, &now);
	char buffer[20];
	strftime(buffer, sizeof(buffer), "%Y/%m/%d", &ltm);
	return string(buffer);
}

bool hasActiveMembership(const string& username) {
	ifstream memberFile("UserMembership.txt");
	if (!memberFile.is_open()) return false;

	string fileUser, planIDStr, datePurchase;
	int currentPlanId = -1;
	bool recordFound = false;

	string line;
	while (getline(memberFile, line)) {
		if (line.empty()) continue;
		stringstream ss(line);

		getline(ss, fileUser, ',');
		getline(ss, planIDStr, ',');
		getline(ss, datePurchase, ',');

		if (fileUser == username && !planIDStr.empty()) {
			try {
				currentPlanId = stoi(planIDStr);
				recordFound = true;
			}
			catch (...) {
				continue;
			}
			break;
		}
	}
	memberFile.close();

	if (!recordFound || currentPlanId == -1) return false;

	// Look up plan duration
	vector<MembershipPlanRecord> plans = loadMembershipPlans("membershipPlan.txt");
	int duration = 0;
	bool planFound = false;

	for (const auto& p : plans) {
		if (p.id == currentPlanId) {
			duration = p.duration;
			planFound = true;
			break;
		}
	}

	if (!planFound) return false;

	// Calculate expiration date
	tm purchaseTm = { 0 };
	int year, month, day;
	char sep1, sep2;
	stringstream dateStream(datePurchase);

	if (dateStream >> year >> sep1 >> month >> sep2 >> day) {
		purchaseTm.tm_year = year - 1900;
		purchaseTm.tm_mon = month - 1;
		purchaseTm.tm_mday = day;
		purchaseTm.tm_isdst = -1;

		purchaseTm.tm_mon += duration;
		time_t expireTime = mktime(&purchaseTm);
		time_t now = time(nullptr);

		// Active if expireTime is greater than or equal to the current time
		return difftime(expireTime, now) > 0;
	}

	return false;
}

void viewMembershipPlan(Member member) {
	vector<MembershipPlanRecord>membership = loadMembershipPlans("membershipPlan.txt");
	ifstream memberFile("UserMembership.txt");

	if (!memberFile.is_open()) {
		cerr << "Error opening file!" << endl;
		return;
	}

	string username, datePurchase;
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
	
		targetPlanId = stoi(planID);

		if (username == member.loginInfo.usernames) {
			hasSubscription = true;
			break; // Found active subscription!
		}
	}

	memberFile.close();

	if (hasSubscription && targetPlanId != -1) {

		tm purchaseTm = {0};
		int year, month, day;
		char slash1, slash2;

		stringstream ss(datePurchase);

		if (ss >> year >> slash1 >> month >> slash2 >> day) {
			purchaseTm.tm_year = year - 1900; // Years since 1900
			purchaseTm.tm_mon = month - 1;   // Months 0-11
			purchaseTm.tm_mday = day;
			purchaseTm.tm_isdst = -1;

			for (const auto& p : membership) {
				if (p.id == targetPlanId) {
					purchaseTm.tm_mon += p.duration;

					time_t expireTime = mktime(&purchaseTm);
					time_t now = time(nullptr);

					char expireDateBuffer[20];
					strftime(expireDateBuffer, sizeof(expireDateBuffer), "%Y/%m/%d", &purchaseTm);
					string dateExpire(expireDateBuffer);

					double secondsLeft = difftime(expireTime, now);
					int daysLeft = static_cast<int>(secondsLeft / 86400);

					cout << "================================================" << endl;
					cout << "                  USER DETAILS                  " << endl;
					cout << "================================================" << endl << endl;

					cout << "Name: " << member.name << endl;

					if (daysLeft >= 0) {
						cout << "Membership Plan Subscribed: " << p.planName << endl;
						cout << "Date Subscribed: " << datePurchase << endl;
						cout << "Status: Active" << endl;
						cout << "Expiry Date: " << dateExpire << "  (" << daysLeft << " days remaining.) " << endl << endl;
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
		cout << "================================================" << endl;
		cout << "                  USER DETAILS                  " << endl;
		cout << "================================================" << endl << endl;

		cout << "Name: " << member.name << endl;
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

void registerMembershipPlan(Member member) {

	if (hasActiveMembership(member.loginInfo.usernames)) {
		cout << "\n=================================================" << endl;
		cout << "[NOTICE] You already have an ACTIVE membership!" << endl;
		cout << "Please select Option 2 (Renew) to extend it or wait for the plan expired to register a new membership." << endl;
		cout << "=================================================" << endl;
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
				selectedIndex = i;
				break;
			}
		}

		if (selectedIndex != -1) {
			cout << "\n------------------------------------------------" << endl;
			cout << "           SELECTED PLAN CONFIRMATION           " << endl;
			cout << "------------------------------------------------" << endl;
			cout << "Plan Name : " << membership[selectedIndex].planName << endl;
			cout << "Duration  : " << membership[selectedIndex].duration << endl;
			cout << "Price     : " << membership[selectedIndex].price << endl;

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

			cout << "\nPress Enter to return to the User Menu...";
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

void renewMembership(Member member) {
	vector<MembershipPlanRecord>membership = loadMembershipPlans("membershipPlan.txt");
	ifstream memberFile("UserMembership.txt");

	if (!memberFile.is_open()) {
		cerr << "Error opening file!" << endl;
		return;
	}

	string username, datePurchase;
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

		currentPlanId = stoi(planID);

		if (username == member.loginInfo.usernames) {
			hasSubscription = true;
			break; // Found active subscription!
		}
	}

	memberFile.close();

	if (!hasSubscription || currentPlanId == -1 ) {
		cout << "\nNo existing subscription found. " << endl;
		cout << "Please use 'Subscribe a membership plan' option first." << endl;
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

	tm purchaseTm = { 0 };
	int year, month, day;
	char sep1, sep2;
	stringstream ss(datePurchase);

	ss >> year >> sep1 >> month >> sep2 >> day;
	purchaseTm.tm_year = year - 1900;
	purchaseTm.tm_mon = month - 1;
	purchaseTm.tm_mday = day;
	purchaseTm.tm_isdst = -1;

	// Calculate original expiration date
	purchaseTm.tm_mon += plan.duration;
	time_t expireTime = mktime(&purchaseTm);
	time_t now = time(nullptr);

	if (difftime(expireTime, now) <= 0) {
		cout << "\nNo existing subscription found. " << endl;
		cout << "Please choose option 1 'Subscribe a membership plan' option first." << endl;
		return;
	}

	if (plan.id == currentPlanId) {
		char currentExpireBuffer[20];
		strftime(currentExpireBuffer, sizeof(currentExpireBuffer), "%Y/%m/%d", &purchaseTm);
		string currentExpireDateStr(currentExpireBuffer);

		// Calculate NEW extended expiration date (add another duration period)
		tm newExpireTm = purchaseTm;
		newExpireTm.tm_mon += plan.duration;
		mktime(&newExpireTm); // Normalizes months (e.g. month 13 becomes next year Jan)

		// Format NEW expiration date to string
		char newExpireBuffer[20];
		strftime(newExpireBuffer, sizeof(newExpireBuffer), "%Y/%m/%d", &newExpireTm);
		string newExpireDateStr(newExpireBuffer);

		cout << "\n================================================" << endl;
		cout << "              RENEW MEMBERSHIP PLAN             " << endl;
		cout << "================================================" << endl << endl;
	
		string durationStr = to_string(plan.duration) + (plan.duration == 1 ? " Month" : " Months");
	

		cout << "Current Plan    : " << plan.planName << endl;
		cout << "Renew Duration  : " << durationStr << endl;
		cout << "New expiry date : " << newExpireDateStr << endl;
		cout << "Amount due      : " << fixed << setprecision(2) << plan.price << endl;

		char confirm;
		cout << "\nConfirm renewal? (Y/N): ";
		cin >> confirm;

		if (confirm == 'Y' || confirm == 'y') {
			// process to payment
			membershipPaymentProcess(member, plan);

		}
		else {
			cout << "Renewal cancelled." << endl;
		}

		cout << "\nPress Enter to return to the User Menu...";
		cin.ignore(1000, '\n');
		cin.get();
		return;
	}
	
}

void membershipPlan(Member member) {
	clearScreen();

	int choice; 
	do {
		viewMembershipPlan(member);

		cout << endl;
		cout << "1. Subscribe a membership plan" << endl;
		cout << "2. Renew your membership" << endl;
		cout << "0. Return to Member Menu" << endl << endl;
		cout << "Enter your choice (or '0' to return to menu): ";

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

