#include "GraphKit.h"

#include <queue>
#include <time.h>
#include <iostream>

void GraphKit::greedy(int &currentMemory, int &peakMemory, int *copyIndegree)
{
    struct Element
    {
        int node;
        int delta;

        bool operator<(const Element other) const
        {
            return this->delta > other.delta;
        }
    };

    std::priority_queue<Element> elementQueue;

    for (int index = 0; index < numStartNodes; index++)
    {
        int node = startNodes[index];
        Element element;
        element.node = node;
        element.delta = outSum[node] - inSum[node];
        elementQueue.push(element);
    }

    int cnt = -1;
    while (!elementQueue.empty())
    {
        Element element = elementQueue.top();
        elementQueue.pop();
        int node = element.node;
        int delta = element.delta;

        *(greedySequence + ++cnt) = node;
        currentMemory += delta;
        if (currentMemory > peakMemory)
            peakMemory = currentMemory;

        for (int edge = graph.getEdgeHead(node); graph.isValid(edge); edge = graph.getEdgeNext(edge))
        {
            int to = graph.getEdgeTo(edge);
            copyIndegree[to]--;
            if (copyIndegree[to] == 0)
            {
                Element element;
                element.node = to;
                element.delta = outSum[to] - inSum[to];
                elementQueue.push(element);
            }
        }
    }
}

void GraphKit::runGreedy()
{
    greedySequence = new int[graph.getNumNodes()];
    int *copyInDegree = new int[graph.getNumNodes()];
    for (int node = 0; node < graph.getNumNodes(); node++)
        copyInDegree[node] = inDegree[node];

    int currentMemory = 0;
    int peakMemory = 0;

    time_t startTime = clock();
    greedy(currentMemory, peakMemory, copyInDegree);
    time_t endTime = clock();

    greedyTime = (double)(endTime - startTime) / CLOCKS_PER_SEC;

    greedyMemory = peakMemory;
    if (memory == -1)
        memory = greedyMemory;
    else if (memory > greedyMemory)
        memory = greedyMemory;
}

int GraphKit::getGreedyMemory()
{
    return greedyMemory;
}

double GraphKit::getGreedyTime()
{
    return greedyTime;
}

void GraphKit::printGreedySequence()
{
    std::cout << "greedy sequence: {";
    for (int i = 0; i < graph.getNumNodes(); i++)
    {
        std::cout << *(greedySequence + i);
        if (i != graph.getNumNodes() - 1)
            std::cout << ", ";
    }
    std::cout << "}" << std::endl
              << std::endl;
}