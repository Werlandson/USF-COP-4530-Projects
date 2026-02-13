#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
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
    CNode<T> *getCursor() const;
    const T &back() const;
    const T &front() const;
    void advance();
    void add(const T &elem);
    void remove();

private:
    CNode<T> *cursor;
};