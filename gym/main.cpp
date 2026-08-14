#include<iostream>
#include<iomanip>

#include "User.hpp"
using namespace std;

int main() {
 
	Member* members[MAX_USERS];
	int userCount = 0;

	char choice;

	cout << "================================" << endl;
	cout << "    WELCOME TO FITNESS GYM!!" << endl;
	cout << "================================" << endl;

	cout << "1. User" << endl;
	cout << "2. Admin" << endl;
	cout << "0. Exit" << endl;

	do {
		cout << "Enter your choice: ";
		cin >> choice;

		if (cin.fail()) {
			if (cin.eof()) {
				cout << "\nInput closed. Exiting." << endl;
				break;
			}
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Invalid input. Please try again." << endl;
			continue;
		}

		switch (choice) {
		case '1':
			userLogin();
			break;
		case '2':
			adminLogin();
			break;
		case '0':
			cout << "Thank you for using Fitness Gym!" << endl;
			break;
		default:
			cout << "Invalid choice. Please try again." << endl;
			break;
		}
	} while (choice != '0');


	return 0;
}
