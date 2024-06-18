#include "GraphKit.h"

#include <time.h>
#include <iostream>

void GraphKit::dfs(int node, int &currentMemory, int &peakMemory, int *copyInDegree, int &cnt)
{
    *(dfsSequence + cnt) = node;
    currentMemory += outSum[node] - inSum[node];
    if (peakMemory < currentMemory)
        peakMemory = currentMemory;

    for (int edge = graph.getEdgeHead(node); graph.isValid(edge); edge = graph.getEdgeNext(edge))
    {
        int to = graph.getEdgeTo(edge);

        copyInDegree[to]--;
        if (copyInDegree[to] == 0)
            dfs(to, currentMemory, peakMemory, copyInDegree, ++cnt);
    }
}

void GraphKit::runDfs()
{
    dfsSequence = new int[graph.getNumNodes()];
    int *copyInDegree = new int[graph.getNumNodes()];
    for (int node = 0; node < graph.getNumNodes(); node++)
        copyInDegree[node] = inDegree[node];

    int currentMemory = 0;
    int peakMemory = 0;
    int cnt = -1;

    time_t startTime = clock();
    for (int index = 0; index < numStartNodes; index++)
    {
        dfs(startNodes[index], currentMemory, peakMemory, copyInDegree, ++cnt);
    }
    time_t endTime = clock();
    dfsTime = (double)(endTime - startTime) / CLOCKS_PER_SEC;
    dfsMemory = peakMemory;

    if (memory == -1)
    {
        memory = dfsMemory;
        memcpy(bestSequence, dfsSequence, sizeof(int) * graph.getNumNodes());
    }
    else if (memory > dfsMemory)
    {
        memory = dfsMemory;
        memcpy(bestSequence, dfsSequence, sizeof(int) * graph.getNumNodes());
    }
}

int GraphKit::getDfsMemory()
{
    return dfsMemory;
}

double GraphKit::getDfsTime()
{
    return dfsTime;
}

int *GraphKit::getDfsSequence()
{
    int *sequence = new int[graph.getNumNodes()];
    memcpy(sequence, dfsSequence, sizeof(int) * graph.getNumNodes());
    return sequence;
}

void GraphKit::printDfsSequence()
{
    std::cout << "dfs sequence: {";
    for (int i = 0; i < graph.getNumNodes(); i++)
    {
        std::cout << *(dfsSequence + i);
        if (i != graph.getNumNodes() - 1)
            std::cout << ", ";
    }
    std::cout << "}" << std::endl
              << std::endl;
}