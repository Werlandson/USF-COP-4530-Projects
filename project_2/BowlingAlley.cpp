/*
 * Name: Waylon Erlandson, Duong Minh, Duc Long Nguyen
 * Description: Implements BowlingAlley service tasks as typing a random 5-character sequence.
 */

#include "BowlingAlley.h"

#include <iostream>
#include <limits>
#include <random>
#include <string>

using namespace std;

BowlingAlley::BowlingAlley()
    : Company() {}

void BowlingAlley::printWelcome()
{
    cout << "Welcome to the Bowling Alley simulator!" << endl
         << endl
         << "Keep the lanes moving. Let a group wait more than 5 seconds and you're out." << endl
         << endl;
}

// Generates a random 5-character sequence for typing task.
static string randomSequence5(mt19937 &gen)
{
    // Character set includes uppercase letters and digits.
    static const char charset[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "0123456789";
    uniform_int_distribution<> pick(0, static_cast<int>(sizeof(charset) - 2)); // exclude null terminator
    // Build the 5-character string.
    string s;
    s.reserve(5);
    for (int i = 0; i < 5; i++)
        s.push_back(charset[pick(gen)]);
    return s;
}

void BowlingAlley::performServiceTask(const Group &group, mt19937 &gen)
{
    cout << "Type the 5-character sequence to serve the group (case-sensitive):" << endl;

    // Perform the task for each difficulty level.
    for (int i = 0; i < group.getDifficulty(); i++)
    {
        if (!isGameRunning())
            return; // Exit if game ended.

        const string target = randomSequence5(gen); // Generate random sequence.
        bool correct = false;
        // Loop until correct input or game ends.
        while (!correct)
        {
            if (!isGameRunning())
                return; // Exit if game ended.

            cout << i + 1 << ". Type: " << target << " -> ";
            string input;
            if (!(cin >> input))
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Please enter a valid 5-character response." << endl;
                continue; // Retry input.
            }

            if (!isGameRunning())
                return; // Exit if game ended.

            if (input != target)
            {
                cout << "Incorrect. Try again" << endl;
            }
            else
            {
                correct = true;
                cout << "Correct!" << endl;
            }
        }
    }
}

