#include<iostream>
#include<iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>
#include <limits>
#include <regex>

#include "User.hpp"
#include "ScheduleMenu.hpp"

using namespace std;


string getCurrentDateTime() {

    time_t now = time(nullptr);
    tm localTime{};

#ifdef _WIN32
    localtime_s(&localTime, &now);
#else
    localTime = *localtime(&now);
#endif

    stringstream ss;

    ss << put_time(&localTime, "%d/%m/%Y %H:%M:%S");

    return ss.str();
}

string addMonths(int months) {

    time_t now = time(nullptr);
    tm date{};

#ifdef _WIN32
    localtime_s(&date, &now);
#else
    date = *localtime(&now);
#endif

    date.tm_mon += months;

    mktime(&date);

    stringstream ss;

    ss << put_time(&date, "%d/%m/%Y");

    return ss.str();
}


string generatePaymentID() {

    ifstream file("PaymentHistory.txt");

    int lastID = 0;
    string line;

    while (getline(file, line)) {

        if (line.empty()) {
            continue;
        }

        stringstream ss(line);

        string paymentID;
        getline(ss, paymentID, ',');

        if (paymentID.length() > 2) {

            try {
                int id = stoi(paymentID.substr(2));

                if (id > lastID) {
                    lastID = id;
                }
            }
            catch(...){
                //Ignore invalid payment ID
            }
        }
    }

    file.close();

    stringstream result;

    result << "P" << setw(4) << setfill('0') << lastID + 1;

    return result.str();
}

void saveMembership(string username, string planName, string startDate, string endDate, string status) {

    ofstream file("UserMembership.txt");

    if (!file) {
        cout << "Error: Cannot open file!\n";
        return;
    }

    file << username << ","
        << planName << ","
        << startDate << ","
        << endDate << ","
        << status << endl;

    file.close();
}

void savePayment(string paymentID, string username, string planName, double amount, string paymentDate, string paymentMethod) {
    
    ofstream file("UserPayment.txt"); 

        if (!file) {
            cout << "Error opening file!\n";
            return;
        }

        file << paymentID << ","
            << username << ","
            << planName << ","
            << fixed << setprecision(2) << amount << ","
            << paymentDate << ","
            << paymentMethod << endl;

        file.close();

}

void generateMemberReceipt(string paymentID, string username, MembershipPlanRecord plan, string paymentDate, string paymentMethod,
    string startDate, string endDate) {

    cout << "================================================" << endl;
    cout << "               FITNESS GYM RECEIPT              " << endl;
    cout << "================================================" << endl;
    cout << "Payment ID    :" << paymentID << endl;
    cout << "Date and Time : " << paymentDate << endl;
    cout << "Username      : " << username << endl;
    cout << "------------------------------------------------" << endl;

    cout << "\nMembership Information" << endl;
    cout << "------------------------------------------------" << endl;
    cout << "Plan Name: " << plan.planName << endl;
    cout << "Duration : " << plan.duration << (plan.duration == 1 ? " month" : " months") << endl;
    cout << "Start Date :" << startDate << endl;
    cout << "End Date : " << endDate << endl;
    cout << "------------------------------------------------" << endl;

    cout << "\nPayment Summary" << endl;
    cout << "------------------------------------------------" << endl;
    cout << "Payment Method : " << paymentMethod << endl;
    cout << "Amount : RM " << fixed << setprecision(2) << plan.price << endl;
    cout << "------------------------------------------------" << endl;
    cout << "TOTAL : RM " << fixed << setprecision(2) << plan.price << endl;

    cout << "\n================================================" << endl;
    cout << "         Thank you for choosing Fitness Gym!    " << endl;
    cout << "================================================" << endl;



}



