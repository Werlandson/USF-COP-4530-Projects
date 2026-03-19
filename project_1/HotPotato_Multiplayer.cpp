#include <thread>
#include <chrono>
#include <random>
#include <iostream>
#include <cctype>
#include <string>

using namespace std;

template <typename T>
class CNode
{
public:
    T elem;
    CNode *next;
};

template <typename T>
class CircleList
{
public:
    CircleList() : cursor(NULL), size(0) {}
    ~CircleList()
    {
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

        if (old == cursor)
            cursor = NULL;
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

// Stores a player name and time spent touching potato
// Provides functions for updating time spent touching potato and accesing player name
class Player
{
private:
    string name;
    // Dictates whether a player is the user or not
    bool isUser;
    char playerKey;
    double timeTouchingPotato;

public:
    Player() {}
    Player(string name, bool isUser, char key = ' ') : name(name), isUser(isUser), playerKey(key), timeTouchingPotato(0) {}
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
    char getPlayerKey()
    {
        return playerKey;
    }
};

class Game
{
public:
    Game() : round(1) {}
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
    void playGame()
    {
        // Clear leftover newline from the buffer
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "\nStarting Hot Potato Game: \nRemember, don't hold the hot potato for too long!\n\n";

        while (!gameEnded())
        {
            playRound();
            round += 1;
        }

        cout << endl;
        cout << players.getCursor()->elem.getName() << " is the Hot Potato champion!!!" << endl;
    }
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

        while (true)
        {
            CNode<Player> *currPlayer = players.getCursor();

            if (currPlayer->elem.getIsUser())
            {
                cout << currPlayer->elem.getName() << ", press '" << currPlayer->elem.getPlayerKey() << "' to pass: ";
                auto start = std::chrono::steady_clock::now();
                
                char inputKey;
                while (true)
                {
                    cin >> inputKey;
                    inputKey = tolower(inputKey);
                    if (inputKey == currPlayer->elem.getPlayerKey())
                    {
                        cout << endl;
                        break;
                    }
                    else
                    {
                        cout << "Wrong key! Try again: ";
                    }
                }
                
                auto end = std::chrono::steady_clock::now();
                std::chrono::duration<double> timeTouchingPotato = end - start;
                currPlayer->elem.addTimeTouchingPotato(timeTouchingPotato.count());
            }
            else
            {
                double timeTouchingPotato = dist(gen);
                std::this_thread::sleep_for(std::chrono::duration<double>(timeTouchingPotato));
                currPlayer->elem.addTimeTouchingPotato(timeTouchingPotato);
            }

            if (currPlayer->elem.getTimeTouchingPotato() > 3)
            {
                std::cout
                    << currPlayer->elem.getName() << " has been eliminated." << endl
                    << endl;
                for (int j = 0; j < players.getSize() - 1; j++)
                {
                    players.advance();
                    // Gives the player additional time to touch potato in next round.
                    players.getCursor()->elem.addTimeTouchingPotato(-1.0);
                }
                players.remove();
                break;
            }
            cout << currPlayer->elem.getName() << " passes the potato!" << endl;
            players.advance();
        }
    }
    void addPlayer(string name, bool isUser, char key = ' ')
    {
        players.add(Player(name, isUser, key));
    }

private:
    CircleList<Player> players;
    int round;
};

int main()
{
    Game game;
    int humanPlayers, botPlayers;

    // Get game parameters from user
    cout << "=== Hot Potato Game ===" << endl;
    cout << "Enter number of human players: ";
    cin >> humanPlayers;

    // Validate human players input
    while (humanPlayers < 1)
    {
        cout << "Please type in 1 or more human players" << endl;
        cout << "Enter number of human players: ";
        cin >> humanPlayers;
    }

    cout << "Enter number of bot players: ";
    cin >> botPlayers;

    // Validate bot players input
    while (botPlayers < 0)
    {
        cout << "Please type in 0 or more bot players" << endl;
        cout << "Enter number of bot players: ";
        cin >> botPlayers;
    }

    // Validate total players
    if (humanPlayers + botPlayers < 2)
    {
        cout << "Total players must be 2 or more!" << endl;
        cout << "Setting bot players to 1..." << endl;
        botPlayers = 1;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // Add human players with assigned keys
    string keys = "asdfghjkl";
    for (int i = 0; i < humanPlayers; i++)
    {
        string playerName;
        cout << "Enter name for Player " << (i + 1) << " (key: " << keys[i] << "): ";
        getline(cin, playerName);
        game.addPlayer(playerName, true, keys[i]);
    }

    // Add bot players
    for (int i = 0; i < botPlayers; i++)
    {
        game.addPlayer("Bot " + to_string(i + 1), false, ' ');
    }

    game.playGame();

    return 0;
}