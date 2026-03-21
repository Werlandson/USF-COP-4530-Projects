/*
 * Name: Waylon Erlandson, Duong Minh, Duc Long Nguyen
 * Description: Restaurant inherits the shared Company simulation behavior and differentiates
 * itself through arithmetic-problem serving tasks.
 * */
#include "Restaurant.h"
#include <iostream>
#include <limits>

using namespace std;

Restaurant::Restaurant()
    : Company(),
      arithmeticProblems(createArithmeticProblems()) {} // initialize math problems

void Restaurant::printWelcome()
{
    cout << "Welcome to the Restaurant simulator!" << endl
         << endl
         << "This is a serious estbalishment, and we don't mess around. Make our customers"
         << endl
         << "wait more than 5 seconds, and you're out." << endl
         << endl;
}

void Restaurant::performServiceTask(const Group &group, mt19937 &gen)
{
    // randomly choose a problem from the list
    uniform_int_distribution<> arithmeticProblemRange(0, arithmeticProblems.size() - 1);

    cout << "Answer problems to serve them. To answer, type the number and press enter:" << endl;

    for (int i = 0; i < group.getDifficulty(); i++)
    {
        if (!isGameRunning())
            return;

        bool answerCorrect = false;
        int answer;

        // get a random arithmetic problem
        ArithmeticProblem arithmeticProblem = arithmeticProblems[arithmeticProblemRange(gen)];

        while (!answerCorrect)
        {
            if (!isGameRunning())
                return;

            // display problem to user
            cout << i + 1 << ". " << arithmeticProblem.operand1 << " " << arithmeticProblem.operator1 << " " << arithmeticProblem.operand2 << " = ";

            if (!(cin >> answer))
            {
                // handle invalid input
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Please enter a valid integer answer." << endl;
                continue;
            }

            if (!isGameRunning())
                return;

            // check if answer is correct
            if (answer != arithmeticProblem.answer)
            {
                cout << "Your answer was incorrect. Try again" << endl;
            }
            else
            {
                answerCorrect = true;
                cout << "Correct!" << endl;
            }
        }
    }
}
