#include<iostream>
#include<iomanip>
#include<vector>
#include<string>
#include<fstream>
#include <sstream>

#include"User.hpp"

using namespace std;

void viewMembershipPlan(Member member) {
	ifstream membershipFile("membershipPlan.txt");
	ifstream memberFile("UserMembership.txt");

	if (!membershipFile.is_open() || !memberFile.is_open()) {
		cerr << "Error opening file!" << endl;
		return;
	}

	cout << "================================================" << endl;
	cout << "                MEMBERSHIP PLANS                " << endl;
	cout << "================================================" << endl << endl;

	cout << left << setw(5) << "No."
		<< left << setw(15) << "Plan Name"
		<< left << setw(15) << "Duration"
		<< left << setw(15) << "Price" << endl;
	cout << "------------------------------------------------" << endl;

	int no, duration;
	string planName;
	double price;
	string description;

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

		cout << planName << "Plan Benefits:" << endl;

		stringstream ss(benefits);
		string item;

		while (getline(ss, item, ';')) {
			cout << "- " << item << endl;
		}

		cout << "------------------------------------------------" << endl;
	}

	membershipFile.close();
}

void membershipPlan() {
	viewMembershipPlan();

	cout << "1. Subsribe a membership plan" << endl;
	cout << "2. Renew your membership" << endl;
	cout << "0. Return to Member Menu" << endl;
}
