/*
 * Names: Tuan Minh Duong, Waylon Erlandson, and Duc Long Nguyen
 * Description: Generic linked list with empty, push_front, push_back, pop_front, pop_back, head, tail, and size functionality.
 * */

#pragma once

// Generic linked list node
template <typename T>
class Node
{
public:
    T elem;
    Node<T> *next;
};

// Singly linked list with head and tail pointers
template <typename T>
class LinkedList
{
public:
    // Linked List constructor
    LinkedList() : head(nullptr), tail(nullptr), size(0) {}
    // Linked List destructor
    ~LinkedList()
    {
        // Remove elements until the list is empty
        while (!empty())
        {
            pop_front();
        }
    }
    // Returns boolean indicating whether the list is empty or not
    bool empty() const
    {
        return head == nullptr;
    }
    // Adds node to the front of the list
    void push_front(const T &elem)
    {
        Node<T> *v = new Node<T>;
        v->elem = elem;
        v->next = head;

        // If list is empty, tail also points to new node
        if (head == nullptr)
            tail = v;

        head = v;
        size += 1;
    }
    // Adds node to end of list
    void push_back(const T &elem)
    {
        Node<T> *v = new Node<T>;
        v->elem = elem;
        v->next = nullptr;

        // If list is empty, new node becomes head
        if (head == nullptr)
        {
            head = v;
        }
        else
        {
            tail->next = v;
        }

        tail = v;
        size += 1;
    }
    // Removes the head of the list
    void pop_front()
    {
        if (head == nullptr)
        {
            return;
        }

        Node<T> *old = head;
        head = head->next;

        // If only one node, tail points to nullptr
        if (head == nullptr)
        {
            tail = nullptr;
        }

        delete old;
        size -= 1;
    }
    // Removes last node if it exists
    void pop_back()
    {
        // Returns if list is empty to avoid remaining logic
        if (head == nullptr)
            return;

        Node<T> *old = tail;

        // If only one node, head and tail point to nullptr
        if (head == tail)
        {
            head = nullptr;
            tail = nullptr;
            delete old;
            size -= 1;
            return;
        }

        Node<T> *p;

        // Iterates to node before tail
        for (p = head; p->next != tail; p = p->next)
            ;

        tail = p;
        p->next = nullptr;
        delete old;
        size -= 1;
    }
    // Returns immutable head
    const Node<T> *getHead() const
    {
        return head;
    }
    // Returns immutable tail
    const Node<T> *getTail() const
    {
        return tail;
    }
    // Returns size
    int getSize() const
    {
        return size;
    }

private:
    Node<T> *head;
    Node<T> *tail;
    int size;
};
