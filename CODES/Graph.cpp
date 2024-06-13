#include "Graph.h"

#include <iostream>

Graph::Graph()
{
    numEdges = 0;
    numNodes = 0;
    heads = nullptr;
    edgeTo = nullptr;
    edgeWeight = nullptr;
    edgeNext = nullptr;
    total = 0;
}

Graph::Graph(int numNodes, int numEdges)
{
    this->numNodes = numNodes;
    this->numEdges = numEdges;
    heads = new int[numNodes];
    edgeTo = new int[numEdges];
    edgeWeight = new int[numEdges];
    edgeNext = new int[numEdges];
    total = 0;

    for (int node = 0; node < numNodes; node++)
    {
        heads[node] = -1;
    }
}

int Graph::getNumNodes()
{
    return numNodes;
}

int Graph::getNumEdges()
{
    return numEdges;
}

void Graph::setEdgeWeight(int edge, int newWeight)
{
    edgeWeight[edge] = newWeight;
}

void Graph::addEdge(int from, int to, int weight)
{
    edgeTo[total] = to;
    edgeWeight[total] = weight;
    edgeNext[total] = heads[from];
    heads[from] = total;
    total++;
}

int Graph::getEdgeHead(int node)
{
    if (total == 0)
        return -1;
    return heads[node];
}

int Graph::getEdgeNext(int edge)
{
    return edgeNext[edge];
}

int Graph::getEdgeTo(int edge)
{
    return edgeTo[edge];
}

int Graph::getEdgeWeight(int edge)
{
    return edgeWeight[edge];
}

bool Graph::isValid(int edge)
{
    if (edge == -1)
        return false;
    return true;
}

void Graph::print()
{
    for (int node = 0; node < numNodes; node++)
    {
        std::cout << "Node " << node << ": ";
        for (int edge = getEdgeHead(node); isValid(edge); edge = getEdgeNext(edge))
        {
            std::cout << "(" << getEdgeTo(edge) << ", " << getEdgeWeight(edge) << ") ";
        }
        std::cout << std::endl;
    }
}
