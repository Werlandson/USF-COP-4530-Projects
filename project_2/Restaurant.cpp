#include "Restaurant.h"
#include <iostream>
#include <limits>

using namespace std;

Restaurant::Restaurant()
    : Company(),
      arithmeticProblems(createArithmeticProblems()) {}

void Restaurant::printWelcome()
{
    cout << "Welcome to the Restaurant simulator!" << endl
         << endl
         << "This is a serious estbalishment, and we don't mess around. Make our customers"
         << endl
         << "wait more than 5 seconds, and you're out." << endl
         << endl;
}

void Restaurant::performServiceTask(const Group &group, std::mt19937 &gen)
{
    uniform_int_distribution<> arithmeticProblemRange(0, static_cast<int>(arithmeticProblems.size()) - 1);
    cout << "Answer problems to serve them. To answer, type the number and press enter:" << endl;

    for (int i = 0; i < group.getDifficulty(); i++)
    {
        if (!isGameRunning())
            return;

        bool answerCorrect = false;
        int answer;
        ArithmeticProblem arithmeticProblem = arithmeticProblems[arithmeticProblemRange(gen)];

        while (!answerCorrect)
        {
            if (!isGameRunning())
                return;

            cout << i + 1 << ". " << arithmeticProblem.operand1 << " " << arithmeticProblem.operator1 << " " << arithmeticProblem.operand2 << " = ";
            if (!(cin >> answer))
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Please enter a valid integer answer." << endl;
                continue;
            }

            if (!isGameRunning())
                return;

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