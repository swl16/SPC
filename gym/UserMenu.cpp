#include<iostream>
#include<iomanip>
#include<vector>
#include<string>
#include<fstream>
#include<sstream>

#include "User.hpp"
//#include "ScheduleMenu.hpp"

using namespace std;

extern string loggedInUser;
extern int userCount;

void displayUserMenu() {
	cout << "==================================================" << endl;
	cout << "                FITNESS MEMBER PORTAL			   " << endl;
	cout << "==================================================" << endl;
	cout << "1. View My Profile\n";
	cout << "2. Edit My Profile\n";
	cout << "3. View Membership Plans\n";
	cout << "4. Renew Membership\n";
	cout << "5. View Class Schedule\n";
	cout << "6. Book Gym Class\n";
	cout << "7. Book Personal Trainer\n";
	cout << "8. View My Bookings\n";
	cout << "9. Cancel Booking\n";
	cout << "10. Make Payment\n";
	cout << "11. View Payment History\n";
	cout << "12. Gym Check - In\n";
	cout << "13. Gym Check - Out\n";
	cout << "14. Record Fitness Progress\n";
	cout << "15. View Fitness Progress\n";
	cout << "0. Logout\n";
	cout << "--------------------------------------------------" << endl;
}

int loggedInMember(Member members[]) {

	for (int i = 0; i < userCount; i++) {
		if(members[i].loginInfo.usernames == loggedInUser) {
			return i; // Return the index of the logged-in member
		}
	}
	return -1; // Return -1 if no logged-in member is found
}

void viewProfile(Member* members) {

	int i = loggedInMember(members);

	if (i == -1) {
		cout << "Error: No user is currently logged in.\n";
		return;
	}
	cout << "================================\n";
	cout << "         USER PROFILE           \n";
	cout << "================================\n";
	cout << "Username: " << members[i].loginInfo.usernames << endl;
	cout << "Name: " << members[i].name << endl;
	cout << "Age: " << members[i].age << endl;
	cout << "Gender: " << members[i].gender << endl;
	cout << "Phone Number: " << members[i].phNo << endl;
	cout << "Email: " << members[i].email << endl;
}

void editProfile(Member* members) {

	int i = loggedInMember(members);

	if(i == -1) {
		cout << "Error: No user is currently logged in.\n";
		return;
	}
	char choice;

	do {

		cout << "================================\n";
		cout << "         EDIT PROFILE           \n";
		cout << "================================\n";
		cout << "1. Edit Name\n";
		cout << "2. Edit Age\n";
		cout << "3. Edit Gender\n";
		cout << "4. Edit Phone Number\n";
		cout << "5. Edit Email\n";
		cout << "0. Back to Main Menu\n";
		cout << "--------------------------------\n";
		cout << "Enter your choice: ";
		cin >> choice;

		cin.ignore(); // Clear the input buffer

		switch (choice){
		case '1':
			cout << "Enter new name: ";
			getline(cin, members[i].name);
			cout << "Name updated successfully.\n";
			break;
		case'2':
			cout << "Enter new age: ";
			cin >> members[i].age;
			cout << "Age updated successfully.\n";
			break;
		case '3':
			cout << "Enter new gender (M/F): ";
			cin >> members[i].gender;
			cout << "Gender updated successfully.\n";
			break;
		case '4':
			cout << "Enter new phone number: ";
			cin >> members[i].phNo;
			cout << "Phone number updated successfully.\n";
			break;
		case '5':
			cout << "Enter new email: ";
			cin >> members[i].email;
			cout << "Email updated successfully.\n";
			break;

		default:
			cout << "Invalid choice. Please try again.\n";
			break;
		}

	}while(choice != '0');

	saveUser(members); // Save the updated profile to the file
}

void viewMembershipPlans() {
}

void renewMembership() {
}

void viewClassSchedule() {
}

void bookGymClass() {
}

void bookPersonalTrainer() {
}

void viewBookings() {
}

void cancelBooking() {
}

void makePayment() {
}

void viewPaymentHistory() {
}

void gymCheckIn() {
}

void gymCheckOut() {
}

void recordFitnessProgress() {
}

void viewFitnessProgress() {
}

void userMenu(Member* members) {

	int choice;

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
	cout << "14. View Fitness Progress\n";
	cout << "0. Logout\n";
	cout << "--------------------------------------------------" << endl;
	cout << "Enter your choice : ";
	cin >> choice;

	switch (choice) {
	case 1:
		viewProfile(members);
		break;

	case 2:
		editProfile(members);
		break;

	case 3:
		break;

	case 4:
		break;

	case 5:
		break;

	case 6:
		break;

	case 7:
		break;

	case 8:
		break;

	case 9:
		break;

	case 10:
		break;

	case 11:
		break;

	case 12:
		break;

	case 13:
		break;

	case 14:
		break;

	case 15:
		break;

	case 0:
		cout << "Logging out...\n";
		break;

	default:
		cout << "Invalid choice. Please try again.\n";
		break;

	}while (choice != 0);

}