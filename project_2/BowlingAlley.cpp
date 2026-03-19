/*
 * Name: Waylon Erlandson
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

static string randomSequence5(mt19937 &gen)
{
    static const char charset[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "0123456789";
    uniform_int_distribution<> pick(0, static_cast<int>(sizeof(charset) - 2)); // exclude null terminator

    string s;
    s.reserve(5);
    for (int i = 0; i < 5; i++)
        s.push_back(charset[pick(gen)]);
    return s;
}

void BowlingAlley::performServiceTask(const Group &group, std::mt19937 &gen)
{
    cout << "Type the 5-character sequence to serve the group (case-sensitive):" << endl;

    for (int i = 0; i < group.getDifficulty(); i++)
    {
        if (!isGameRunning())
            return;

        const string target = randomSequence5(gen);
        bool correct = false;
        while (!correct)
        {
            if (!isGameRunning())
                return;

            cout << i + 1 << ". Type: " << target << " -> ";
            string input;
            if (!(cin >> input))
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Please enter a valid 5-character response." << endl;
                continue;
            }

            if (!isGameRunning())
                return;

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

