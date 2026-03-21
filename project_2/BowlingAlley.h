/*
 * Name: Waylon Erlandson, Duong Minh, Duc Long Nguyen
 * Description: BowlingAlley class inherits Company and differentiates itself through
 * typing-based serving tasks.
 */
#pragma once

#include "Company.h"

using namespace std;

class BowlingAlley : public Company
{
public:
    // Constructor for BowlingAlley.
    BowlingAlley();

private:
    void printWelcome() override;
    void performServiceTask(const Group &group, mt19937 &gen) override;
};

