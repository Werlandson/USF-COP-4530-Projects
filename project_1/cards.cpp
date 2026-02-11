#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;

// Card Class
class Card
{
private:
    string rank;
    string suit;

public:
    Card() {}
    Card(string r, string s) : rank(r), suit(s) {}

    string toString() const
    {
        return rank + " of " + suit;
    }
};

// Template Circular Linked List
template <typename T>
class CircularLinkedList
{
private:
    struct Node
    {
        T data;
        Node *next;
        Node(const T &value) : data(value), next(nullptr) {}
    };

    Node *head;
    Node *tail;
    int size;

public:
    CircularLinkedList() : head(nullptr), tail(nullptr), size(0) {}

    // Destructor
    ~CircularLinkedList()
    {
        clear();
    }

    void insert(const T &value)
    {
        Node *newNode = new Node(value);

        if (!head)
        {
            head = tail = newNode;
            newNode->next = head;
        }
        else
        {
            tail->next = newNode;
            tail = newNode;
            tail->next = head;
        }

        size++;
    }

    void removeFront()
    {
        if (!head)
            return;

        if (head == tail)
        {
            delete head;
            head = tail = nullptr;
        }
        else
        {
            Node *temp = head;
            head = head->next;
            tail->next = head;
            delete temp;
        }

        size--;
    }

    void display() const
    {
        if (!head)
        {
            cout << "List is empty.\n";
            return;
        }

        Node *current = head;
        do
        {
            cout << current->data.toString() << endl;
            current = current->next;
        } while (current != head);
    }

    vector<T> toVector() const
    {
        vector<T> elements;

        if (!head)
            return elements;

        Node *current = head;
        do
        {
            elements.push_back(current->data);
            current = current->next;
        } while (current != head);

        return elements;
    }

    void clear()
    {
        if (!head)
            return;

        Node *current = head;
        Node *temp;

        do
        {
            temp = current;
            current = current->next;
            delete temp;
        } while (current != head);

        head = tail = nullptr;
        size = 0;
    }

    int getSize() const
    {
        return size;
    }

    T getFront() const
    {
        if (head)
            return head->data;
        return T();
    }

    void rotateHeadTail()
    {
        if (head && head->next != head)
        {
            head = head->next;
            tail = tail->next;
        }
    }
};

// Deck Class (Uses CircularLinkedList<Card>)
class Deck
{
private:
    CircularLinkedList<Card> cards;

public:
    void initializeDeck()
    {
        string suits[] = {"Hearts", "Diamonds", "Clubs", "Spades"};
        string ranks[] = {"2", "3", "4", "5", "6", "7", "8", "9", "10",
                          "Jack", "Queen", "King", "Ace"};

        for (string suit : suits)
        {
            for (string rank : ranks)
            {
                cards.insert(Card(rank, suit));
            }
        }
    }

    void shuffleDeck()
    {
        vector<Card> temp = cards.toVector();

        random_device rd;
        mt19937 g(rd());

        shuffle(temp.begin(), temp.end(), g);

        cards.clear();

        for (const Card &card : temp)
        {
            cards.insert(card);
        }

        cout << "Deck shuffled successfully!\n";
    }

    void drawCard()
    {
        cout << "Drawing: " << cards.getFront().toString() << endl;
        cards.removeFront();
    }

    Card getFront()
    {
        return cards.getFront();
    }

    void showDeck()
    {
        cards.display();
    }

    void rotateDeck()
    {
        cards.rotateHeadTail();
    }

    int remainingCards()
    {
        return cards.getSize();
    }
};

// Main Function
int main()
{
    Deck deck;
    deck.initializeDeck();

    cout << "Original Deck\n";
    deck.showDeck();

    cout << "\nShuffling Deck\n";
    deck.shuffleDeck();

    cout << "Shuffled Deck\n";
    deck.showDeck();

    random_device rd;
    mt19937 g(rd());
    uniform_int_distribution<> dis(1, 52);
    int randomRotations = dis(g);

    cout << "\nRotating deck " << randomRotations << " times\n";
    for (int i = 0; i < randomRotations; i++)
    {
        deck.rotateDeck();
    }

    cout << "\nDeck After Rotation\n";
    deck.showDeck();

    return 0;
}
