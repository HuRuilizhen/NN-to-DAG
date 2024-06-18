#include "GraphKit.h"

#include <ctime>
#include <cstring>
#include <iostream>
#include <set>
#include <vector>
#include <map>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace GraphKitDp
{
    int numThreadsAlive = 0;
    std::mutex mutex;
    std::mutex memoizationMutex;
    std::mutex numThreadsAliveMutex;
    std::condition_variable conditionVariable;

    struct Memoization
    {
        std::vector<int> schedule;
        std::vector<int> indegree;
        int current_memory;
        int peak_memory;
    };

    struct State
    {
        std::set<int> candidate;
        Memoization memoization;
        bool operator<(const State &other) const
        {
            return memoization.current_memory < memoization.current_memory;
        }
    };

    State currentGuideStateCalculation(State lastGuideState, int candidateNode, Graph &graph, int *inSum, int *outSum)
    {
        State currentGuideState;
        std::set<int> newCandidate = lastGuideState.candidate;
        std::vector<int> newSchedule = lastGuideState.memoization.schedule;
        std::vector<int> newIndegree = lastGuideState.memoization.indegree;
        newCandidate.erase(candidateNode);
        newSchedule.push_back(candidateNode);

        int newcurrentMemory = lastGuideState.memoization.current_memory + outSum[candidateNode] - inSum[candidateNode];
        int peakMemory = std::max(newcurrentMemory, lastGuideState.memoization.peak_memory);

        for (int edge = graph.getEdgeHead(candidateNode); graph.isValid(edge); edge = graph.getEdgeNext(edge))
        {
            int to = graph.getEdgeTo(edge);
            newIndegree[to]--;
            if (newIndegree[to] == 0)
            {
                newCandidate.insert(to);
            }
        }

        currentGuideState.candidate = newCandidate;
        currentGuideState.memoization.schedule = newSchedule;
        currentGuideState.memoization.indegree = newIndegree;
        currentGuideState.memoization.current_memory = newcurrentMemory;
        currentGuideState.memoization.peak_memory = peakMemory;
        return currentGuideState;
    }

    void numThreadsAliveIncrement()
    {
        std::lock_guard<std::mutex> lock(numThreadsAliveMutex);
        numThreadsAlive++;
    }

    void numThreadsAliveDescrement()
    {
        std::lock_guard<std::mutex> lock(numThreadsAliveMutex);
        numThreadsAlive--;
    }

    bool numThreadsAliveChecker()
    {
        std::lock_guard<std::mutex> lock(numThreadsAliveMutex);
        return !numThreadsAlive;
    }

    void stageCalculation(std::set<int> currentCandidate, std::vector<int> currentSchedule, std::vector<int> currentIndegree, std::map<std::set<int>, Memoization> &currentMemoization, int currentMemory, int peakMemory, Graph graph, int *inSum, int *outSum, int boundPeakMemory)
    {
        numThreadsAliveIncrement();

        for (auto node : currentCandidate)
        {
            std::set<int> newCandidate = currentCandidate;
            std::vector<int> newSchedule = currentSchedule;
            std::vector<int> newIndegree = currentIndegree;

            newCandidate.erase(node);
            newSchedule.push_back(node);

            for (int edge = graph.getEdgeHead(node); graph.isValid(edge); edge = graph.getEdgeNext(edge))
            {
                int to = graph.getEdgeTo(edge);
                if (--newIndegree[to])
                    continue;
                newCandidate.insert(to);
            }

            int newCurrentMemory = currentMemory - inSum[node] + outSum[node];
            int newPeakMemory = std::max(peakMemory, newCurrentMemory);

            if (newPeakMemory > boundPeakMemory)
                continue;

            std::unique_lock<std::mutex> memoizationLock(memoizationMutex);
            if (currentMemoization.find(newCandidate) == currentMemoization.end())
                currentMemoization.emplace(newCandidate, GraphKitDp::Memoization{newSchedule, newIndegree, newCurrentMemory, newPeakMemory});
            else if (currentMemoization.at(newCandidate).peak_memory > newPeakMemory)
                currentMemoization.find(newCandidate)
                    ->second = GraphKitDp::Memoization{newSchedule, newIndegree, newCurrentMemory, newPeakMemory};
            memoizationLock.unlock();
        }

        numThreadsAliveDescrement();
        if (numThreadsAliveChecker())
            conditionVariable.notify_all();
    }
};

