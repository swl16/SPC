#include<iostream>
#include<iomanip>
#include<vector>
#include<string>
#include<fstream>
#include <sstream>
#include <ctime>

#include"User.hpp"

using namespace std;

void viewMembershipPlan(Member member) {
	ifstream membershipFile("membershipPlan.txt");
	ifstream memberFile("UserMembership.txt");

	if (!membershipFile.is_open() || !memberFile.is_open()) {
		cerr << "Error opening file!" << endl;
		return;
	}

	string username, datePurchase;
	int targetPlanId = -1;
	bool hasSubscription = false;

	while (memberFile >> username >> targetPlanId >> datePurchase) {
		if (username == member.loginInfo.usernames) {
			hasSubscription = true;
			break; // Found active subscription!
		}
	}

	memberFile.close();

	int no, duration;
	string planName;
	double price;
	string description;

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

			while (membershipFile >> no >> planName >> duration >> price) {
				string ignoreBenefits;
				getline(membershipFile, ignoreBenefits);

				if (no == targetPlanId) {
					purchaseTm.tm_mon += duration;

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
						cout << "Membership Plan Subscribed: " << planName << endl;
						cout << "Date Subscribed: " << datePurchase << endl;
						cout << "Status: Active" << endl;
						cout << "Expiry Date: " << dateExpire << "  (" << daysLeft << " days remaining.) " << endl << endl;
					}
					else {
						cout << "Membership Plan Subscribed: - " << endl;
						cout << "Membership plan " << planName << " EXPIRED " << abs(daysLeft) << " days ago." << endl << endl;
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

	cout << left << setw(5) << "No."
		<< left << setw(15) << "Plan Name"
		<< left << setw(15) << "Duration"
		<< left << setw(15) << "Price" << endl;
	cout << "------------------------------------------------" << endl;


	membershipFile.clear();
	membershipFile.seekg(0);

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
}

void registerMembershipPlan() {
	ifstream membershipFile("membershipPlan.txt");

	if (!membershipFile.is_open()) {
		cerr << "Error opening file!" << endl;
		return;
	}

	int no, duration;
	string planName;
	double price;
	
	vector<int> planIDs;
	vector<string> durationStr, name, priceStr;

	while (membershipFile >> no >> planName >> duration >> price) {
		planIDs.push_back(no); 
		name.push_back(planName);
		durationStr.push_back(to_string(duration) + (duration == 1 ? " Month" : " Months"));
		priceStr.push_back("RM " + to_string(price).substr(0, to_string(price).find('.') + 3));

		string ignoreBenefits;
		getline(membershipFile, ignoreBenefits);
	}
	membershipFile.close();

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
		for (size_t i = 0; i < planIDs.size(); ++i) {
			if (planIDs[i] == choice) {
				selectedIndex = i;
				break;
			}
		}

		if (selectedIndex != -1) {
			cout << "\n------------------------------------------------" << endl;
			cout << "           SELECTED PLAN CONFIRMATION           " << endl;
			cout << "------------------------------------------------" << endl;
			cout << "Plan Name : " << name[selectedIndex] << endl;
			cout << "Duration  : " << durationStr[selectedIndex] << endl;
			cout << "Price     : " << priceStr[selectedIndex] << endl;

			char confirm;
			cout << "\nConfirm subscription? (Y/N): ";
			cin >> confirm;

			if (confirm == 'Y' || confirm == 'y') {
				//process to payment
				break;

				//string currentDate = getCurrentDate();

				//// Append new subscription to UserMembership.txt
				//ofstream userMemFile("UserMembership.txt", ios::app);
				//if (userMemFile.is_open()) {
				//	userMemFile << member.loginInfo.usernames << " "
				//		<< planIDs[selectedIndex] << " "
				//		<< currentDate << "\n";
				//	userMemFile.close();

				//	cout << "\nRegistration successful! Subscription saved." << endl;
				//	cout << "Subscribed on: " << currentDate << endl;
				//}
				//else {
				//	cerr << "Error: Could not save subscription to file." << endl;
				//}
				//break; // Exit loop after successful registration
			}
			else {
				cout << "Subscription cancelled." << endl;
			}
		}
		else {
			cout << "Invalid Plan No. Please try again." << endl;
		}
	} while (true);
}

void renewMembership() {
	ifstream membershipFile("membershipPlan.txt");

	if (!membershipFile.is_open()) {
		cerr << "Error opening file!" << endl;
		return;
	}

	int no, duration;
	string planName;
	double price;

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


}

void membershipPlan(Member member) {
	viewMembershipPlan(member);

	cout << endl;

	cout << "1. Subscribe a membership plan" << endl;
	cout << "2. Renew your membership" << endl;
	cout << "0. Return to Member Menu" << endl;
}


