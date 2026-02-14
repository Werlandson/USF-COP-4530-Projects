#include <iostream>
#include <string>
using namespace std;

// Player class
class Player
{
private:
    string name;

public:
    // Constructor: Initialize a player with a name
    Player(const string &pname) : name(pname) {}

    // Return player's name as a string
    string toString() const
    {
        return name;
    }
};

// Circular Linked List class using templates
template <typename T>
class CircularLinkedList
{
private:
    // Node structure for the linked list
    struct Node
    {
        T data;     // Stores the element
        Node *next; // Points to the next node
        Node(const T &d) : data(d), next(nullptr) {}
    };

    Node *tail; // Points to the last node in the circular list
    int size;   // Tracks the number of elements

public:
    // Constructor: Initialize empty list
    CircularLinkedList() : tail(nullptr), size(0) {}

    // Check if the list is empty
    bool isEmpty() const
    {
        return tail == nullptr;
    }

    // Add an element to the end of the list
    void addElement(const T &data)
    {
        Node *newNode = new Node(data);

        if (isEmpty())
        {
            tail = newNode;
            tail->next = tail; // Point to itself for circular structure
        }
        else
        {
            newNode->next = tail->next;
            tail->next = newNode;
            tail = newNode;
        }
        size++;
    }

    // Remove the next element in the circular order
    T removeNext(Node *&current)
    {
        if (size == 1)
        {
            T winner = current->data;
            delete current;
            tail = nullptr;
            size = 0;
            return winner;
        }

        Node *temp = current->next;
        T removedData = temp->data;

        current->next = temp->next;

        if (temp == tail)
        {
            tail = current;
        }

        delete temp;
        size--;
        return removedData;
    }

    // Play the hot potato game
    T playGame(int passes)
    {
        if (isEmpty())
            throw runtime_error("No players in list.");

        Node *current = tail;

        // Continue eliminating until one player remains
        while (size > 1)
        {
            // Pass the potato 'passes' times
            for (int i = 0; i < passes - 1; i++)
            {
                current = current->next;
            }

            // Eliminate the next player
            T eliminated = current->next->data;
            cout << "Eliminated: " << eliminated.toString() << endl;
            removeNext(current);
            printElements();
        }

        return tail->data;
    }

    // Display all remaining elements in the list
    void printElements() const
    {
        if (isEmpty())
        {
            cout << "No players remaining." << endl;
            return;
        }

        Node *start = tail->next;
        cout << "Remaining players (" << size << "): ";
        Node *p = start;
        bool first = true;
        do
        {
            if (!first)
                cout << ", ";
            cout << p->data.toString();
            first = false;
            p = p->next;
        } while (p != start);
        cout << endl;
    }

    // Destructor: Free all allocated memory
    ~CircularLinkedList()
    {
        while (!isEmpty())
        {
            removeNext(tail);
        }
    }
};

int main()
{
    // Create a circular linked list to store players
    CircularLinkedList<Player> game;
    int n, k;

    // Get game parameters from user
    cout << "Enter number of players: ";
    cin >> n;

    cout << "Enter number of passes before elimination: ";
    cin >> k;

    // Add players to the game
    for (int i = 1; i <= n; i++)
    {
        game.addElement(Player("Player " + to_string(i)));
    }

    // Display initial game state and play
    cout << "\nStarting Hot Potato Game: \n\n";
    game.printElements();

    Player winner = game.playGame(k);

    // Display the winner
    cout << "\nWinner is: " << winner.toString() << endl;

    return 0;
}
