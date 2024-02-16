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

    void workThread(std::set<int> currentCandidate, std::vector<int> currentSchedule, std::vector<int> currentIndegree, std::map<std::set<int>, Memoization> &currentMemoization, int currentMemory, int peakMemory, Graph graph, int *inSum, int *outSum)
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

void GraphKit::dp(int &currentMemory, int &peakMemory)
{
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

        for (it = lastMemoization.begin(); it != lastMemoization.end(); it++)
        {
            std::set<int> currentCandidate = it->first;
            GraphKitDp::Memoization memoization = it->second;
            std::vector<int> currentSchedule = memoization.schedule;
            std::vector<int> currentIndegree = memoization.indegree;
            int currentMemory = memoization.current_memory;
            int peakMemory = memoization.peak_memory;

            threads.emplace_back(GraphKitDp::workThread, currentCandidate, currentSchedule, currentIndegree, std::ref(currentMemoization), currentMemory, peakMemory, graph, inSum, outSum);
        }

        std::unique_lock<std::mutex> lock(GraphKitDp::mutex);
        GraphKitDp::conditionVariable.wait(lock);

        for (auto &thread : threads)
            if (thread.joinable())
                thread.join();

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

void GraphKit::runDp()
{
    dpSequence = new int[graph.getNumNodes()];

    int currentMemory = 0;
    int peakMemory = 0;

    time_t startTime = clock();
    dp(currentMemory, peakMemory);
    time_t endTime = clock();

    dpTime = (double)(endTime - startTime) / CLOCKS_PER_SEC;

    dpMemory = peakMemory;
    if (memory == -1)
        memory = dpMemory;
    else if (memory > dpMemory)
        memory = dpMemory;
}

int GraphKit::getDpMemory()
{
    return dpMemory;
}

double GraphKit::getDpTime()
{
    return dpTime;
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