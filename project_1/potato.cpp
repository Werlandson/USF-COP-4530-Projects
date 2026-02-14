#include <iostream>
#include <string>
using namespace std;

// Player class
class Player
{
private:
    int id;
    string name;

public:
    Player() : id(0), name("") {}

    Player(int pid, const string &pname) : id(pid), name(pname) {}

    int getId() const { return id; }
    string getName() const { return name; }

    // Simple method to return player info as string
    string toString() const
    {
        return name;
    }
};

// Template class
template <typename T>
class CircularLinkedList
{
private:
    struct Node
    {
        T data;
        Node *next;
        Node(const T &d) : data(d), next(nullptr) {}
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

    void addElement(const T &data)
    {
        Node *newNode = new Node(data);

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

    T playGame(int passes)
    {
        if (isEmpty())
            throw runtime_error("No players in list.");

        Node *current = tail;

        while (size > 1)
        {
            for (int i = 0; i < passes - 1; i++)
            {
                current = current->next;
            }

            T eliminated = current->next->data;
            cout << "Eliminated: " << eliminated.toString() << endl;
            removeNext(current);
            printElements();
        }

        return tail->data;
    }

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
    // Using the template with Player class
    CircularLinkedList<Player> game;
    int n, k;

    cout << "Enter number of players: ";
    cin >> n;

    cout << "Enter number of passes before elimination: ";
    cin >> k;

    // Automatically assign Player 1, Player 2, ...
    for (int i = 1; i <= n; i++)
    {
        game.addElement(Player(i, "Player " + to_string(i)));
    }

    cout << "\nStarting Hot Potato Game: \n\n";

    game.printElements();

    Player winner = game.playGame(k);

    cout << "\nWinner is: " << winner.toString() << endl;

    return 0;
}
