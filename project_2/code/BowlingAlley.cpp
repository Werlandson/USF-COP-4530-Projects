/*
 * Name: Waylon Erlandson, Duong Minh, Duc Long Nguyen
 * Description: Implements BowlingAlley service tasks as typing a random 5-character sequence.
 */

#include "BowlingAlley.h"

#include <iostream>
#include <limits>
#include <random>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <thread>

using namespace std;

// Temporarily set stdin to non-blocking mode so the serve thread can exit promptly.
// This prevents blocking reads from preventing the program from terminating.
struct NonBlockingStdin
{
    int oldFlags;
    NonBlockingStdin()
    {
        // Save current stdin flags, then enable non-blocking mode.
        // This allows the serving thread to terminate promptly when gameIsRunning becomes false.
        oldFlags = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, oldFlags | O_NONBLOCK);
    }
    ~NonBlockingStdin()
    {
        // Restore stdin back to its previous state.
        fcntl(STDIN_FILENO, F_SETFL, oldFlags);
    }
};

// Tries to extract the next whitespace-delimited token from `buffer`.
// Returns true if a token was extracted into `outToken`.
static bool tryPopToken(string &buffer, string &outToken)
{
    // Extract the next whitespace-delimited token from `buffer`.
    // Returns false if there's no delimiter yet (i.e., no complete token).
    const size_t pos = buffer.find_first_of(" \t\r\n");
    if (pos == string::npos)
        return false;

    if (pos > 0)
        outToken = buffer.substr(0, pos);
    else
        outToken.clear();

    buffer.erase(0, pos + 1);

    if (outToken.empty())
        return false;

    return true;
}

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

// Prompts the user with a number of sequence tasks for serving the group according to the group difficulty
void BowlingAlley::performServiceTask(const Group &group, mt19937 &gen)
{
    cout << "Type the 5-character sequence to serve the group (case-sensitive):" << endl;

    // Non-blocking stdin so we can stop promptly when the game ends.
    NonBlockingStdin nbStdin;
    string inputBuffer;

    // Perform the task n times where n is the group difficulty.
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

            cout << i + 1 << ". Type: " << target << " -> " << flush;

            while (!correct)
            {
                if (!isGameRunning())
                    return;

                // Read available input without blocking.
                // Because stdin is non-blocking, read() will return immediately:
                //   - nread > 0 => append bytes to inputBuffer
                //   - otherwise  => no more bytes available right now (e.g., EAGAIN)
                char tmp[256];
                while (true)
                {
                    const ssize_t nread = read(STDIN_FILENO, tmp, sizeof(tmp));
                    if (nread > 0)
                        inputBuffer.append(tmp, static_cast<size_t>(nread));
                    else
                        break; // EAGAIN => nothing available; other errors => stop reading now
                }

                string token;
                if (tryPopToken(inputBuffer, token))
                {
                    if (token.size() != 5)
                    {
                        cout << "Incorrect. Try again" << endl;
                        break; // ask again for the same i
                    }

                    if (token != target)
                    {
                        cout << "Incorrect. Try again" << endl;
                        break; // ask again for the same i
                    }

                    correct = true;
                    cout << "Correct!" << endl;
                    break;
                }

                // No complete token yet; wait briefly to avoid busy-waiting.
                this_thread::sleep_for(chrono::milliseconds(50));
            }
        }
    }
}
