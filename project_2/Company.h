/*
 * Name: Waylon Erlandson
 * Description: Parent class for businesses that manage arriving Groups, seating, serving tasks,
 * earnings, and termination conditions. Child classes customize the serving task.
 */
#pragma once

#include <atomic>
#include <chrono>
#include <random>
#include <thread>

#include "Queue.h"

class Group
{
public:
    Group();
    Group(const Group &group);

    std::chrono::system_clock::time_point getTimeArrived() const;
    int getSize() const;
    int getDifficulty() const;

private:
    int size;
    int difficulty;
    std::chrono::system_clock::time_point timeArrived;
};

class Company
{
public:
    Company();
    virtual ~Company();

    // Starts the simulation and blocks until the game ends.
    void startGame();

protected:
    // Child customization points.
    virtual void printWelcome() = 0;
    virtual void performServiceTask(const Group &group, std::mt19937 &gen) = 0;

    // Allows children to stop the game if needed.
    void stopGame();

    bool isGameRunning() const;
    float getEarnings() const;

private:
    void fillTableOpenings();
    void monitorWaitTimes();
    void randomArrivalGeneration();
    void serveTables();

    Queue<Group> waitingLine;
    Queue<Group> tables;
    float earnings;
    std::atomic<bool> gameIsRunning;
    int tableCapacity;

    std::thread randomArrivalThread;
    std::thread fillTableThread;
    std::thread monitorWaitThread;
    std::thread serveTablesThread;
};

