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

    char buffer[30];
    strftime(buffer, sizeof(buffer), "%Y/%m/%d %H:%M:%S", &localTime);
    return string(buffer);
}

string addMonths(int months, const string& baseDateStr) {
    tm date = {};
    if (!baseDateStr.empty()) {
        int year, month, day;
        char s1, s2;
        stringstream ss(baseDateStr);
        if (ss >> year >> s1 >> month >> s2 >> day) {
            date.tm_year = year - 1900;
            date.tm_mon = month - 1;
            date.tm_mday = day;
            date.tm_isdst = -1;
        }
    }
    else {
        time_t now = time(nullptr);
#ifdef _WIN32
        localtime_s(&date, &now);
#else
        date = *localtime(&now);
#endif
    }

    date.tm_mon += months;
    mktime(&date); // Normalize calendar overflow

    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y/%m/%d", &date);
    return string(buffer);
}


string generatePaymentID() {

    ifstream file("UserPayment.txt");

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

void saveMembership(string username, int planID, string startDate, string endDate) {

    ifstream inFile("UserMembership.txt");
    vector<string> lines;
    bool userFound = false;
    string line;

    string newRecord = username + "," + to_string(planID) + "," + startDate + "," + endDate;

    // 1. Read existing records into a vector
    if (inFile.is_open()) {
        while (getline(inFile, line)) {
            if (line.empty()) continue;

            stringstream ss(line);
            string fileUser;
            getline(ss, fileUser, ',');

            // If this is the user's existing record, update the line
            if (fileUser == username) {
                lines.push_back(newRecord);
                userFound = true;
            }
            else {
                // Otherwise, keep the original line intact
                lines.push_back(line);
            }
        }
        inFile.close();
    }

    // 2. If the user didn't exist yet (new registration), append them to the list
    if (!userFound) {
        lines.push_back(newRecord);
    }

    // 3. Overwrite the file with the updated list
    ofstream outFile("UserMembership.txt");

    if (!outFile) {
        cout << "Error: Could not open UserMembership.txt for writing.\n";
        return;
    }

    // Loop through the updated string list and save it back to the text file
    for (size_t i = 0; i < lines.size(); i++) {
        outFile << lines[i] << "\n";
    }

    outFile.close();
}

void savePayment(string paymentID, string username, string planName, double amount, string paymentDate, string paymentMethod) {
    
    ofstream file("UserPayment.txt", ios::app);

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
    cout << "Payment ID    : " << paymentID << endl;
    cout << "Date and Time : " << paymentDate << endl;
    cout << "Username      : " << username << endl;
    cout << "------------------------------------------------" << endl;

    cout << "\nMembership Information" << endl;
    cout << "------------------------------------------------" << endl;
    cout << "Plan Name  : " << plan.planName << endl;
    cout << "Duration   : " << plan.duration << (plan.duration == 1 ? " month" : " months") << endl;
    cout << "Start Date : " << startDate << endl;
    cout << "End Date   : " << endDate << endl;
    cout << "------------------------------------------------" << endl;

    cout << "\nPayment Summary" << endl;
    cout << "------------------------------------------------" << endl;
    cout << "Payment Method : " << paymentMethod << endl;
    cout << "Amount : RM " << fixed << setprecision(2) << plan.price << endl;
    cout << "------------------------------------------------" << endl;
    cout << "TOTAL : RM " << fixed << setprecision(2) << plan.price << endl;

    cout << "\n================================================" << endl;
    cout << "       Thank you for choosing Fitness Gym!    " << endl;
    cout << "================================================" << endl;



}



void membershipPaymentProcess(Member members, MembershipPlanRecord selectedPlan, string customStartDate, string customEndDate) {

    cout << "------------------------------------------------" << endl;
	cout << "                  \nPAYMENT			         " << endl;
	cout << "------------------------------------------------" << endl;
    cout << "Plan   : " << selectedPlan.planName << endl;
    cout << "Amount : RM " << fixed << setprecision(2) << selectedPlan.price << endl;

    string methodInput;
    char methodChoice;
    string paymentMethod;

    bool paymentCompleted = false;
    string cardName, cardNumber, expDate, ccv;
    string phoneNo, pin;

    do {

        cout << "\nPayment Method" << endl;
        cout << "1. Credit / Debit Card" << endl;
        cout << "2. E-Wallet" << endl;
        cout << "0. Cancel" << endl;
        cout << "Enter payment method: ";
        cin >> methodInput;

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        // Strictly check if the user typed exactly one valid digit
        if (methodInput.length() != 1) {
            cout << "Invalid input! Please type only 1, 2, or 0.\n";
            continue; // This instantly skips the rest of the code and restarts the menu loop
        }

        methodChoice = methodInput[0];

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

                // 1. First, check if the formatting is strictly MM/YY
                if (regex_match(expDate, regex("^(0[1-9]|1[0-2])/[0-9]{2}$"))) {

                    // 2. Extract the month and year as integers
                    int inputMonth = stoi(expDate.substr(0, 2));
                    int inputYear = stoi(expDate.substr(3, 2));

                    // 3. Get the current system time
                    time_t now = time(nullptr);
                    tm ltm;

#ifdef _WIN32
                    localtime_s(&ltm, &now);
#else
                    ltm = *localtime(&now);
#endif

                    // Get the last two digits of the current year (e.g., 2026 becomes 26)
                    int currentYear = (ltm.tm_year + 1900) % 100;
                    int currentMonth = ltm.tm_mon + 1; // tm_mon is 0-11, so we add 1

                    // 4. Validate that the date is in the future or current month
                    if (inputYear > currentYear || (inputYear == currentYear && inputMonth >= currentMonth)) {
                        break; // The card is valid and not expired!
                    }
                    else {
                        cout << "Invalid input. The card has already expired.\n";
                    }
                }
                else {
                    cout << "Invalid format. Please enter in MM/YY format (e.g., 04/27).\n";
                }
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
                if (regex_match(phoneNo, regex("^01[0-9]{8,9}$"))) {
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
            cout << "Payment Cancelled.\n";
            return;

        default:
            cout << "Invalid payment method. Please try again\n";
        }
    } while (!paymentCompleted && methodChoice != '0');

    cout << "\nProcessing payment...\nPayment successful!\n" << endl;

    string paymentID = generatePaymentID();
    string paymentDate = getCurrentDateTime();
    string startDate = customStartDate.empty() ? getCurrentDate() : customStartDate;
    string endDate = customEndDate.empty() ? addMonths(selectedPlan.duration) : customEndDate;

    savePayment(paymentID, members.loginInfo.usernames, selectedPlan.planName, selectedPlan.price, paymentDate, paymentMethod);

    saveMembership(members.loginInfo.usernames, selectedPlan.id, startDate, endDate);

    generateMemberReceipt(paymentID, members.loginInfo.usernames, selectedPlan, paymentDate, paymentMethod, startDate, endDate);

}

void classPaymentProcess(Member member, Schedule selectedClass, int newBookingID) {

    cout << "\n================================================" << endl;
    cout << "                    PAYMENT                     " << endl;
    cout << "================================================" << endl;

    cout << "Amount: RM " << fixed << setprecision(2) << selectedClass.price << endl;

    string methodInput;
    char methodChoice;
    string paymentMethod;

    bool paymentCompleted = false;
    string cardName, cardNumber, expDate, ccv;
    string phoneNo, pin;

    do {

        cout << "\nPayment Method" << endl;
        cout << "1. Credit / Debit Card" << endl;
        cout << "2. E-Wallet" << endl;
        cout << "0. Cancel" << endl;
        cout << "Enter payment method: ";
        cin >> methodInput;

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        // Strictly check if the user typed exactly one valid digit
        if (methodInput.length() != 1) {
            cout << "Invalid input! Please type only 1, 2, or 0.\n";
            continue; // This instantly skips the rest of the code and restarts the menu loop
        }

        methodChoice = methodInput[0];

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

                // check if the formatting is strictly MM/YY
                if (regex_match(expDate, regex("^(0[1-9]|1[0-2])/[0-9]{2}$"))) {

                    //Extract the month and year as integers
                    int inputMonth = stoi(expDate.substr(0, 2));
                    int inputYear = stoi(expDate.substr(3, 2));

                    // Get the current system time
                    time_t now = time(nullptr);
                    tm ltm;

#ifdef _WIN32
                    localtime_s(&ltm, &now);
#else
                    ltm = *localtime(&now);
#endif

                    // Get the last two digits of the current year (e.g., 2026 becomes 26)
                    int currentYear = (ltm.tm_year + 1900) % 100;
                    int currentMonth = ltm.tm_mon + 1; // tm_mon is 0-11, so we add 1

                    // Validate that the date is in the future or current month
                    if (inputYear > currentYear || (inputYear == currentYear && inputMonth >= currentMonth)) {
                        break; // The card is valid and not expired!
                    }
                    else {
                        cout << "Invalid input. The card has already expired.\n";
                    }
                }
                else {
                    cout << "Invalid format. Please enter in MM/YY format (e.g., 04/27).\n";
                }
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
                if (regex_match(phoneNo, regex("^01[0-9]{8,9}$"))) {
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
            cout << "Payment Cancelled.\n";
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