/*
 * Name: Minh Duong, Waylon Erlandson, Long Nguyen
 * Descriptions: Implements a simple hot potato game where a person is eliminated after holding the
 * potato for a given time threshold. The last person standing wins. The program assumes that the first
 * two inputs, number of players and name, are int and string, respectively and does not handle type
 * errors. Additionally, the program assumes that the user is a fair sport and only presses enter when
 * they are prompted to. If the user preemptively presses enter, the subsequent turn will take approximately
 * zero time, and the user will unfairly win.
 * */

#include <thread>
#include <chrono>
#include <random>
#include <iostream>
using namespace std;

// Generic linked list node
template <typename T>
class CNode
{
public:
    T elem;
    CNode *next;
};

// Generic circularly linked list with add, remove, empty, advance, size, and getCursor abstract functionality
template <typename T>
class CircleList
{
public:
    CircleList() : cursor(NULL), size(0) {}
    ~CircleList()
    {
        // Removes nodes until the list is empty
        while (!empty())
            remove();
    }
    bool empty() const
    {
        return cursor == NULL;
    }
    CNode<T> *getCursor() const
    {
        return cursor;
    }
    void advance()
    {
        cursor = cursor->next;
    }
    void add(const T &elem)
    {
        CNode<T> *v = new CNode<T>;
        v->elem = elem;
        // If list is empty, cursor points to itself
        if (cursor == NULL)
        {
            cursor = v;
            v->next = v;
        }
        else
        {
            v->next = cursor->next;
            cursor->next = v;
        }
        size += 1;
    }
    void remove()
    {
        CNode<T> *old = cursor->next;

        // If there is one node in the list, cursor is set to NULL
        if (old == cursor)
            cursor = nullptr;
        else
            cursor->next = old->next;

        size -= 1;

        delete old;
    }
    int getSize() const
    {
        return size;
    }

private:
    CNode<T> *cursor;
    int size;
};

// Stores a player name, whether the player is the user, and time spent touching potato
// Provides functions for updating time spent touching potato and accessing player name and boolean for whether the player is the user.
class Player
{
private:
    string name;
    // Dictates whether a player is the user or not
    bool isUser;
    double timeTouchingPotato;

public:
    Player() {}
    Player(string name, bool isUser) : name(name), isUser(isUser), timeTouchingPotato(0) {}
    // Increases time spent touching potato by (timeSpentTouchingPotato)
    void addTimeTouchingPotato(double timeSpentTouchingPotato)
    {
        timeTouchingPotato += timeSpentTouchingPotato;
    }
    bool getIsUser()
    {
        return isUser;
    }
    string getName()
    {
        return name;
    }
    double getTimeTouchingPotato()
    {
        return timeTouchingPotato;
    }
};

// Takes a circularly linked list of players and provides functionality for playing an entire Hot Potato game with the list of players.
class Game
{
public:
    Game() : round(1) {}
    // Returns a boolean for whether the game ended
    bool gameEnded()
    {
        if (players.getSize() == 1)
        {
            return true;
        }
        else
        {
            return false;
        }
    };
    // Starts a hot potato game, calling playRound until gameEnded is true and printing contextual information
    void playGame()
    {
        // Clear leftover newline from the buffer
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "\nStarting Hot Potato Game: \nRemember, don't hold the hot potato for too long!\n\n";

        // If the game has not ended, start another round.
        while (!gameEnded())
        {
            playRound();
            round += 1;
        }

        cout << endl;
        // Remaining player is the champion.
        cout << players.getCursor()->elem.getName() << " is the Hot Potato champion!!!" << endl;
    }
    /* Starts a Hot Potato round. The potato passes through each player until a player has held the potato for 3 seconds. For the user,
     * the time taken to press enter is the time added to their timeTouchingPotato score. For non-users, the time is a random number
     * between 0.15 and 1 seconds.
     *  */
    void playRound()
    {
        // Initializes random number generator, range [0.05, 1], for the time a potato is held by the bot.
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<double> dist(0.15, 1.0);

        std::cout << "Round " << round << ": " << endl;
        std::this_thread::sleep_for(std::chrono::duration<double>(1));
        std::cout << endl
                  << "Start!" << endl
                  << endl;

        // Infinite loop iterates through players until a break statement is met.
        while (true)
        {
            CNode<Player> *currPlayer = players.getCursor();

            if (currPlayer->elem.getIsUser())
            {
                cout << "Your turn! Press enter to pass the potato: " << endl;

                // If currPlayer is user, timeTouchingPotato is determined by how long it takes for the user to press enter.
                auto start = std::chrono::steady_clock::now();
                std::cin.get();
                auto end = std::chrono::steady_clock::now();

                std::chrono::duration<double> timeTouchingPotato = end - start;
                currPlayer->elem.addTimeTouchingPotato(timeTouchingPotato.count());
            }
            else
            {
                // timeTouchingPotato is a random number in range [0.15, 1]
                double timeTouchingPotato = dist(gen);
                std::this_thread::sleep_for(std::chrono::duration<double>(timeTouchingPotato));
                currPlayer->elem.addTimeTouchingPotato(timeTouchingPotato);
            }

            if (currPlayer->elem.getTimeTouchingPotato() > 3)
            {
                std::cout
                    << currPlayer->elem.getName() << " has been eliminated." << endl
                    << endl;
                // Moves cursor to the node before the eliminated node for removal purposes while giving players additional time
                for (int j = 0; j < players.getSize() - 1; j++)
                {
                    players.advance();
                    // Gives the player additional time to touch potato in next round.
                    players.getCursor()->elem.addTimeTouchingPotato(-1.0);
                }
                // Removes eliminated player from players list
                players.remove();
                break;
            }
            cout << currPlayer->elem.getName() << " passes the potato!" << endl;
            players.advance();
        }
    }
    void addPlayer(string name, bool isUser)
    {
        players.add(Player(name, isUser));
    }

private:
    CircleList<Player> players;
    int round;
};

int main()
{

    Game game;
    string username;
    int n;

    // Get game parameters from user
    cout << "Enter number of players: ";
    cin >> n;

    cout << "Enter your name: ";
    cin >> username;

    // Add players to the game
    for (int i = n; i > 0; i--)
    {
        game.addPlayer("Player " + to_string(i), false);
    }
    game.addPlayer(username, true);
    game.playGame();

    return 0;
}
