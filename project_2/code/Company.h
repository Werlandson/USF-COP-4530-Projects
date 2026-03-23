/*
 * Names: Waylon Erlandson, Minh Duong, Duc Long Nguyen
 * Description: Parent class for businesses that manage arriving Groups, seating, serving tasks,
 * earnings, and termination conditions. Child classes customize the serving task.
 */
#pragma once

#include <atomic>
#include <chrono>
#include <random>
#include <thread>

#include "Queue.h"

using namespace std;

/* Class for a group that is initialized with a random size and difficulty.
Size is used for calculating the bill and difficulty is used to determine
the number of service tasks */
class Group
{
public:
    Group(); // creates group with random size/difficulty
    Group(const Group &group);

    chrono::system_clock::time_point getTimeArrived() const;
    int getSize() const;
    int getDifficulty() const;

private:
    int size;
    int difficulty;
    chrono::system_clock::time_point timeArrived; // used to track wait time
};

class Company
{
public:
    Company();
    virtual ~Company();

    void startGame(); // starts simulation

protected:
    virtual void printWelcome() = 0;
    virtual void performServiceTask(const Group &group, mt19937 &gen) = 0;

    void stopGame(); // ends simulation

    bool isGameRunning() const;
    float getEarnings() const;

private:
    void fillTableOpenings();       // move groups from waitingLine to tables
    void monitorWaitTimes();        // end game if waiting too long
    void randomArrivalGeneration(); // add new groups to queue
    void serveTables();             // serve groups at tables

    Queue<Group> waitingLine; // queue of waiting groups
    Queue<Group> tables;      // groups being served

    float earnings;
    atomic<bool> gameIsRunning;
    int tableCapacity;

    thread randomArrivalThread;
    thread fillTableThread;
    thread monitorWaitThread;
    thread serveTablesThread;
};
