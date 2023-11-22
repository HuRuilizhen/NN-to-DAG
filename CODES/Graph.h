#ifndef __GRAPH_H__
#define __GRAPH_H__

class Graph
{
private:
    int numNodes;
    int numEdges;
    int *heads;
    int *edgeTo;
    int *edgeWeight;
    int *edgeNext;
    int total;

public:
    Graph();
    Graph(int numNodes, int numEdges);

    int getNumNodes();
    int getNumEdges();

    void addEdge(int from, int to, int weight);
    int getEdgeHead(int node);
    int getEdgeNext(int edge);
    int getEdgeTo(int edge);
    int getEdgeWeight(int edge);
    bool isValid(int edge);

    void print();
};

#endif // __GRAPH_H__