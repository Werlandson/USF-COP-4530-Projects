/*
 * Names: Waylon Erlandson, Minh Duong, Duc Long Nguyen
 * Description: Main program that lets the user choose between the restaurant or bowling simulation.
 */
#include "Restaurant.h"
#include "BowlingAlley.h"

#include <cctype>
#include <iostream>
#include <limits>
using namespace std;

int main()
{
    char choice;

    // keep asking until user enters valid input
    while (true)
    {
        cout << "Choose a simulator: type 'r' for restaurant or 'b' for bowling alley: ";

        if (!(cin >> choice))
        {
            // handle invalid input
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        // convert input to lowercase
        choice = static_cast<char>(tolower(static_cast<unsigned char>(choice)));

        if (choice == 'b' || choice == 'r')
            break;

        cout << "Invalid choice. Please enter 'r' or 'b'." << endl;
    }

    // start the selected simulation
    if (choice == 'b')
    {
        BowlingAlley alley;
        alley.startGame();
    }
    else
    {
        Restaurant restaurant;
        restaurant.startGame();
    }

    return 0;
}
