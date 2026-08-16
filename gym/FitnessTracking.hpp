#include <iostream>
#include <iomanip>


#include "User.hpp"
using namespace std;

struct Member;


double calculateBMI(double weight, double height);
void setFitnessGoal(Member& members);
void updateFitnessMetrics(Member& members);
void logWorkoutSession(Member& members);
void resetFitnessMetrics(Member& members);
void generateFitnessReport(const Member& members);
void fitnessMenu(Member& members);