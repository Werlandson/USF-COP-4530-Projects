/*
 * Name: Waylon Erlandson
 * Description: Header file for ArithmeticProblem class. Each arithmetic problem has two operands, an operator (+, -, *, or /),
 * and a corresponding answer.
 */
#pragma once
#include <vector>

// A class for storing arithmetic problems and answers and copying those arithmetic problems
class ArithmeticProblem
{
public:
    ArithmeticProblem(int operand1, int operand2, char operator1, int answer)
        : operand1(operand1), operand2(operand2), operator1(operator1), answer(answer) {}
    ArithmeticProblem(const ArithmeticProblem &problem)
        : operand1(problem.operand1), operand2(problem.operand2), operator1(problem.operator1), answer(problem.answer) {}

    int operand1;
    int operand2;
    char operator1;
    int answer;
};

std::vector<ArithmeticProblem> createArithmeticProblems();
