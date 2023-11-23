#include "GraphKit.h"

#include <queue>

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
        elementQueue.push({node, outSum[node] - inSum[node]});
    }

    while (!elementQueue.empty())
    {
        Element element = elementQueue.top();
        elementQueue.pop();
        int node = element.node;
        int delta = element.node;

        currentMemory += delta;
        if (currentMemory > peakMemory)
            peakMemory = currentMemory;

        for (int edge = graph.getEdgeHead(node); graph.isValid(edge); edge = graph.getEdgeNext(edge))
        {
            int to = graph.getEdgeTo(edge);

            inDegree[to]--;
            if (inDegree[to] == 0)
                elementQueue.push({to, outSum[to] - inSum[to]});
        }
    }
}

void GraphKit::runGreedy()
{
    int *copyInDegree = new int[graph.getNumNodes()];
    for (int node = 0; node < graph.getNumNodes(); node++)
        copyInDegree[node] = inDegree[node];

    int currentMemory = 0;
    int peakMemory = 0;

    greedy(currentMemory, peakMemory, copyInDegree);

    greedyResult = peakMemory;
    if (result == -1)
        result = greedyResult;
    else if (result > greedyResult)
        result = greedyResult;
}

int GraphKit::getGreedyResult()
{
    return greedyResult;
}