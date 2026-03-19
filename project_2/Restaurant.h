/*
 * Name: Waylon Erlandson
 * Description: Header file for Restaurant class. Restaurant inherits the shared Company simulation
 * behavior and differentiates itself through arithmetic-problem serving tasks.
 * */
#pragma once

#include <vector>

#include "ArithmeticProblem.h"
#include "Company.h"

class Restaurant : public Company
{
public:
    Restaurant();

private:
    void printWelcome() override;
    void performServiceTask(const Group &group, std::mt19937 &gen) override;

    std::vector<ArithmeticProblem> arithmeticProblems;
};
