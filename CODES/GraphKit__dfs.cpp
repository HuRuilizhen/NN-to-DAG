#include "GraphKit.h"
#include<iostream>

void GraphKit::dfs(int node, int &currentMemory, int &peakMemory, int *copyInDegree)
{

    currentMemory = currentMemory + outSum[node];
    if (peakMemory < currentMemory)
        peakMemory = currentMemory; 

    currentMemory = currentMemory - inSum[node];

    for (int edge = graph.getEdgeHead(node); graph.isValid(edge); edge = graph.getEdgeNext(edge))
    {
        int to = graph.getEdgeTo(edge);

        copyInDegree[to]--;
        if (copyInDegree[to] == 0)
            dfs(to, currentMemory, peakMemory, copyInDegree);
    }
}

void GraphKit::runDfs()
{
    int *copyInDegree = new int[graph.getNumNodes()];
    for (int node = 0; node < graph.getNumNodes(); node++)
        copyInDegree[node] = inDegree[node];

    int currentMemory = 0;
    int peakMemory = 0;

    for (int index = 0; index < numStartNodes; index++)
    {
        dfs(startNodes[index], currentMemory, peakMemory, copyInDegree);
    }

    dfsResult = peakMemory;
    if (result == -1)
        result = dfsResult;
    else if (result > dfsResult)
        result = dfsResult;
}

int GraphKit::getDfsResult()
{
    return dfsResult;
}