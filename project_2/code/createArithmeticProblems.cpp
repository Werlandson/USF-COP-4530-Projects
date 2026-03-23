/*
 * Names: Waylon Erlandson, Minh Duong, Duc Long Nguyen
 * Description: Creates a vector of arithmetic problems for each pair of numbers 1–12 using +, -, *, and /.
 * Division problems are only added when the result is a whole number.
 */
#include "ArithmeticProblem.h"
#include <iostream>

using namespace std;

vector<ArithmeticProblem> createArithmeticProblems()
{
    vector<ArithmeticProblem> arithmeticProblems;
    char operators[] = {'+', '*', '-', '/'}; // list of operators

    for (int i = 1; i < 13; i++)
    {
        for (int j = 1; j < 13; j++)
        {
            for (char oper : operators)
            {
                switch (oper)
                {
                case '/':
                {
                    // only include division if first number is divisible by second
                    if (i % j == 0)
                    {
                        ArithmeticProblem problem = ArithmeticProblem(i, j, oper, i / j);
                        arithmeticProblems.push_back(problem);
                    }
                    break;
                }
                case '*':
                {
                    // multiplication problem
                    ArithmeticProblem problem = ArithmeticProblem(i, j, oper, i * j);
                    arithmeticProblems.push_back(problem);
                    break;
                }
                case '+':
                {
                    // addition problem
                    ArithmeticProblem problem = ArithmeticProblem(i, j, oper, i + j);
                    arithmeticProblems.push_back(problem);
                    break;
                }
                case '-':
                {
                    // subtraction problem
                    ArithmeticProblem problem = ArithmeticProblem(i, j, oper, i - j);
                    arithmeticProblems.push_back(problem);
                    break;
                }
                }
            }
        }
    }

    return arithmeticProblems; // return full list of problems
}
