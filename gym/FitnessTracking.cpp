#include "User.hpp"
#include "FitnessTracking.hpp"



double calculateBMI(double weight, double height) {

	if (height <= 0 || weight <= 0)
		return 0;

	return weight / (height * height);
}

void setFitnessGoal(Member& members) {
	cout << "\n-----SET WEEKLY WORKOUT GOAL-----\n";
	cout << "Current Goal: " << members.fitness.targetWorkoutMins << "\n";

	int targetMins;

	while (true) {

		cout << "Enter weekly workout goal (? minutes/week) (or '0' to return to menu) : ";

		if (cin >> targetMins && targetMins >= 0) {
			break;
		}

		cout << "Invalid input. Please enter a positive number or '0' to return to menu.\n";

		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}

	if (targetMins == 0) {
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

		cout << "Enter weight (kg) (or '0' to return to menu) : ";

		if (cin >> newWeight && (newWeight == 0 || (newWeight > 0 && newWeight < 300))) {
			break;
		}

		cout << "Invalid weight. Weight cannot be less than 0 or more than 300kg\n";

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

		cout << "Invalid height. Height cannot be less than 0 or more than 2.8m\n";

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
		cout << "Category: Underweight)\n";
	} else if (members.fitness.bmi < 25.0) {
		cout << "Category: Normal Weight)\n";
	} else if (members.fitness.bmi < 30.0) {
		cout << "Category: Overweight)\n";
	} else {
		cout << "Category: Obese)\n";
	}
}

void logWorkoutSession(Member& members) {

	char choice;
	int minutes;
	int intensity;
	cout << "\n----- LOG WORKOUT SESSION -----\n";

	do {
		cout << "Enter workout duration (minutes) (or '0' to return to menu) : ";

		while (!(cin >> minutes) || minutes <= 0) {
			cout << "Invalid duration! Please enter a positive whole number (or '0' to return to menu).";
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}

		if (minutes == 0) {
			cout << "[CANCELLED] Returning to previous menu...\n";
			return;
		}

		members.fitness.workoutDuration += minutes;

		cout << "Select Workout Intensity Level :\n";
		cout << "1. Light (Cardio / Yoga [~5 kcal/min]\n";
		cout << "2. Moderate (Weightlifting / Cycling) [~8 kcal/min]\n";
		cout << "3. High Intensity (HIIT / Running) [~12 kcal/min]\n";


		cout << "Choice (1-3) : ";
		while (!(cin >> intensity) || intensity < 1 || intensity > 3) {
			cout << "Invalid selection. Choice must be 1, 2 or 3 ";
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

		cout << fixed << setprecision(1);
		cout << "-> Logged : " << minutes << " minutes | Estimate Calories Burned : " << estCalories << " kcal\n";

		cout << "\nDo you want to log another workout session for today? (Y/N) : ";
		cin >> choice;

	} while (choice == 'Y' || choice == 'y');

	cout << "\n[SUCCESS] Session logged! Total workout time : " << members.fitness.workoutDuration << "minutes.\n";
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
    } else {
        cout << "[CANCELLED] Reset action aborted. Returning to previous menu...\n";
    }
}

void generateFitnessReport(const Member& members) {
	cout << "===================================\n";
	cout << "     FITNESS PROGRESS REPORT\n";
	cout << "===================================\n";
	cout << "Name  : " << members.name << "\n";
	cout << "-----------------------------------\n";

	cout << fixed << setprecision(2);
	cout << left << setw(20) << "Weight :"  << members.fitness.weight << " kg\n";
	cout << left << setw(20) << "Height :"  << members.fitness.height << " m\n";
	cout << left << setw(20) << "BMI    :"  << members.fitness.bmi << "\n";
	cout << "----------------------------------------\n";
	cout << left << setw(20) << "Logged Workout Time" << ": " << members.fitness.workoutDuration << " mins\n";

	if (members.fitness.targetWorkoutMins > 0) {
		cout << left << setw(30) << "Weekly Workout Goal" << ": " << members.fitness.targetWorkoutMins << " minutes/week\n";

		cout << left << setw(20) << "Goal Status : ";
		if (members.fitness.workoutDuration >= members.fitness.targetWorkoutMins) {
			cout << "[ACHIEVED] Goal reached!\n";
		}
		else {
			int remaining = members.fitness.targetWorkoutMins - members.fitness.workoutDuration;
			cout << "[IN PROGRESS] " << remaining << " minutes remaining\n";
		}
	}
	else {
		cout << left << setw(30) << "Weekly Workout Goal" << ": Not Set\n";
		cout << left << setw(20) << "Goal Status" << ": N/A\n";
	}

	cout << left << setw(20) << "Total Calories : " << members.fitness.caloriesBurned << " kcal\n";
	cout << "-----------------------------------\n";
}

void fitnessMenu(Member& members) {
	int choice;

	do {
		cout << "===========================================\n";
		cout << "          FITNESS TRACKER MENU\n";
		cout << "===========================================\n";
		cout << "1. Set Weekly Workout Goal\n";
		cout << "2. Update Body Metrics (Recalculate BMI)\n";
		cout << "3. Log Workout Session\n";
		cout << "4. Reset Fitness Metrics\n";
		cout << "5. Generate Fitness Progress Report\n";
		cout << "0. Back to User Menu\n";
		cout << "===========================================\n";
		cout << "Enter your choice (1-5) or '0' to return to user menu : ";

		if (!(cin >> choice)) {
			cout << "Invalid input. Please enter a number between 1 and 5 or '0' to return to user menu \n";
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
		}

		switch (choice) {
		case 1:
			setFitnessGoal(members);
			break;
		case 2:
			updateFitnessMetrics(members);
			break;
		case 3:
			logWorkoutSession(members);
			break;
		case 4:
			resetFitnessMetrics(members);
			break;
		case 5:
			generateFitnessReport(members);
			break;
		case 0:
			cout << "\nExiting Fitness Module.\n";
			break;
		default:
			cout << "Invalid choice. Please select 0-5.\n";
		}

	} while (choice != 0);
}
