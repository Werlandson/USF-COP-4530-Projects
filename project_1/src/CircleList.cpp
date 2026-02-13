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
    CircleList();
    ~CircleList();
    bool empty() const;
    const T &back() const;
    const T &front() const;
    void advance();
    void add(const T &elem);
    void remove();

private:
    CNode<T> *cursor;
};

template <typename T>
CircleList<T>::CircleList() : cursor(NULL) {}

template <typename T>
CircleList<T>::~CircleList()
{
    while (!empty())
        remove();
}

template <typename T>
bool CircleList<T>::empty() const
{
    return cursor == NULL;
}

template <typename T>
const T &CircleList<T>::back() const
{
    return cursor->elem;
}

template <typename T>
const T &CircleList<T>::front() const
{
    return cursor->next->elem;
}

template <typename T>
void CircleList<T>::advance()
{
    cursor = cursor->next;
}

template <typename T>
void CircleList<T>::add(const T &elem)
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
}

template <typename T>
void CircleList<T>::remove()
{
    CNode<T> *old = cursor->next;

    if (old == cursor)
        cursor = NULL;
    else
        cursor->next = old->next;

    delete old;
}
