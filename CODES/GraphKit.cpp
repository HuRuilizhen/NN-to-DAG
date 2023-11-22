#include "GraphKit.h"

#include <iostream>

GraphKit::GraphKit()
{
}

GraphKit::GraphKit(Graph graph)
{
    load(graph);
}

void GraphKit::dfs(int node, int &currentMemory, int &peakMemory, bool *visit, int *copyInDegree)
{
    visit[node] = true;

    currentMemory = currentMemory - inSum[node] + outSum[node];
    if (peakMemory < currentMemory)
        peakMemory = currentMemory;

    for (int edge = graph->getEdgeHead(node); graph->isValid(edge); edge = graph->getEdgeNext(edge))
    {

        int from = node;
        int to = graph->getEdgeTo(edge);

        if (visit[to])
            continue;

        copyInDegree[to]--;
        if (copyInDegree[to] == 0)
            dfs(to, currentMemory, peakMemory, visit, copyInDegree);
    }
}

void GraphKit::load(Graph graph)
{
    this->graph = new Graph(graph);
    inDegree = new int[graph.getNumNodes()];
    inSum = new int[graph.getNumNodes()];
    outSum = new int[graph.getNumNodes()];
    numStartNodes = 0;
    result = -1;
    dfsResult = -1;

    for (int node = 0; node < graph.getNumNodes(); node++)
    {
        inDegree[node] = 0;
        inSum[node] = 0;
        outSum[node] = 0;
    }

    for (int node = 0; node < graph.getNumNodes(); node++)
    {
        for (int edge = graph.getEdgeHead(node); graph.isValid(edge); edge = graph.getEdgeNext(edge))
        {

            int from = node;
            int to = graph.getEdgeTo(edge);
            int weight = graph.getEdgeWeight(edge);

            inSum[to] += weight;
            outSum[from] += weight;
            inDegree[to]++;
        }
    }

    for (int node = 0; node < graph.getNumNodes(); node++)
        if (inDegree[node] == 0)
            numStartNodes++;

    startNodes = new int[numStartNodes];
    int counter = 0;

    for (int node = 0; node < graph.getNumNodes(); node++)
        if (inDegree[node] == 0)
            startNodes[counter++] = node;
}

void GraphKit::runDfs()
{
    int *copyInDegree = new int[graph->getNumNodes()];
    bool *visit = new bool[graph->getNumNodes()];
    for (int node = 0; node < graph->getNumNodes(); node++)
    {
        copyInDegree[node] = inDegree[node];
        visit[node] = false;
    }

    int currentMemory = 0;
    int peakMemory = 0;

    for (int index = 0; index < numStartNodes; index++)
    {
        dfs(startNodes[index], currentMemory, peakMemory, visit, copyInDegree);
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
int GraphKit::getResult()
{
    return result;
}
void GraphKit::printResult()
{
    std::cout << "DFS Result: " << dfsResult << std::endl;
    std::cout << "Result: " << result << std::endl;
}