void membershipPaymentProcess(Member members, MembershipPlanRecord selectedPlan) {


	cout << "                  \nPAYMENT			         " << endl;
	cout << "------------------------------------------------" << endl;
    cout << "Amount : RM " << fixed << setprecision(2) << selectedPlan.price << endl;

    char methodChoice;
    string paymentMethod;

    bool paymentCompleted = false;
    string cardName, cardNumber, expDate, ccv;
    string phoneNo, pin;

    do {

        cout << "Payment Method" << endl;
        cout << "1. Credit / Debit Card" << endl;
        cout << "2. E-Wallet" << endl;
        cout << "0. Cancel" << endl;
        cout << "Enter payment method: ";

        switch (methodChoice) {

        case '1':
            paymentMethod = "Credit / Debit Card";
            cout << "\n--- " << paymentMethod << " Details ---\n";
            cin.ignore();

            while (true) {
                cout << "Enter Cardholder Name: ";
                getline(cin, cardName);
                if (!cardName.empty() && regex_match(cardName, regex("^[A-Za-z\\s]+$"))) {
                    break;
                }
                cout << "Invalid name. Please use alphabetic characters only.\n";
            }

            while (true) {
                cout << "Enter 12-digit Card Number: ";
                cin >> cardNumber;
                if (regex_match(cardNumber, regex("^[0-9]{12}$"))) {
                    break;
                }
                cout << "Invalid card number. Must be exactly 12 digits.\n";
            }

            while (true) {
                cout << "Enter Expiry Date (MM/YY): ";
                cin >> expDate;
                if (regex_match(expDate, regex("^(0[1-9]|1[0-2])/[0-9]{2}$"))) {
                    break;
                }
                cout << "Invalid format. Please enter in MM/YY format (e.g., 04/27).\n";
            }

            while (true) {
                cout << "Enter CCV: ";
                cin >> ccv;
                if (regex_match(ccv, regex("^[0-9]{3}$"))) {
                    break;
                }
                cout << "Invalid CCV. Must be exactly 3 digits.\n";
            }

            paymentCompleted = true;

            break;
        case '2':
            paymentMethod = "E-Wallet";
            cout << "\n--- " << paymentMethod << " Details ---\n";

            while (true) {
                cout << "Enter Phone Number (10-11 digits without '-'): ";
                cin >> phoneNo;
                if (regex_match(phoneNo, regex("^[0-9]{10,11}$"))) {
                    break;
                }
                cout << "Invalid phone number. Must be 10 or 11 digits.\n";
            }

            while (true) {
                cout << "Enter 6-digit PIN: ";
                cin >> pin;
                if (regex_match(pin, regex("^[0-9]{6}$"))) {
                    break;
                }
                cout << "Invalid PIN. Must be exactly 6 digits.\n";
            }

            paymentCompleted = true;
            
            break;
        case '0':
            cout << "Payment Cancelled. Returning to previous menu...\n";
            return;

        default:
            cout << "Invalid payment method. Please try again\n";
        }
    } while (!paymentCompleted && methodChoice != '0');

    cout << "\nProcessing payment...\nPayment successful!\n" << endl;

    string paymentID = generatePaymentID();
    string paymentDate = getCurrentDateTime();
    string startDate = addMonths(0);
    string endDate = addMonths(selectedPlan.duration);

    savePayment(paymentID, members.loginInfo.usernames, selectedPlan.planName, selectedPlan.price, paymentDate, paymentMethod);

    saveMembership(members.loginInfo.usernames, selectedPlan.planName, startDate, endDate, "Active");

    generateMemberReceipt(paymentID, members.loginInfo.usernames, selectedPlan, paymentDate, paymentMethod, startDate, endDate);

}

