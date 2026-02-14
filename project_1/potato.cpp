#include <iostream>
#include <string>
using namespace std;

class CircularLinkedList
{
private:
    struct Node
    {
        string name;
        Node *next;
        Node(string n) : name(n), next(nullptr) {}
    };

    Node *tail;
    int size;

public:
    CircularLinkedList() : tail(nullptr), size(0) {}

    bool isEmpty() const
    {
        return tail == nullptr;
    }

    int getSize() const
    {
        return size;
    }

    void addPlayer(const string &name)
    {
        Node *newNode = new Node(name);

        if (isEmpty())
        {
            tail = newNode;
            tail->next = tail;
        }
        else
        {
            newNode->next = tail->next;
            tail->next = newNode;
            tail = newNode;
        }
        size++;
    }

    string removeNext(Node *&current)
    {
        if (size == 1)
        {
            string winner = current->name;
            delete current;
            tail = nullptr;
            size = 0;
            return winner;
        }

        Node *temp = current->next;
        string removedName = temp->name;

        current->next = temp->next;

        if (temp == tail)
        {
            tail = current;
        }

        delete temp;
        size--;
        return removedName;
    }

    string playGame(int passes)
    {
        if (isEmpty())
            return "No players.";

        Node *current = tail;

        while (size > 1)
        {
            for (int i = 0; i < passes - 1; i++)
            {
                current = current->next;
            }

            cout << "Eliminated: " << current->next->name << endl;
            removeNext(current);
            printPlayers();
        }

        return tail->name;
    }

    void printPlayers() const
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
            cout << p->name;
            first = false;
            p = p->next;
        } while (p != start);
        cout << endl;
    }

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
    CircularLinkedList game;
    int n, k;

    cout << "Enter number of players: ";
    cin >> n;

    cout << "Enter number of passes before elimination: ";
    cin >> k;

    // Automatically assign Player 1, Player 2, ...
    for (int i = 1; i <= n; i++)
    {
        game.addPlayer("Player " + to_string(i));
    }

    cout << "\nStarting Hot Potato Game: \n\n";

    game.printPlayers();

    string winner = game.playGame(k);

    cout << "\nWinner is: " << winner << endl;

    return 0;
}
