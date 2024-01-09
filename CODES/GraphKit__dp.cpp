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
        std::vector<int> schedual;
        int current_memory;
        int peak_memory;
    };

    std::vector<std::map<std::set<int>, Memoization>> memoizations;
    memoizations.resize(graph.getNumNodes() + 1);

    std::set<int> initZeroIndegreeNodes;
    std::vector<int> initSchedual;
    for (int index = 0; index < numStartNodes; index++)
    {
        int startNode = startNodes[index];
        initZeroIndegreeNodes.insert(startNode);
    }
    memoizations[0].emplace(initZeroIndegreeNodes, Memoization{initSchedual, 0, 0});

    for (int i = 0; i < graph.getNumNodes(); i++)
    {
        std::map<std::set<int>, Memoization>::iterator it;
        for (it = memoizations[i].begin(); it != memoizations[i].end(); it++)
        {
            std::set<int> zeroIndegreeNodes = it->first;
            Memoization memoization = it->second;
            std::vector<int> schedual = memoization.schedual;

            for (auto node : zeroIndegreeNodes)
            {
                bool flag = false;
                for (int index = 0; index < schedual.size(); index++)
                    if (node == schedual[index])
                        flag = true;
                if (flag)
                    continue;

                std::set<int> newZeroIndegreeNodes;
                std::vector<int> newSchedual = schedual;
                newSchedual.push_back(node);

                int *copyInDegree = new int[graph.getNumNodes()];
                for (int index = 0; index < graph.getNumNodes(); index++)
                    copyInDegree[index] = inDegree[index];

                for (auto from : newSchedual)
                    for (int edge = graph.getEdgeHead(from); graph.isValid(edge); edge = graph.getEdgeNext(edge))
                    {
                        int to = graph.getEdgeTo(edge);
                        copyInDegree[to]--;
                    }

                for (int from = 0; from < graph.getNumNodes(); from++)
                {

                    if (!copyInDegree[from])
                        newZeroIndegreeNodes.insert(from);
                }

                int new_current_memory = memoization.current_memory - inSum[node] + outSum[node];
                int new_peak_memory = std::max(memoization.peak_memory, new_current_memory);

                if (memoizations[i + 1].find(newZeroIndegreeNodes) == memoizations[i + 1].end())
                {
                    memoizations[i + 1].emplace(newZeroIndegreeNodes, Memoization{newSchedual, new_current_memory, new_peak_memory});
                }

                if (memoizations[i + 1].at(newZeroIndegreeNodes).peak_memory > new_peak_memory)
                {

                    memoizations[i + 1].find(newZeroIndegreeNodes)->second = Memoization{newSchedual, new_current_memory, new_peak_memory};
                }
            }
        }
    }

    std::set<int> finalZeroIndegreeNodes;

    for (int node = 0; node < graph.getNumNodes(); node++)
        finalZeroIndegreeNodes.insert(node);

    peakMemory = memoizations[graph.getNumNodes()].at(finalZeroIndegreeNodes).peak_memory;
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