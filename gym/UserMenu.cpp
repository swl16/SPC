#include<iostream>
#include<iomanip>
#include<vector>
#include<string>
#include<fstream>
#include<sstream>

#include "User.hpp"

using namespace std;

extern string loggedInUser;
extern int userCount;

void viewProfile(Member* members) {

	if (loggedInUser.empty()) {
		cout << "Error: No user is currently logged in.\n";
		return;
	}
	bool found = false;

	for (int i = 0; i < userCount; i++) {
		if (members[i].loginInfo.usernames == loggedInUser) {
			cout << "================================\n";
			cout << "         USER PROFILE           \n";
			cout << "================================\n";
			cout << "Username: " << members[i].loginInfo.usernames << endl;
			cout << "Name: " << members[i].name << endl;
			cout << "Age: " << members[i].age << endl;
			cout << "Gender: " << members[i].gender << endl;
			cout << "Phone Number: " << members[i].phNo << endl;
			cout << "Email: " << members[i].email << endl;

			found = true;
			break;
		}
	}
}

void editProfile(Member* members) {

}

void userMenu(){

	string choice;

	do {

		cout << "==================================================" << endl;
		cout << "                FITNESS MEMBER PORTAL			   " << endl;
		cout << "==================================================" << endl;

		cout << "1. View My Profile\n";
		cout << "2. View Membership Plans\n";
		cout << "3. Renew Membership\n";
		cout << "4. View Class Schedule\n";
		cout << "5. Book Gym Class\n";
		cout << "6. Book Personal Trainer\n";
		cout << "7. View My Bookings\n";
		cout << "8. Cancel Booking\n";
		cout << "9. Make Payment\n";
		cout << "10. View Payment History\n";
		cout << "11. Gym Check - In\n";
		cout << "12. Gym Check - Out\n";
		cout << "13. Record Fitness Progress\n";
		cout << "14. Fitness Tracking\n";
		cout << "0. Logout\n";
		cout << "--------------------------------------------------" << endl;
		cout << "Enter your choice : ";
		cin >> choice;

	}while(choice != "0");
}