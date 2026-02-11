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
    deck.shuffleDeck();

    string player1, player2;
    int player1Score = 0, player2Score = 0;
    int rounds = 5;

    cout << "Card Guessing Game\n";
    cout << "Enter Player 1 name: ";
    getline(cin, player1);
    cout << "Enter Player 2 name: ";
    getline(cin, player2);

    cout << "\nGame Rules\n";
    cout << "- Players take turns guessing the rank and suit of the next card\n";
    cout << "- 1 point for correct rank, 1 point for correct suit\n";
    cout << "- Game lasts 5 rounds\n\n";

    cout << "Preview: First 5 Cards\n";
    for (int i = 0; i < 5; i++)
    {
        deck.drawCard();
    }

    cout << "\nGame Starts Now\n\n";

    for (int round = 1; round <= rounds; round++)
    {
        cout << "Round " << round << "\n";
        cout << "Remaining cards: " << deck.remainingCards() << endl;

        // Player 1's turn
        cout << "\n"
             << player1 << "'s turn:\n";
        cout << "Guess the rank (2-10, Jack, Queen, King, Ace): ";
        string guessRank1;
        getline(cin, guessRank1);
        cout << "Guess the suit (Hearts, Diamonds, Clubs, Spades): ";
        string guessSuit1;
        getline(cin, guessSuit1);

        Card actual = deck.getFront();
        cout << "The card is: " << actual.toString() << endl;

        if (actual.toString().find(guessRank1) != string::npos)
        {
            cout << "Correct rank\n";
            player1Score++;
        }
        else
        {
            cout << "Wrong rank\n";
        }

        if (actual.toString().find(guessSuit1) != string::npos)
        {
            cout << "Correct suit\n";
            player1Score++;
        }
        else
        {
            cout << "Wrong suit\n";
        }

        deck.drawCard();

        // Player 2's turn
        cout << "\n"
             << player2 << "'s turn:\n";
        cout << "Guess the rank (2-10, Jack, Queen, King, Ace): ";
        string guessRank2;
        getline(cin, guessRank2);
        cout << "Guess the suit (Hearts, Diamonds, Clubs, Spades): ";
        string guessSuit2;
        getline(cin, guessSuit2);

        actual = deck.getFront();
        cout << "The card is: " << actual.toString() << endl;

        if (actual.toString().find(guessRank2) != string::npos)
        {
            cout << "Correct rank\n";
            player2Score++;
        }
        else
        {
            cout << "Wrong rank\n";
        }

        if (actual.toString().find(guessSuit2) != string::npos)
        {
            cout << "Correct suit\n";
            player2Score++;
        }
        else
        {
            cout << "Wrong suit\n";
        }

        deck.drawCard();

        cout << "\nCurrent Scores - " << player1 << ": " << player1Score << " | " << player2 << ": " << player2Score << endl;
    }

    cout << "\nFinal Scores\n";
    cout << player1 << ": " << player1Score << endl;
    cout << player2 << ": " << player2Score << endl;

    if (player1Score > player2Score)
        cout << "\n"
             << player1 << " wins\n";
    else if (player2Score > player1Score)
        cout << "\n"
             << player2 << " wins\n";
    else
        cout << "\nIt's a tie\n";

    return 0;
}
