/*
 * Name: Waylon Erlandson, Duong Minh, Duc Long Nguyen
 * Description: Restaurant inherits the shared Company simulation behavior and differentiates
 * itself through arithmetic-problem serving tasks.
 * */
#include "Restaurant.h"
#include <iostream>
#include <limits>
#include <sys/select.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

using namespace std;

// Checks whether stdin has data available to read without blocking.
// Used so the serving thread can exit promptly when gameIsRunning becomes false.
static bool stdinReady(int timeoutMs)
{
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);

    timeval tv;
    tv.tv_sec = timeoutMs / 1000;
    tv.tv_usec = (timeoutMs % 1000) * 1000;

    const int result = select(STDIN_FILENO + 1, &readfds, nullptr, nullptr, &tv);
    return (result > 0) && FD_ISSET(STDIN_FILENO, &readfds);
}

// Temporarily set stdin to non-blocking mode so the serve thread can exit promptly.
// This prevents blocking reads from preventing the program from terminating.
struct NonBlockingStdin
{
    int oldFlags;
    NonBlockingStdin()
    {
        // Save current stdin flags, then enable non-blocking mode.
        // This allows the serve thread to periodically check gameIsRunning
        // instead of being stuck inside a blocking std::cin read.
        oldFlags = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, oldFlags | O_NONBLOCK);
    }
    ~NonBlockingStdin()
    {
        // Restore stdin back to its previous blocking/non-blocking state.
        fcntl(STDIN_FILENO, F_SETFL, oldFlags);
    }
};

// Tries to extract the next whitespace-delimited token from `buffer`.
// Returns true if a token was extracted into `outToken`.
static bool tryPopToken(string &buffer, string &outToken)
{
    // Extract the next whitespace-delimited token from `buffer`.
    // If there isn't a full token yet, we return false and keep waiting.
    const size_t pos = buffer.find_first_of(" \t\r\n");
    if (pos == string::npos)
        return false;

    if (pos > 0)
        outToken = buffer.substr(0, pos);
    else
        outToken.clear();

    // Erase token + one delimiter character (the delimiter itself counts as one char here).
    buffer.erase(0, pos + 1);

    // If we extracted an empty token due to consecutive whitespace, keep popping by returning false.
    if (outToken.empty())
        return false;

    return true;
}

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

// Performs a number of service tasks for the First-In group of tables according to the group difficulty
void Restaurant::performServiceTask(const Group &group, mt19937 &gen)
{
    // randomly choose a problem from the list
    uniform_int_distribution<> arithmeticProblemRange(0, arithmeticProblems.size() - 1);

    cout << "Answer problems to serve them. To answer, type the number and press enter:" << endl;

    // Service task performed n times where n is the group difficulty.
    NonBlockingStdin nbStdin; // allow timely shutdown while waiting for user input
    string inputBuffer;

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
            cout << i + 1 << ". " << arithmeticProblem.operand1 << " " << arithmeticProblem.operator1 << " " << arithmeticProblem.operand2 << " = " << flush;

            // Wait for a complete token (whitespace-delimited) using non-blocking stdin.
            // We do *not* use blocking std::cin here because the serve thread must be able
            // to exit promptly when the game ends.
            while (true)
            {
                if (!isGameRunning())
                    return;

                // Read whatever is currently available from stdin (non-blocking).
                // If there is no data ready, read() returns -1 and sets errno=EAGAIN,
                // so we break and wait briefly (rather than blocking forever).
                char tmp[256];
                while (true)
                {
                    const ssize_t nread = read(STDIN_FILENO, tmp, sizeof(tmp));
                    if (nread > 0)
                        inputBuffer.append(tmp, static_cast<size_t>(nread));
                    else
                        break;
                }

                string token;
                if (tryPopToken(inputBuffer, token))
                {
                    // Try to parse integer token.
                    try
                    {
                        answer = stoi(token);
                        break; // got valid integer token
                    }
                    catch (...)
                    {
                        cout << "Please enter a valid integer answer." << endl;
                        // Keep waiting for a valid integer token for the same question.
                        continue;
                    }
                }

                // No complete token yet; sleep briefly to avoid busy-waiting.
                this_thread::sleep_for(chrono::milliseconds(50));
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
