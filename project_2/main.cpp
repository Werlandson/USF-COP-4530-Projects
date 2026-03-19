#include "Restaurant.h"
#include "BowlingAlley.h"

#include <cctype>
#include <iostream>
#include <limits>
using namespace std;

int main()
{
    char choice;
    while (true)
    {
        cout << "Choose a simulator: type 'r' for restaurant or 'b' for bowling alley: ";
        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        choice = static_cast<char>(std::tolower(static_cast<unsigned char>(choice)));
        if (choice == 'b' || choice == 'r')
            break;

        cout << "Invalid choice. Please enter 'r' or 'b'." << endl;
    }

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
