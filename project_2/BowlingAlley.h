/*
 * Name: Waylon Erlandson
 * Description: BowlingAlley class inherits Company and differentiates itself through
 * typing-based serving tasks.
 */
#pragma once

#include "Company.h"

class BowlingAlley : public Company
{
public:
    BowlingAlley();

private:
    void printWelcome() override;
    void performServiceTask(const Group &group, std::mt19937 &gen) override;
};

