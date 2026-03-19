/*
 * Name: Waylon Erlandson
 * Description: A generic queue implemented using a circularly linked list with enqueue, dequeue, peek, and size operations.
 * The queue uses blocking to prevent race conditions when accessing from multiple threads and access attempts on an empty queue.
 * */
#pragma once

#include <condition_variable>
#include <mutex>
#include <stdexcept>
#include <vector>
#include "CircleList.h"

/* Generic queue with enqueue, dequeue, peek, and size operations
implemented using a circularly linked list */
template <typename T>
class Queue
{
public:
    // Adds an element to the queue.
    // If any threads are blocked in peek()/dequeue() waiting for data,
    // one of them will be woken up after the element is added.
    void enqueue(const T &elem)
    {
        {
            std::lock_guard<std::mutex> lock(m_);
            // Once closed, the queue will no longer accept new elements.
            if (closed_)
                return;
            queue.add(elem);
        }
        // Signal that at least one new element is available.
        cv_.notify_one();
    }
    // Removes the front element from the queue.
    // If the queue is empty, this call will block until:
    //   1) another thread enqueues an element, or
    //   2) the queue is closed via close().
    // If the queue is closed and empty when it wakes, it throws "Queue closed".
    void dequeue()
    {
        std::unique_lock<std::mutex> lock(m_);
        // Block until there is an element to remove or the queue is closed.
        cv_.wait(lock, [&]
                 { return closed_ || queue.getSize() > 0; });
        if (queue.getSize() == 0)
            throw std::runtime_error("Queue closed");
        queue.remove();
    }
    // Returns the front element of the queue by value.
    // If the queue is empty, this call will block until:
    //   1) another thread enqueues an element, or
    //   2) the queue is closed via close().
    // If the queue is closed and empty when it wakes, it throws "Queue closed".
    T peek() const
    {
        std::unique_lock<std::mutex> lock(m_);
        // Block until there is an element to read or the queue is closed.
        cv_.wait(lock, [&]
                 { return closed_ || queue.getSize() > 0; });
        if (queue.getSize() == 0)
            throw std::runtime_error("Queue closed");
        return queue.getCursor()->next->elem;
    }
    int getSize() const
    {
        std::lock_guard<std::mutex> lock(m_);
        return queue.getSize();
    }

    std::vector<T> getAll() const
    {
        std::lock_guard<std::mutex> lock(m_);
        return queue.getAll();
    }

    // Marks the queue as closed and wakes any threads blocked in
    // peek()/dequeue(). After this:
    //   - enqueue() becomes a no-op
    //   - blocked consumers will wake, see an empty closed queue,
    //     and throw "Queue closed" so they can exit cleanly.
    void close()
    {
        {
            std::lock_guard<std::mutex> lock(m_);
            closed_ = true;
        }
        cv_.notify_all();
    }

private:
    mutable std::mutex m_;               // Used to implement blocking behavior of peek, enqueue, and dequeue operations
    mutable std::condition_variable cv_; // Used to prevent peek operation until queue has element(s)
    bool closed_ = false;                // Used to prevent methods from waiting indefinitely
    CLinkedList<T> queue;
};
