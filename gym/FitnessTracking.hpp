#pragma once
#include <iostream>
#include <iomanip>


#include "User.hpp"
using namespace std;

struct Member;

struct FitnessRecord {
	double weight = 0.0;
	double height = 0.0;
	double bmi = 0.0;
	int workoutDuration = 0;
	int targetWorkoutMins = 0;
	double caloriesBurned = 0.0;
};

void ensureFileExists(const string& filename = "FitnessData.txt");
void loadFitnessData(vector <Member>& members, const string& filename = "FitnessData.txt");
void saveFitnessData(const Member& member, const string& filename = "FitnessData.txt");
void saveMember(const Member& currentUser, const string& filename = "FitnessData.txt");
void loadUserData(Member& currentUser, const string& filename = "FitnessData.txt");
double calculateBMI(double weight, double height);
void setFitnessGoal(Member& members);
void updateFitnessMetrics(Member& members);
void logWorkoutSession(Member& members);
void resetFitnessMetrics(Member& members);
void generateFitnessReport(const Member& members);
void fitnessMenu(Member& currentUser);

