/*
 * Name: Waylon Erlandson, Duong Minh, Duc Long Nguyen
 * Description: Header file for Restaurant class
 * */
#pragma once

#include <vector>

#include "ArithmeticProblem.h"
#include "Company.h"

using namespace std;

class Restaurant : public Company
{
public:
    // Constructor for Restaurant.
    Restaurant();

private:
    // Prints the welcome message.
    void printWelcome() override;
    // Performs the service task by presenting arithmetic problems to solve.
    void performServiceTask(const Group &group, mt19937 &gen) override;

    vector<ArithmeticProblem> arithmeticProblems;
};
