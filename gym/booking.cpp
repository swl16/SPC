#include<iostream>
#include<iomanip>
#include<vector>
#include<string>
#include<fstream>
#include <sstream>
#include <ctime>

#include"User.hpp"
#include"booking.h"

using namespace std;

int generateBookingID(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) return 1001; // Default start ID

    int lastID = 1000;
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        int currentID;
        if (ss >> currentID) {
            lastID = currentID;
        }
    }
    file.close();
    return lastID + 1;
}

void bookClass(Member member) {
	ifstream classFile("classes.txt");
    if (!classFile.is_open()) {
        cerr << "Error opening classes.txt!" << endl;
        return;
    }

    vector<Class> availableClasses;
    Class c;

    while(classFile >> c.classID)
    cout << "================================================" << endl;
    cout << "                  CLASS BOOKING                 " << endl;
    cout << "================================================" << endl << endl;
}