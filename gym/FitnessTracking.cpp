#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <limits>
#include <vector>
#include "User.hpp"
#include "FitnessTracking.hpp"

using namespace std;

const string DATA_FILE = "fitness_data.txt";

void ensureFileExists(const string& filename) {
    ifstream checkFile(filename);
    if (!checkFile.is_open()) {
        ofstream createFile(filename); 
        createFile.close();
    }
    else {
        checkFile.close(); 
    }
}

void loadFitnessData(vector<Member>& members, const string& filename) {
    ensureFileExists(filename);

    members.clear();
    ifstream inFile(filename);
    if (!inFile.is_open()) return;

    string line;
    while (getline(inFile, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back(); 
        }
        if (line.empty()) continue;

        stringstream ss(line);
        Member m;

        string weightStr, heightStr, bmiStr, durationStr, targetStr, caloriesStr;

        getline(ss, m.name, '|');
        getline(ss, weightStr, '|');
        getline(ss, heightStr, '|');
        getline(ss, bmiStr, '|');
        getline(ss, durationStr, '|');
        getline(ss, targetStr, '|');
        getline(ss, caloriesStr, '|');

        if (m.name.empty()) continue;

        try {
            m.fitness.weight = weightStr.empty() ? 0.0 : stod(weightStr);
            m.fitness.height = heightStr.empty() ? 0.0 : stod(heightStr);
            m.fitness.bmi = bmiStr.empty() ? 0.0 : stod(bmiStr);
            m.fitness.workoutDuration = durationStr.empty() ? 0 : stoi(durationStr);
            m.fitness.targetWorkoutMins = targetStr.empty() ? 0 : stoi(targetStr);
            m.fitness.caloriesBurned = caloriesStr.empty() ? 0.0 : stod(caloriesStr);
        }
        catch (...) {
            continue;
        }

        members.push_back(m);
    }
    inFile.close();
}

void saveFitnessData(const Member& member, const string& filename) {
    ofstream outFile(filename, ios::app);
    if (!outFile.is_open()) {
        cout << "[ERROR] Could not open file for appending.\n";
        return;
    }

    outFile << member.name << "|"
        << member.fitness.weight << "|"
        << member.fitness.height << "|"
        << member.fitness.bmi << "|"
        << member.fitness.workoutDuration << "|"
        << member.fitness.targetWorkoutMins << "|"
        << member.fitness.caloriesBurned << "\n";

    outFile.close();
}

void saveMember(const Member& currentUser, const string& filename) {
    vector<Member> members;
    loadFitnessData(members, filename);

    bool found = false;
    for (auto& m : members) {
        if (m.name == currentUser.name) {
            m = currentUser;
            found = true;
            break;
        }
    }

    if (!found) {
        saveFitnessData(currentUser, filename);
    }
    else {
        ofstream outFile(filename, ios::trunc);
        if (!outFile.is_open()) return;

        for (const auto& m : members) {
            outFile << m.name << "|"
                << m.fitness.weight << "|"
                << m.fitness.height << "|"
                << m.fitness.bmi << "|"
                << m.fitness.workoutDuration << "|"
                << m.fitness.targetWorkoutMins << "|"
                << m.fitness.caloriesBurned << "\n";
        }
        outFile.close();
    }
}

void loadUserData(Member& currentUser, const string& filename) {
    vector<Member> members;
    loadFitnessData(members, filename);

    bool found = false;
    for (const auto& m : members) {
        if (m.name == currentUser.name) {
            currentUser.fitness = m.fitness; 
            found = true;
            break;
        }
    }

    if (!found) {
        saveFitnessData(currentUser, filename);
    }
}

double calculateBMI(double weight, double height) {
    if (height <= 0 || weight <= 0)
        return 0;

    return weight / (height * height);
}

