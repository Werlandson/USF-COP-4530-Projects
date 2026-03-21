/*
 * Names: Waylon Erlandson, Minh Duong, Duc Long Nguyen
 * Description: A generic queue implemented using a circularly linked list.
 * Supports enqueue, dequeue, peek, and size operations with basic thread safety.
 */
#pragma once

#include <condition_variable>
#include <mutex>
#include <stdexcept>
#include <vector>
#include "CircleList.h"

using namespace std;

template <typename T>
class Queue
{
public:
    void enqueue(const T &elem)
    {
        {
            lock_guard<mutex> lock(m_);
            if (closed_) return; // don't add if queue is closed
            queue.add(elem);
        }
        cv_.notify_one(); // wake up a waiting thread
    }

    void dequeue()
    {
        unique_lock<mutex> lock(m_);

        // wait until there is data or queue is closed
        cv_.wait(lock, [&]
                 { return closed_ || queue.getSize() > 0; });

        if (queue.getSize() == 0)
            throw runtime_error("Queue closed");

        queue.remove();
    }

    T peek() const
    {
        unique_lock<mutex> lock(m_);

        // wait until there is data or queue is closed
        cv_.wait(lock, [&]
                 { return closed_ || queue.getSize() > 0; });

        if (queue.getSize() == 0)
            throw runtime_error("Queue closed");

        return queue.getCursor()->next->elem;
    }

    int getSize() const
    {
        lock_guard<mutex> lock(m_);
        return queue.getSize();
    }

    vector<T> getAll() const
    {
        lock_guard<mutex> lock(m_);
        return queue.getAll();
    }

    void close()
    {
        {
            lock_guard<mutex> lock(m_);
            closed_ = true;
        }
        cv_.notify_all(); // wake all waiting threads
    }

private:
    mutable mutex m_;                // protects queue access
    mutable condition_variable cv_;  // used for blocking
    bool closed_ = false;            // stops queue when true
    CLinkedList<T> queue;
};
