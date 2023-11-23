#include "GraphKit.h"

#include <iostream>

GraphKit::GraphKit()
{
}

GraphKit::GraphKit(Graph graph)
{
    load(graph);
}

void GraphKit::load(Graph graph)
{
    this->graph = graph;
    inDegree = new int[graph.getNumNodes()];
    inSum = new int[graph.getNumNodes()];
    outSum = new int[graph.getNumNodes()];
    numStartNodes = 0;
    result = -1;
    dfsResult = -1;
    greedyResult = -1;

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

int GraphKit::getResult()
{
    return result;
}

void GraphKit::printResult()
{
    std::cout << std::endl;
    std::cout << "DFS Result:\t " << dfsResult << std::endl;
    std::cout << "Greedy Result:\t " << greedyResult << std::endl;
    std::cout << std::endl;
    std::cout << "Result:\t " << result << std::endl;
    std::cout << std::endl;
}