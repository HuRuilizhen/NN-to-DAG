#include "GraphKit.h"

#include <ctime>
#include <cstring>
#include <iostream>
#include <set>
#include <vector>
#include <map>
#include <queue>

void GraphKit::dp(int &currentMemory, int &peakMemory)
{
    struct Memoization
    {
        std::vector<int> schedule;
        std::vector<int> indegree;
        int current_memory;
        int peak_memory;
    };

    std::vector<std::map<std::set<int>, Memoization>> memoizations;
    memoizations.resize(graph.getNumNodes() + 1);

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
    memoizations[0].emplace(initCandidate, Memoization{initSchedule, initIndegree, 0, 0});

    /*
        Execute dynamic programming
    */
    for (int i = 0; i < graph.getNumNodes(); i++)
    {
        std::map<std::set<int>, Memoization>::iterator it;
        for (it = memoizations[i].begin(); it != memoizations[i].end(); it++)
        {
            std::set<int> currentCandidate = it->first;
            Memoization memoization = it->second;
            std::vector<int> currentSchedule = memoization.schedule;
            std::vector<int> currentIndegree = memoization.indegree;

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

                int new_current_memory = memoization.current_memory - inSum[node] + outSum[node];
                int new_peak_memory = std::max(memoization.peak_memory, new_current_memory);

                if (memoizations[i + 1].find(newCandidate) == memoizations[i + 1].end())
                    memoizations[i + 1].emplace(newCandidate, Memoization{newSchedule, newIndegree, new_current_memory, new_peak_memory});
                else if (memoizations[i + 1].at(newCandidate).peak_memory > new_peak_memory)
                    memoizations[i + 1].find(newCandidate)->second = Memoization{newSchedule, newIndegree, new_current_memory, new_peak_memory};
            }
        }
    }

    /*
        compile statistics of results
    */
    std::set<int> finalCandidate;
    Memoization finalMemoization = memoizations[graph.getNumNodes()].at(finalCandidate);
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