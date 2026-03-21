/*
 * Name: Waylon Erlandson, Duong Minh, Duc Long Nguyen
 * Description: A generic circularly linked list class with add, remove, getCursor, traverse,
 * and size functionality
 * */
#pragma once

#include <vector>

using namespace std;

// Generic linked list node
template <typename T>
class CNode
{
public:
    T elem;
    CNode *next;
};

/* Generic circularly linked list with add, remove, getCursor,
traverse, and size functionality */
template <typename T>
class CLinkedList
{
public:
    // Constructor initializes empty list.
    CLinkedList() : cursor(nullptr), n(0) {}
    // Cleans up all nodes.
    ~CLinkedList()
    {
        // Removes elements until list is empty
        while (!empty())
        {
            remove();
        }
    }
    // Checks if the list is empty.
    bool empty()
    {
        return cursor == nullptr;
    }
    // Returns the current cursor node.
    CNode<T> *getCursor() const
    {
        return cursor;
    }
    // Moves the cursor to the next node.
    void traverse()
    {
        cursor = cursor->next;
    }
    // Adds a new element to the list.
    void add(const T &elem)
    {
        CNode<T> *v = new CNode<T>;
        v->elem = elem;

        // If list is empty, new node points to itself
        if (cursor == nullptr)
        {
            cursor = v;
            cursor->next = v;
        }
        else
        {
            v->next = cursor->next;
            cursor->next = v;
            // New node assigned to cursor so cursor->next is First-In node for remove operation
            cursor = v;
        }
        n += 1;
    }
    void remove()
    {
        CNode<T> *old = cursor->next;

        // If there is one node in the list, cursor is set to nullptr
        if (cursor->next == cursor)
        {
            cursor = nullptr;
        }
        else
        {
            cursor->next = old->next;
        }

        delete old;
        n -= 1;
    }
    int getSize() const
    {
        return n;
    }

    vector<T> getAll() const
    {
        vector<T> result;
        if (cursor == nullptr)
            return result;
        CNode<T> *p = cursor->next;
        for (int i = 0; i < n; i++)
        {
            result.push_back(p->elem);
            p = p->next;
        }
        return result;
    }

private:
    CNode<T> *cursor;
    int n; // Size of list
};
