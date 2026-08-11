#include <iostream>
#include "User.hpp"
using namespace std;

struct FitnessRecord {
	double weight;
	double height;
	double bmi = 0.0;
	int workoutDuration = 0;
	int targetWorkoutMins = 0;
	double caloriesBurned = 0.0;
};

double calculateBMI(double weight, double height);
void setFitnessGoal(Member& members);
void updateFitnessMetrics(Member& members);
void logWorkoutSession(Member& members);
void resetFitnessMetrics(Member& members);
void generateFitnessReport(const Member& members);
void fitnessMenu(Member& members);