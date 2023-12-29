#include "GraphKit.h"

#include <time.h>
#include <iostream>

void GraphKit::idfs(int node, int &currentMemory, int &peakMemory, int *copyInDegree, int &cnt, Graph *newGraph)
{
    *(idfsSequence + cnt) = node;
    currentMemory += -outSum[node] + inSum[node];
    if (peakMemory < currentMemory)
        peakMemory = currentMemory;

    for (int edge = newGraph->getEdgeHead(node); newGraph->isValid(edge); edge = newGraph->getEdgeNext(edge))
    {
        int to = newGraph->getEdgeTo(edge);

        copyInDegree[to]--;
        if (copyInDegree[to] == 0)
            idfs(to, currentMemory, peakMemory, copyInDegree, ++cnt, newGraph);
    }
}

void GraphKit::runIdfs()
{
    idfsSequence = new int[graph.getNumNodes()];
    int *copyInDegree = new int[graph.getNumNodes()];
    for (int node = 0; node < graph.getNumNodes(); node++)
        copyInDegree[node] = outDegree[node];

    int currentMemory = 0;
    int peakMemory = 0;
    int cnt = -1;

    Graph *newGraph = new Graph(graph.getNumNodes(), graph.getNumEdges());
    for (int node = 0; node < graph.getNumNodes(); node++)
        for (int edge = graph.getEdgeHead(node); graph.isValid(edge); edge = graph.getEdgeNext(edge))
        {
            int from = node;
            int to = graph.getEdgeTo(edge);
            int weight = graph.getEdgeWeight(edge);
            newGraph->addEdge(to, from, weight);
        }

    time_t startTime = clock();
    for (int index = 0; index < numEndNodes; index++)
    {
        idfs(endNodes[index], currentMemory, peakMemory, copyInDegree, ++cnt, newGraph);
    }
    time_t endTime = clock();

    idfsTime = (double)(endTime - startTime) / CLOCKS_PER_SEC;

    idfsMemory = peakMemory;
    if (memory == -1)
        memory = idfsMemory;
    else if (memory > idfsMemory)
        memory = idfsMemory;
}

int GraphKit::getIdfsMemory()
{
    return idfsMemory;
}

double GraphKit::getIdfsTime()
{
    return idfsTime;
}

void GraphKit::printIdfsSequence()
{
    std::cout << "idfs sequence: {";
    for (int i = 0; i < graph.getNumNodes(); i++)
    {
        std::cout << *(idfsSequence + graph.getNumNodes() - i - 1);
        if (i != graph.getNumNodes() - 1)
            std::cout << ", ";
    }
    std::cout << "}" << std::endl
              << std::endl;
}