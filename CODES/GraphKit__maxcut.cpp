#include "GraphKit.h"

#include <iostream>
#include <queue>

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

void GraphKit::runMaxcut()
{
    int inf = 0x7fffffff;

    int flowSum = 0;
    for (int node = 0; node < graph.getNumNodes(); node++)
        for (int edge = graph.getEdgeHead(node); graph.isValid(edge); edge = graph.getEdgeNext(edge))
        {
            int weight = graph.getEdgeWeight(edge);
            flowSum += weight;
        }
    flowSum++;

    Graph *newGraph = new Graph(graph.getNumNodes() + 2, 2 * graph.getNumEdges() + 2 * (numStartNodes + numEndNodes));
    int sourceNode = graph.getNumNodes();
    int sinkNode = graph.getNumNodes() + 1;

    for (int index = 0; index < numStartNodes; index++)
    {
        int startNode = startNodes[index];
        newGraph->addEdge(sourceNode, startNode, inf);
        newGraph->addEdge(startNode, sourceNode, 0);
    }

    for (int index = 0; index < numEndNodes; index++)
    {
        int endNode = endNodes[index];
        newGraph->addEdge(endNode, sinkNode, inf);
        newGraph->addEdge(sinkNode, endNode, 0);
    }

    for (int node = 0; node < graph.getNumNodes(); node++)
        for (int edge = graph.getEdgeHead(node); graph.isValid(edge); edge = graph.getEdgeNext(edge))
        {
            int from = node;
            int to = graph.getEdgeTo(edge);
            int weight = graph.getEdgeWeight(edge);
            newGraph->addEdge(from, to, flowSum - weight);
            newGraph->addEdge(to, from, 0);
        }

    int *depth = new int[newGraph->getNumNodes()];
    for (int node = 0; node < newGraph->getNumNodes(); node++)
        depth[node] = 0;

    while (bfs(sourceNode, sinkNode, newGraph, depth))
        while (true)
        {
            int flow = dinic(sourceNode, inf, sourceNode, sinkNode, newGraph, depth);
            if (flow == 0)
                break;
        }

    int maxcut = 0;
    for (int edge = 0; edge < newGraph->getNumEdges(); edge += 2)
    {
        int remainder = newGraph->getEdgeWeight(edge);

        if (remainder)
            continue;

        int contribution = flowSum - newGraph->getEdgeWeight(edge ^ 1);
        std::cout << contribution << std::endl;
        maxcut += contribution;
    }

    maximumPeak = maxcut;
}