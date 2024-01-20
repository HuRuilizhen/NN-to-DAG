#include "GraphKit.h"

#include <iostream>
#include <queue>
#include <string.h>

bool GraphKit::bfs(int sourceNode, int sinkNode, Graph *newGraph, int *depth)
{
    for (int node = 0; node < newGraph->getNumNodes(); node++)
        depth[node] = 0;

    std::queue<int> nodeQueue;
    nodeQueue.push(sourceNode);
    depth[sourceNode] = 1;

    while (!nodeQueue.empty())
    {
        int node = nodeQueue.front();
        nodeQueue.pop();

        for (int edge = newGraph->getEdgeHead(node); newGraph->isValid(edge); edge = newGraph->getEdgeNext(edge))
        {
            int from = node;
            int to = newGraph->getEdgeTo(edge);
            int weight = newGraph->getEdgeWeight(edge);

            if (depth[to] || weight == 0)
                continue;

            nodeQueue.push(to);
            depth[to] = depth[from] + 1;

            if (to == sinkNode)
                return true;
        }
    }

    return false;
}

int GraphKit::dinic(int node, int flow, int sourceNode, int sinkNode, Graph *newGraph, int *depth)
{
    if (node == sinkNode)
        return flow;

    int rest = flow;

    for (int edge = newGraph->getEdgeHead(node); newGraph->isValid(edge) && rest; edge = newGraph->getEdgeNext(edge))
    {
        int from = node;
        int to = newGraph->getEdgeTo(edge);
        int weight = newGraph->getEdgeWeight(edge);

        if (depth[to] != depth[from] + 1 || weight == 0)
            continue;

        int temp = dinic(to, std::min(rest, weight), sourceNode, sinkNode, newGraph, depth);

        if (temp == 0)
            depth[to] = 0;

        rest -= temp;
        newGraph->setEdgeWeight(edge, newGraph->getEdgeWeight(edge) - temp);
        newGraph->setEdgeWeight(edge ^ 1, newGraph->getEdgeWeight(edge ^ 1) + temp);
    }

    return flow - rest;
}

Graph *GraphKit::getFlowGraph()
{
    int flowSum = 0;
    for (int node = 0; node < graph.getNumNodes(); node++)
        for (int edge = graph.getEdgeHead(node); graph.isValid(edge); edge = graph.getEdgeNext(edge))
        {
            int weight = graph.getEdgeWeight(edge);
            flowSum += weight;
        }
    flowSum++;

    Graph *newGraph = new Graph(graph.getNumNodes() + 2, 2 * graph.getNumEdges() + 2 * (numStartNodes + numEndNodes));
    int *demand = new int[2 * graph.getNumEdges() + 2 * (numStartNodes + numEndNodes)];
    int *newInSum = new int[graph.getNumNodes() + 2];
    int *newOutSum = new int[graph.getNumNodes() + 2];
    int sourceNode = graph.getNumNodes();
    int sinkNode = graph.getNumNodes() + 1;
    memset(demand, 0, sizeof(int) * newGraph->getNumEdges());
    memset(newInSum, 0, sizeof(int) * newGraph->getNumNodes());
    memset(newOutSum, 0, sizeof(int) * newGraph->getNumNodes());

    for (int index = 0; index < numStartNodes; index++)
    {
        int startNode = startNodes[index];
        newGraph->addEdge(sourceNode, startNode, 1);
        newGraph->addEdge(startNode, sourceNode, 0);
        demand[newGraph->getEdgeHead(sourceNode)] = 0;
        demand[newGraph->getEdgeHead(startNode)] = 0;
        newInSum[startNode]++;
        newOutSum[sourceNode]++;
    }

    for (int index = 0; index < numEndNodes; index++)
    {
        int endNode = endNodes[index];
        newGraph->addEdge(endNode, sinkNode, 1);
        newGraph->addEdge(sinkNode, endNode, 0);
        demand[newGraph->getEdgeHead(endNode)] = 0;
        demand[newGraph->getEdgeHead(sinkNode)] = 0;
        newInSum[endNode]++;
        newOutSum[sinkNode]++;
    }

    for (int node = 0; node < graph.getNumNodes(); node++)
        for (int edge = graph.getEdgeHead(node); graph.isValid(edge); edge = graph.getEdgeNext(edge))
        {
            int from = node;
            int to = graph.getEdgeTo(edge);
            int weight = graph.getEdgeWeight(edge);
            newGraph->addEdge(from, to, 1);
            newGraph->addEdge(to, from, 0);
            demand[newGraph->getEdgeHead(from)] = weight;
            demand[newGraph->getEdgeHead(to)] = weight;
            newOutSum[from]++;
            newInSum[to]++;
        }

    /* code for getting weights of new graph*/

    return newGraph;
}

void GraphKit::runMaxcut()
{
    Graph *newGraph = getFlowGraph();
    int *depth = new int[newGraph->getNumNodes()];
    for (int node = 0; node < newGraph->getNumNodes(); node++)
        depth[node] = 0;
    int sourceNode = newGraph->getNumNodes() - 2;
    int sinkNode = newGraph->getNumNodes() - 1;

    int inf = 0x7fffffff;
    while (bfs(sourceNode, sinkNode, newGraph, depth))
        while (true)
        {
            int flow = dinic(sourceNode, inf, sourceNode, sinkNode, newGraph, depth);
            if (flow == 0)
                break;
        }

    int maxcut = 0;
    /* code for getting maxcut */

    maximumPeak = maxcut;
}