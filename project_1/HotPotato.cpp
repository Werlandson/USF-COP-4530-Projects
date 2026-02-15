#include <thread>
#include <chrono>
#include <random>
#include <iostream>
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
                cout << "Your turn! Press enter to pass the potato: " << endl;
                auto start = std::chrono::steady_clock::now();
                std::cin.get();
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