void GraphKit::dp(int &currentMemory, int &peakMemory, bool multithreading, bool bound, int calculation)
{
    if (calculation < 0)
        calculation = 0x7fffffff;

    int boundPeakMemory = 0x7fffffff;
    if (bound)
        boundPeakMemory = memory;

    std::map<std::set<int>, GraphKitDp::Memoization> lastMemoization;
    std::map<std::set<int>, GraphKitDp::Memoization> currentMemoization;
    GraphKitDp::State lastGuideState;
    GraphKitDp::State currentGuideState;

    /*
        Initialize Memoization related variables
    */
    std::set<int> initCandidate;
    std::vector<int> initSchedule;
    std::vector<int> initIndegree;
    for (int index = 0; index < numStartNodes; index++)
    {
        int startNode = startNodes[index];
        initCandidate.insert(startNode);
    }
    for (int node = 0; node < graph.getNumNodes(); node++)
        initIndegree.push_back(inDegree[node]);
    lastMemoization.emplace(initCandidate, GraphKitDp::Memoization{initSchedule, initIndegree, 0, 0});
    lastGuideState = GraphKitDp::State{initCandidate, GraphKitDp::Memoization{initSchedule, initIndegree, 0, 0}};

    /*
        Execute dynamic programming
    */
    for (int i = 0; i < graph.getNumNodes(); i++)
    {
        std::vector<std::thread> threads;
        std::map<std::set<int>, GraphKitDp::Memoization>::iterator it;
        std::priority_queue<GraphKitDp::State> stateHeap;

        for (it = lastMemoization.begin(); it != lastMemoization.end(); it++)
        {
            if (stateHeap.size() < calculation)
                stateHeap.push({it->first, it->second});
            else if (stateHeap.top().memoization.current_memory > it->second.current_memory)
            {
                stateHeap.pop();
                stateHeap.push({it->first, it->second});
            }
        }

        stateHeap.push(lastGuideState);
        currentGuideState = GraphKitDp::currentGuideStateCalculation(lastGuideState, bestSequence[i], graph, inSum, outSum);
        lastGuideState = currentGuideState;

        while (!stateHeap.empty())
        {
            GraphKitDp::State state = stateHeap.top();
            stateHeap.pop();

            std::set<int> currentCandidate = state.candidate;
            GraphKitDp::Memoization memoization = state.memoization;
            std::vector<int> currentSchedule = memoization.schedule;
            std::vector<int> currentIndegree = memoization.indegree;
            int currentMemory = memoization.current_memory;
            int peakMemory = memoization.peak_memory;

            if (multithreading)
                threads.emplace_back(GraphKitDp::stageCalculation, currentCandidate, currentSchedule, currentIndegree, std::ref(currentMemoization), currentMemory, peakMemory, graph, inSum, outSum, boundPeakMemory);
            else
                GraphKitDp::stageCalculation(currentCandidate, currentSchedule, currentIndegree, currentMemoization, currentMemory, peakMemory, graph, inSum, outSum, boundPeakMemory);
        }

        if (multithreading)
        {
            std::unique_lock<std::mutex> lock(GraphKitDp::mutex);
            GraphKitDp::conditionVariable.wait(lock);

            for (auto &thread : threads)
                if (thread.joinable())
                    thread.join();
        }

        iterationMemory.push_back(getProcessMemory());

        lastMemoization = currentMemoization;
        currentMemoization.clear();
    }

    /*
        compile statistics of results
    */
    std::set<int> finalCandidate;
    GraphKitDp::Memoization finalMemoization = lastMemoization.at(finalCandidate);
    std::vector<int> finalSchedule = finalMemoization.schedule;
    std::memcpy(dpSequence, &finalMemoization.schedule[0], sizeof(finalSchedule[0]) * finalSchedule.size());
    peakMemory = finalMemoization.peak_memory;
}

int GraphKit::dpSoftBudget(int &currentMemory, int &peakMemory, bool multithreading, bool bound, int calculation, int budget, time_t startTime)
{
    if (calculation < 0)
        calculation = 0x7fffffff;

    int boundPeakMemory = 0x7fffffff;
    if (bound)
        boundPeakMemory = memory;
    budget = std::min(budget, memory);

    std::map<std::set<int>, GraphKitDp::Memoization> lastMemoization;
    std::map<std::set<int>, GraphKitDp::Memoization> currentMemoization;

    /*
        Initialize Memoization related variables
    */
    std::set<int> initCandidate;
    std::vector<int> initSchedule;
    std::vector<int> initIndegree;
    for (int index = 0; index < numStartNodes; index++)
    {
        int startNode = startNodes[index];
        initCandidate.insert(startNode);
    }
    for (int node = 0; node < graph.getNumNodes(); node++)
        initIndegree.push_back(inDegree[node]);
    lastMemoization.emplace(initCandidate, GraphKitDp::Memoization{initSchedule, initIndegree, 0, 0});

    /*
        Execute dynamic programming
    */
    for (int i = 0; i < graph.getNumNodes(); i++)
    {
        std::vector<std::thread> threads;
        std::map<std::set<int>, GraphKitDp::Memoization>::iterator it;
        std::priority_queue<GraphKitDp::State> stateHeap;

        for (it = lastMemoization.begin(); it != lastMemoization.end(); it++)
        {
            if (stateHeap.size() < calculation)
                stateHeap.push({it->first, it->second});
            else if (stateHeap.top().memoization.current_memory > it->second.current_memory)
            {
                stateHeap.pop();
                stateHeap.push({it->first, it->second});
            }
        }

        if (stateHeap.empty())
            return NO_SOLUTION;

        while (!stateHeap.empty())
        {
            GraphKitDp::State state = stateHeap.top();
            stateHeap.pop();

            std::set<int> currentCandidate = state.candidate;
            GraphKitDp::Memoization memoization = state.memoization;
            std::vector<int> currentSchedule = memoization.schedule;
            std::vector<int> currentIndegree = memoization.indegree;
            int currentMemory = memoization.current_memory;
            int peakMemory = memoization.peak_memory;

            /*
                Record current running time
            */
            time_t endTime = clock();
            double runningTime = (double)(endTime - startTime) / CLOCKS_PER_SEC;
            if (runningTime > maximumTime)
                return TIMEOUT;

            if (multithreading)
                threads.emplace_back(GraphKitDp::stageCalculation, currentCandidate, currentSchedule, currentIndegree, std::ref(currentMemoization), currentMemory, peakMemory, graph, inSum, outSum, boundPeakMemory);
            else
                GraphKitDp::stageCalculation(currentCandidate, currentSchedule, currentIndegree, currentMemoization, currentMemory, peakMemory, graph, inSum, outSum, boundPeakMemory);
        }

        if (multithreading)
        {
            std::unique_lock<std::mutex> lock(GraphKitDp::mutex);
            GraphKitDp::conditionVariable.wait(lock);

            for (auto &thread : threads)
                if (thread.joinable())
                    thread.join();
        }

        lastMemoization = currentMemoization;
        currentMemoization.clear();
    }

    /*
        compile statistics of results
    */
    std::set<int> finalCandidate;
    GraphKitDp::Memoization finalMemoization = lastMemoization.at(finalCandidate);
    std::vector<int> finalSchedule = finalMemoization.schedule;
    std::memcpy(dpSequence, &finalMemoization.schedule[0], sizeof(finalSchedule[0]) * finalSchedule.size());
    peakMemory = finalMemoization.peak_memory;
    return SOLUTION;
}