void setFitnessGoal(Member& members) {
    cout << "\n----- SET WEEKLY WORKOUT GOAL -----\n";
    cout << "Current Goal: " << members.fitness.targetWorkoutMins << "\n";

    int targetMins;

    while (true) {
        cout << "Enter weekly workout goal (minutes/week) (or '-1' to cancel): ";

        if (cin >> targetMins && targetMins >= -1) {
            break;
        }

        cout << "Invalid input. Please enter a positive number or -1 to cancel.\n";

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    if (targetMins == -1) {
        cout << "[CANCELLED] Returning to previous menu...\n";
        return;
    }

    members.fitness.targetWorkoutMins = targetMins;
    cout << "\n[SUCCESS] Weekly target set to : " << members.fitness.targetWorkoutMins << " mins/week.\n";
}

void updateFitnessMetrics(Member& members) {
    double newWeight, newHeight;

    cout << "\n----- UPDATE WEIGHT & HEIGHT -----\n";

    while (true) {
        cout << "Enter weight (kg) (or '0' to return to menu): ";

        if (cin >> newWeight && (newWeight == 0 || (newWeight > 0 && newWeight < 300))) {
            break;
        }

        cout << "Invalid weight. Weight cannot be less than 0 or more than 300kg.\n";

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    if (newWeight == 0) {
        cout << "[CANCELLED] Returning to previous menu...\n";
        return;
    }

    while (true) {
        cout << "Enter height (m) (or '0' to return to menu): ";

        if (cin >> newHeight && (newHeight == 0 || (newHeight > 0 && newHeight < 2.8))) {
            break;
        }

        cout << "Invalid height. Height cannot be less than 0 or more than 2.8m.\n";

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    if (newHeight == 0) {
        cout << "[CANCELLED] Returning to previous menu...\n";
        return;
    }

    members.fitness.weight = newWeight;
    members.fitness.height = newHeight;
    members.fitness.bmi = calculateBMI(newWeight, newHeight);

    cout << fixed << setprecision(2);
    cout << "\nBMI : " << members.fitness.bmi << endl;

    if (members.fitness.bmi < 18.5) {
        cout << "Category: Underweight\n";
    }
    else if (members.fitness.bmi < 25.0) {
        cout << "Category: Normal\n";
    }
    else if (members.fitness.bmi < 30.0) {
        cout << "Category: Overweight\n";
    }
    else {
        cout << "Category: Obese\n";
    }
}

void logWorkoutSession(Member& members) {
    char choice;
    int minutes;
    int intensity;
    cout << "\n----- LOG WORKOUT SESSION -----\n";

    do {
        cout << "Enter workout duration (minutes) (or '0' to return to menu): ";

        while (!(cin >> minutes) || minutes < 0) {
            cout << "Invalid duration! Please enter a non-negative whole number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        if (minutes == 0) {
            cout << "[CANCELLED] Returning to previous menu...\n";
            return;
        }

        cout << "Select Workout Intensity Level:\n";
        cout << "1. Light\n";
        cout << "2. Moderate\n";
        cout << "3. High Intensity\n";

        cout << "Choice (1-3): ";
        while (!(cin >> intensity) || intensity < 1 || intensity > 3) {
            cout << "Invalid selection. Choice must be 1, 2 or 3: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        double burnRate = 8.0;
        switch (intensity) {
        case 1: burnRate = 5.0; break;
        case 2: burnRate = 8.0; break;
        case 3: burnRate = 12.0; break;
        }

        double estCalories = minutes * burnRate;

        members.fitness.workoutDuration += minutes;
        members.fitness.caloriesBurned += estCalories;

        cout << fixed << setprecision(1);
        cout << "-> Logged: " << minutes << " minutes | Estimated Calories Burned: " << estCalories << " kcal\n";

        cout << "\nDo you want to log another workout session for today? (Y/N): ";
        cin >> choice;

    } while (choice == 'Y' || choice == 'y');

    cout << "\n[SUCCESS] Session logged! Total workout time: " << members.fitness.workoutDuration << " minutes.\n";
    if (members.fitness.targetWorkoutMins > 0 && members.fitness.workoutDuration >= members.fitness.targetWorkoutMins) {
        cout << "Great job! You reached your weekly workout target!\n";
    }
}

void resetFitnessMetrics(Member& member) {
    char confirm;
    cout << "\nAre you sure you want to reset workout duration and calories burned to 0? (Y/N): ";
    cin >> confirm;

    if (confirm == 'Y' || confirm == 'y') {
        member.fitness.workoutDuration = 0;
        member.fitness.caloriesBurned = 0.0;
        cout << "[SUCCESS] Accumulated workout logs have been reset to zero.\n";
    }
    else {
        cout << "[CANCELLED] Reset action aborted. Returning to previous menu...\n";
    }
}

void generateFitnessReport(const Member& members) {
    cout << "\n===================================\n";
    cout << "      FITNESS PROGRESS REPORT\n";
    cout << "===================================\n";
    cout << left << setw(20) << "Name" << ": " << members.name << "\n";
    cout << "-----------------------------------\n";

    cout << fixed << setprecision(2);
    cout << left << setw(20) << "Weight" << ": " << members.fitness.weight << " kg\n";
    cout << left << setw(20) << "Height" << ": " << members.fitness.height << " m\n";
    cout << left << setw(20) << "BMI" << ": " << members.fitness.bmi << "\n";
    cout << "-----------------------------------\n";
    cout << left << setw(20) << "Logged Workout Time" << ": " << members.fitness.workoutDuration << " mins\n";

    if (members.fitness.targetWorkoutMins > 0) {
        cout << left << setw(20) << "Weekly Workout Goal" << ": " << members.fitness.targetWorkoutMins << " mins/week\n";
        cout << left << setw(20) << "Goal Status" << ": ";
        if (members.fitness.workoutDuration >= members.fitness.targetWorkoutMins) {
            cout << "[ACHIEVED] Goal reached!\n";
        }
        else {
            int remaining = members.fitness.targetWorkoutMins - members.fitness.workoutDuration;
            cout << "[IN PROGRESS] " << remaining << " mins remaining\n";
        }
    }
    else {
        cout << left << setw(20) << "Weekly Workout Goal" << ": Not Set\n";
        cout << left << setw(20) << "Goal Status" << ": N/A\n";
    }

    cout << left << setw(20) << "Total Calories" << ": " << members.fitness.caloriesBurned << " kcal\n";
    cout << "-----------------------------------\n";
}

void fitnessMenu(Member& currentUser) {
    ensureFileExists(DATA_FILE);
    loadUserData(currentUser, DATA_FILE);

    int choice;

    do {
        cout << "\n===========================================\n";
        cout << "           FITNESS TRACKER MENU\n";
        cout << "===========================================\n";
        cout << "1. Set Weekly Workout Goal\n";
        cout << "2. Update Body Metrics (Recalculate BMI)\n";
        cout << "3. Log Workout Session\n";
        cout << "4. Reset Fitness Metrics\n";
        cout << "5. Generate Fitness Progress Report\n";
        cout << "0. Back to User Menu\n";
        cout << "===========================================\n";
        cout << "Enter your choice (1-5) or '0' to return: ";

        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a valid choice.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
        case 1:
            setFitnessGoal(currentUser);
            saveMember(currentUser, DATA_FILE);
            break;
        case 2:
            updateFitnessMetrics(currentUser);
            saveMember(currentUser, DATA_FILE);
            break;
        case 3:
            logWorkoutSession(currentUser);
            saveMember(currentUser, DATA_FILE);
            break;
        case 4:
            resetFitnessMetrics(currentUser);
            saveMember(currentUser, DATA_FILE);
            break;
        case 5:
            generateFitnessReport(currentUser);
            break;
        case 0:
            cout << "\nExiting Fitness Module.\n";
            break;
        default:
            cout << "Invalid choice. Please select 0-5.\n";
        }

    } while (choice != 0);
}