void classPaymentProcess(Member member, Schedule selectedClass, int newBookingID) {

    cout << "\n================================================" << endl;
    cout << "                    PAYMENT                     " << endl;
    cout << "================================================" << endl;

    cout << "Amount: RM " << fixed << setprecision(2) << selectedClass.price << endl;

    char methodChoice;
    string paymentMethod;

    bool paymentCompleted = false;
    string cardName, cardNumber, expDate, ccv;
    string phoneNo, pin;

    do {

        cout << "Payment Method" << endl;
        cout << "1. Credit / Debit Card" << endl;
        cout << "2. E-Wallet" << endl;
        cout << "0. Cancel" << endl;
        cout << "Enter payment method: ";

        switch (methodChoice) {

        case '1':
            paymentMethod = "Credit / Debit Card";
            cout << "\n--- " << paymentMethod << " Details ---\n";
            cin.ignore();

            while (true) {
                cout << "Enter Cardholder Name: ";
                getline(cin, cardName);
                if (!cardName.empty() && regex_match(cardName, regex("^[A-Za-z\\s]+$"))) {
                    break;
                }
                cout << "Invalid name. Please use alphabetic characters only.\n";
            }

            while (true) {
                cout << "Enter 12-digit Card Number: ";
                cin >> cardNumber;
                if (regex_match(cardNumber, regex("^[0-9]{12}$"))) {
                    break;
                }
                cout << "Invalid card number. Must be exactly 12 digits.\n";
            }

            while (true) {
                cout << "Enter Expiry Date (MM/YY): ";
                cin >> expDate;
                if (regex_match(expDate, regex("^(0[1-9]|1[0-2])/[0-9]{2}$"))) {
                    break;
                }
                cout << "Invalid format. Please enter in MM/YY format (e.g., 04/27).\n";
            }

            while (true) {
                cout << "Enter CCV: ";
                cin >> ccv;
                if (regex_match(ccv, regex("^[0-9]{3}$"))) {
                    break;
                }
                cout << "Invalid CCV. Must be exactly 3 digits.\n";
            }

            paymentCompleted = true;

            break;
        case '2':
            paymentMethod = "E-Wallet";
            cout << "\n--- " << paymentMethod << " Details ---\n";

            while (true) {
                cout << "Enter Phone Number (10-11 digits without '-'): ";
                cin >> phoneNo;
                if (regex_match(phoneNo, regex("^[0-9]{10,11}$"))) {
                    break;
                }
                cout << "Invalid phone number. Must be 10 or 11 digits.\n";
            }

            while (true) {
                cout << "Enter 6-digit PIN: ";
                cin >> pin;
                if (regex_match(pin, regex("^[0-9]{6}$"))) {
                    break;
                }
                cout << "Invalid PIN. Must be exactly 6 digits.\n";
            }

            paymentCompleted = true;

            break;
        case '0':
            cout << "Payment Cancelled. Returning to previous menu...\n";
            return;

        default:
            cout << "Invalid payment method. Please try again\n";
        }
    } while (!paymentCompleted && methodChoice != '0');

    string paymentID = generatePaymentID();
    string paymentDate = getCurrentDateTime();

    time_t now = time(nullptr);
    tm ltm;
#ifdef _WIN32
    localtime_s(&ltm, &now);
#else
    ltm = *localtime(&now);
#endif
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y/%m/%d", &ltm);
    string dateBooked = string(buffer);

    // 1. Save Booking Data to classBookings.txt
    ofstream outFile("classBookings.txt", ios::app);
    if (outFile.is_open()) {
        outFile << newBookingID << ","
            << member.loginInfo.usernames << ","
            << selectedClass.scheduleID << ","
            << dateBooked << "\n";
        outFile.close();
    }
    else {
        cout << "Error: Could not write to classBookings.txt\n";
    }

    savePayment(paymentID, member.loginInfo.usernames, selectedClass.className, selectedClass.price, paymentDate, paymentMethod);

    cout << "\n================================================" << endl;
    cout << "              CLASS BOOKING RECEIPT             " << endl;
    cout << "================================================" << endl;
    cout << "Payment ID    : " << paymentID << endl;
    cout << "Date and Time : " << paymentDate << endl;
    cout << "Username      : " << member.loginInfo.usernames << endl;
    cout << "------------------------------------------------" << endl;

    cout << "Class Information" << endl;
    cout << "------------------------------------------------" << endl;
    cout << "Booking ID : " << newBookingID << endl;
    cout << "Class Name : " << selectedClass.className << endl;
    cout << "Trainer    : " << (selectedClass.trainerName.empty() ? "None" : selectedClass.trainerName) << endl;
    cout << "Class Date : " << selectedClass.date << endl;
    cout << "Time Slot  : " << selectedClass.startTime << " - " << selectedClass.endTime << endl;
    cout << "------------------------------------------------" << endl;

    cout << "Payment Summary" << endl;
    cout << "------------------------------------------------" << endl;
    cout << "Method : " << paymentMethod << endl;
    cout << "Amount : RM " << fixed << setprecision(2) << selectedClass.price << endl;
    cout << "------------------------------------------------" << endl;
    cout << "TOTAL  : RM " << fixed << setprecision(2) << selectedClass.price << endl;
    cout << "================================================" << endl;
    cout << "      See you at the gym! Enjoy your class!     " << endl;
    cout << "================================================\n" << endl;
}