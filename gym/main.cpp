#include<iostream>
#include<limits>

#include "User.hpp"
using namespace std;

int main() {

	string choiceInput;
	char choice;
	

	do {
		// choosing user or admin menu
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
		cin >> choiceInput;

		// input validation
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		if (choiceInput.length() != 1) {
			cout << "Invalid input! Please enter a single digit.\n";
			choice = ' '; // Reset to safe default
			pauseScreen(); // Pause so they can read the error before the screen clears
			continue; // Restart the main menu loop
		}
		// take the first index of the input
		choice = choiceInput[0];

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
