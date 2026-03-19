/*
 * Name: Waylon Erlandson
 * Description: Implementation for Company and Group classes.
 */

#include "Company.h"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <thread>
#include <vector>

using namespace std;

Group::Group()
{
    // Each Group is created with random size and difficulty.
    random_device rd;
    mt19937 gen(rd());

    uniform_int_distribution<> distr(1, 8);
    size = distr(gen);
    difficulty = distr(gen);

    // Timestamp used to determine how long a group has been waiting.
    timeArrived = chrono::system_clock::now();
}

Group::Group(const Group &group)
    : size(group.size), difficulty(group.difficulty), timeArrived(group.timeArrived) {}

chrono::system_clock::time_point Group::getTimeArrived() const
{
    return timeArrived;
}

int Group::getSize() const
{
    return size;
}

int Group::getDifficulty() const
{
    return difficulty;
}

Company::Company()
    : waitingLine(),
      tables(),
      earnings(0),
      gameIsRunning(false),
      tableCapacity(10) {}

Company::~Company()
{
    // Ensure worker threads can't block forever if the object is destroyed.
    stopGame();

    if (randomArrivalThread.joinable())
        randomArrivalThread.join();
    if (fillTableThread.joinable())
        fillTableThread.join();
    if (monitorWaitThread.joinable())
        monitorWaitThread.join();
    if (serveTablesThread.joinable())
        serveTablesThread.join();
}

void Company::startGame()
{
    printWelcome();
    gameIsRunning.store(true);

    // Start the simulation threads (joinable so they can't outlive the Company object).
    randomArrivalThread = thread(&Company::randomArrivalGeneration, this);
    fillTableThread = thread(&Company::fillTableOpenings, this);
    monitorWaitThread = thread(&Company::monitorWaitTimes, this);
    serveTablesThread = thread(&Company::serveTables, this);

    // Wait for the simulation threads to finish after the game ends.
    if (randomArrivalThread.joinable())
        randomArrivalThread.join();
    if (fillTableThread.joinable())
        fillTableThread.join();
    if (monitorWaitThread.joinable())
        monitorWaitThread.join();
    if (serveTablesThread.joinable())
        serveTablesThread.join();
}

bool Company::isGameRunning() const
{
    return gameIsRunning.load();
}

float Company::getEarnings() const
{
    return earnings;
}

void Company::fillTableOpenings()
{
    // Consumer/producer thread:
    // - consumes from waitingLine (blocks on peek/dequeue when empty)
    // - produces into tables (enqueue)
    while (gameIsRunning.load())
    {
        if (tables.getSize() < tableCapacity)
        {
            try
            {
                Group group = waitingLine.peek();
                waitingLine.dequeue();
                tables.enqueue(group);
            }
            catch (const std::runtime_error &)
            {
                break;
            }
        }
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}

void Company::monitorWaitTimes()
{
    // Monitor thread:
    // Checks whether the oldest waiting group has been waiting too long.
    while (gameIsRunning.load())
    {
        const auto waiting = waitingLine.getAll();
        if (!waiting.empty() &&
            waiting.front().getTimeArrived() + chrono::seconds(5) < chrono::system_clock::now())
        {
            stopGame();
        }
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}

void Company::randomArrivalGeneration()
{
    // Producer thread: random arrivals between 0.5 and 4 seconds.
    mt19937 gen(random_device{}());
    uniform_int_distribution<> distr(500, 4000);
    while (gameIsRunning.load())
    {
        this_thread::sleep_for(chrono::milliseconds(distr(gen)));
        waitingLine.enqueue(Group());
    }
}

void Company::serveTables()
{
    // Consumer thread: takes groups from tables and runs the child-defined service task.
    mt19937 gen(random_device{}());
    while (gameIsRunning.load())
    {
        uniform_real_distribution<float> tipRange(20, 50);

        Group group;
        try
        {
            group = tables.peek();
            tables.dequeue();
        }
        catch (const std::runtime_error &)
        {
            break;
        }

        float bill = group.getSize() * 30;
        cout << fixed << setprecision(2) << "Next group - "
             << "Bill: $" << bill << "; Difficulty: " << group.getDifficulty() << endl;

        // If the game ends while serving, stop immediately (no finishing / no earnings).
        if (!gameIsRunning.load())
            return;

        performServiceTask(group, gen);

        if (!gameIsRunning.load())
            return;

        bill += tipRange(gen);
        earnings += bill;
        cout << fixed << setprecision(2) << "Table served! Total earnings: $" << earnings << endl
             << endl;
    }
}

void Company::stopGame()
{
    bool expected = true;
    if (!gameIsRunning.compare_exchange_strong(expected, false))
        return;

    waitingLine.close();
    tables.close();

    cout << fixed << setprecision(2) << endl
         << "Game over. Earnings: $" << earnings << endl;
}

