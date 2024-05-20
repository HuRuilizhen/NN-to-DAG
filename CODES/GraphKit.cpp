#include "GraphKit.h"

#include <iostream>
#include <iomanip>

GraphKit::GraphKit()
{
}

GraphKit::GraphKit(Graph graph)
{
    load(graph);

    /*
    Initialize statistical variables
    */

    maximumPeak = -1;
    memory = -1;

    dfsMemory = -1;
    dfsTime = -1;
    greedyMemory = -1;
    greedyTime = -1;
    dpMemory = -1;
    dpTime = -1;
}

void GraphKit::load(Graph graph)
{
    /*
    Load graph data
    */
    this->graph = graph;

    /*
    Calculate graph data
    */
    inDegree = new int[graph.getNumNodes()];
    outDegree = new int[graph.getNumNodes()];
    inSum = new int[graph.getNumNodes()];
    outSum = new int[graph.getNumNodes()];
    numStartNodes = 0;
    numEndNodes = 0;

    for (int node = 0; node < graph.getNumNodes(); node++)
    {
        inDegree[node] = 0;
        outDegree[node] = 0;
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
            outDegree[from]++;
        }
    }

    for (int node = 0; node < graph.getNumNodes(); node++)
    {
        if (inDegree[node] == 0 && outDegree[node] != 0)
            numStartNodes++;
        if (outDegree[node] == 0 && inDegree[node] != 0)
            numEndNodes++;
    }

    startNodes = new int[numStartNodes];
    endNodes = new int[numEndNodes];

    int startNodesCounter = 0;
    int endNodesCounter = 0;

    for (int node = 0; node < graph.getNumNodes(); node++)
    {
        if (inDegree[node] == 0 && outDegree[node] != 0)
            startNodes[startNodesCounter++] = node;
        if (outDegree[node] == 0 && inDegree[node] != 0)
            endNodes[endNodesCounter++] = node;
    }
}

int GraphKit::getMemory()
{
    return memory;
}

int GraphKit::getmaxcutMemory()
{
    return maxcutMemory;
}

void GraphKit::printResult()
{
    std::cout.setf(std::ios::left);
    std::cout << std::endl;

    std::cout << std::setw(16) << "Algorithm:"
              << std::setw(16) << "Peak Memory:"
              << std::setw(16) << "Time:" << std::endl;
    std::cout << std::endl;

    std::cout << std::setw(16) << "DFS Result" << std::setw(16) << dfsMemory << std::setw(16) << dfsTime << std::endl;
    std::cout << std::setw(16) << "IDFS Result" << std::setw(16) << idfsMemory << std::setw(16) << idfsTime << std::endl;
    std::cout << std::setw(16) << "Greedy Result" << std::setw(16) << greedyMemory << std::setw(16) << greedyTime << std::endl;
    std::cout << std::setw(16) << "DP Result" << std::setw(16) << dpMemory << std::setw(16) << dpTime << std::endl;
    std::cout << std::endl;

    std::cout << std::setw(16) << "Maximum Peak" << std::setw(16) << maximumPeak << std::endl;
    std::cout << std::setw(16) << "Best Peak" << std::setw(16) << memory << std::endl;
    std::cout << std::endl;
}