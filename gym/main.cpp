#include<iostream>
#include<limits>

#include "User.hpp"
using namespace std;

int main() {

	char choice;
	

	do {
		clearScreen();
		cout << "   -------           --------\n";
		cout << "   |     |---------- |      |\n";
		cout << "   |                        |\n";
		cout << "   |     |---------- |      |\n";
		cout << "   -------           --------\n";

		cout << "================================" << endl;
		cout << "    WELCOME TO FITNESS GYM!!" << endl;
		cout << "================================" << endl;

		cout << "1. User" << endl;
		cout << "2. Admin" << endl;
		cout << "0. Exit" << endl;
		cout << "--------------------------------" << endl;



		cout << "Enter your choice: ";
		cin >> choice;

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
		}
	} while (choice != '0');


	return 0;
}
