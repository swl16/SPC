#include<iostream>
#include<iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>
#include <limits>

#include "User.hpp"

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

void saveMembership(string username, int planID, string planName, string startDate, string endDate, string status) {

    ofstream file("UserMembership.txt");

    if (!file) {
        cout << "Error: Cannot open file!\n";
        return;
    }

    file << username << ","
        << planID << ","
        << planName << ","
        << startDate << ","
        << endDate << ","
        << status << endl;

    file.close();
}

void savePayment(string paymentID, string username, int planID, string planName, double amount, string paymentDate, string paymentMethod) {
    
    ofstream file("paymentMembership.txt"); 

        if (!file) {
            cout << "Error opening file!\n";
            return;
        }

        file << paymentID << ","
            << username << ","
            << planID << ","
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







void membershipPayment(Member* members) {

	cout << "=====================================" << endl;
	cout << "               PAYMENT			      " << endl;
	cout << "=====================================" << endl;

}