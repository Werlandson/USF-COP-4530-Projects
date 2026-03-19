/*
 * Name: Waylon Erlandson
 * Description: Creates a vector of arithmetic problems for each pair of numbers 1-12 for the addition, subtraction, and multiplication operators
 * and each pair of numbers 1-12 where the first number is divisible by the second for the division operator.
 * */
#include "ArithmeticProblem.h"
#include <iostream>

using namespace std;

vector<ArithmeticProblem> createArithmeticProblems()
{
    vector<ArithmeticProblem> arithmeticProblems;
    char operators[] = {'+', '*', '-', '/'};

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
                    // Adds arithmetic problem only if the first operand is divisible by the second.
                    if (i % j == 0)
                    {
                        ArithmeticProblem problem = ArithmeticProblem(i, j, oper, i / j);
                        arithmeticProblems.push_back(problem);
                    }
                    break;
                }
                case '*':
                {
                    ArithmeticProblem problem = ArithmeticProblem(i, j, oper, i * j);
                    arithmeticProblems.push_back(problem);
                    break;
                }
                case '+':
                {
                    ArithmeticProblem problem = ArithmeticProblem(i, j, oper, i + j);
                    arithmeticProblems.push_back(problem);
                    break;
                }
                case '-':
                {
                    ArithmeticProblem problem = ArithmeticProblem(i, j, oper, i - j);
                    arithmeticProblems.push_back(problem);
                    break;
                }
                }
            }
        }
    }

    return arithmeticProblems;
}