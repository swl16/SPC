#include <iostream>
#include <string>
#include "User.hpp"

double calculateBMI(double weight, double height) {
	return weight / (height * height);
}

void setFitnessGoal(Member& members) {
	cout << "\n=====SET WEEKLY WORKOUT GOAL=====\n";

	int targetMins;

	cout << "Enter weekly workout goal (? minutes/week) : ";
	while (!(cin >> targetMins) || targetMins <= 0) {
		cout << "Invalid input. Please enter a positive whole number : ";
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}

	members.fitness.targetWorkoutMins = targetMins;

	cout << "\n[SUCCESS] Weekly target set to : " << members.fitness.targetWorkoutMins << " mins/week.\n";
}

void updateFitnessMetrics(Member& members) {
	double newWeight, newHeight;
	cout << "\n===== UPDATE WEIGHT & HEIGHT =====\n";
	cout << "Enter current weight (kg) : ";
	while (!(cin >> newWeight) || newWeight <= 0) {
		cout << "Invalid weight. Please enter a positive number : ";
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}

	cout << "Enter current height (m) : ";
	while (!(cin >> newHeight) || newHeight <= 0) {
		cout << "Invalid height. Please enter a positive number : ";
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}

	members.fitness.weight = newWeight;
	members.fitness.height = newHeight;
	members.fitness.bmi = calculateBMI(newWeight, newHeight);

	cout << "\n[SUCCESS] Physical metrics updated.\n";
	cout << fixed << setprecision(2);
	cout << "Calculated BMI : " << members.fitness.bmi << " (";

	if (members.fitness.bmi < 18.5) {
		cout << "Underweight)\n";
	} else if (members.fitness.bmi < 25.0) {
		cout << "Normal weight)\n";
	} else if (members.fitness.bmi < 30.0) {
		cout << "Overweight)\n";
	} else {
		cout << "Obese)\n";
	}
}

void logWorkoutSession(Member& members) {
	char choice;
	cout << "\n===== LOG WORKOUT SESSION =====\n";

	do {
		int minutes;
		cout << "Enter workout duration (minutes) : ";
		while (!(cin >> minutes) || minutes <= 0) {
			cout << "Invalid duration! Please enter a positive whole number: ";
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
		members.fitness.workoutDuration += minutes;

		cout << "Select Workout Intensity Level :\n";
		cout << "1. Light (Cardio / Yoga [~5 kcal/min]\n";
		cout << "2. Moderate (Weightlifting / Cycling) [~8 kcal/min]\n";
		cout << "3. High Intensity (HIIT / Running) [~12 kcal/min]\n";

		int intensity;
		cout << "Choice (1-3) : ";
		while (!(cin >> intensity) || intensity < 1 || intensity > 3) {
			cout << "Invalid selection. Choice must be 1, 2 or 3 : ";
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}

		double burnRate = 8.0;
		switch (intensity) {
		case 1: burnRate = 5.0;
			break;
		case 2: burnRate = 8.0; 
			break;
		case 3: burnRate = 12.0; 
			break;
		}

		double estCalories = minutes * burnRate;

		
		members.fitness.caloriesBurned += estCalories;

		cout << fixed << setprecison(1);
		cout << "-> Logged : " << minutes << " minutes | Estimate Calories Burned : " << estCalories << " kcal\n";
		cout << "\nDo you want to log another workout session for today? (Y/N) : ";
		cin >> choice;
	} while (choice == 'Y' || choice == 'y');

	cout << "\n[SUCCESS] Session logged! Total workout time : " << members.fitness.workoutDuration << "minutes.\n";
	if (members.fitness.targetWorkoutMins > 0 && members.fitness.workoutDuration >= members.fitness.targetWorkoutMins) {
		cout << "Great job! You reached your weekly workout target!\n";
	}
}

void generateFitnessReport(const Member& members) {
	cout << "\n==============================\n";
	cout << "     FITNESS PROGRESS REPORT\n";
	cout << "\n==============================\n"; 
	cout << "Name  : " << members.name << "\n";

}