void GraphKit::runDp(bool multithreading, bool bound, bool softBudget, int calculation)
{
    dpSequence = new int[graph.getNumNodes()];

    int currentMemory = 0;
    int peakMemory = 0;

    time_t startTime = clock();
    if (!softBudget)
    {
        dp(currentMemory, peakMemory, multithreading, bound, calculation);
    }
    else
    {
        int FLAG = NO_SOLUTION;
        int budgetLast = memory;
        int budgetCurrent = budgetLast;
        time_t startTimeSoftBudget = clock();
        FLAG = dpSoftBudget(currentMemory, peakMemory, multithreading, bound, calculation, budgetCurrent, startTimeSoftBudget);

        /*
            Execute dp with soft budget (binary search)
        */

        int iteration = 0;
        while (FLAG != SOLUTION && iteration++ < softBudgetBinarySearchIteration)
        {
            if (FLAG == TIMEOUT)
            {
                budgetLast = budgetCurrent;
                budgetCurrent = budgetLast / 2;
            }
            if (FLAG == NO_SOLUTION)
            {
                budgetCurrent = (budgetLast + budgetCurrent) / 2;
            }

            startTimeSoftBudget = clock();
            FLAG = dpSoftBudget(currentMemory, peakMemory, multithreading, bound, calculation, budgetCurrent, startTimeSoftBudget);
        }
    }
    time_t endTime = clock();

    dpTime = (double)(endTime - startTime) / CLOCKS_PER_SEC;

    dpMemory = peakMemory;
    if (memory == -1)
    {
        memory = dpMemory;
        memcpy(bestSequence, dpSequence, sizeof(int) * graph.getNumNodes());
    }
    else if (memory > dpMemory)
    {
        memory = dpMemory;
        memcpy(bestSequence, dpSequence, sizeof(int) * graph.getNumNodes());
    }
}

int GraphKit::getDpMemory()
{
    return dpMemory;
}

double GraphKit::getDpTime()
{
    return dpTime;
}

int *GraphKit::getDpSequence()
{
    int *sequence = new int[graph.getNumNodes()];
    std::memcpy(sequence, dpSequence, sizeof(dpSequence[0]) * graph.getNumNodes());
    return sequence;
}

void GraphKit::printDpSequence()
{
    std::cout << "dp sequence: {";
    for (int i = 0; i < graph.getNumNodes(); i++)
    {
        std::cout << *(dpSequence + i);
        if (i != graph.getNumNodes() - 1)
            std::cout << ", ";
    }
    std::cout << "}" << std::endl
              << std::endl;
}

void GraphKit::printDpIterationMemory()
{
    std::cout << "dp iteration memory: {";
    for (int i = 0; i < iterationMemory.size(); i++)
    {
        std::cout << iterationMemory[i];
        if (i != iterationMemory.size() - 1)
            std::cout << ", ";
    }
    std::cout << "}" << std::endl
              << std::endl;
}

void GraphKit::setSoftBudgetBinarySearchIteration(int softBudgetBinarySearchIteration)
{
    this->softBudgetBinarySearchIteration = softBudgetBinarySearchIteration;
}

void GraphKit::setMaximumTime(double maximumTime)
{
    this->maximumTime = maximumTime;
}

int GraphKit::getSoftBudgetBinarySearchIteration()
{
    return softBudgetBinarySearchIteration;
}

double GraphKit::getMaximumTime()
{
    return maximumTime;
}