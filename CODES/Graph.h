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
    /*
    Construction method to initialize the number of nodes and the number of edges
    numNodes: the number of Nodes
    numEdges: the number of Edges
    */
    Graph();
    Graph(int numNodes, int numEdges);

    /*
    Getter of numNodes and numEdges
    */
    int getNumNodes();
    int getNumEdges();

    /*
    Setter of edgeWeight
    */
    void setEdgeWeight(int edge, int newWeight);

    /*
    Used to add a directed edge to the class
    from: id of beginning node of the new edge
    to: id of ending node of the new edge
    weight: the weight of the new edge
    */
    void addEdge(int from, int to, int weight);

    /*
    The following method are used to Traverse all the contiguous edges of a node


    for loop structure will be like:
        for (int edge = getEdgeHead(node); isValid(edge); edge=getEdgeNext(edge))
        {
            int from = node;
            int to = getEdgeTo(edge);
            int weight = getEdgeWeight(edge);
        }

    NOTE that all edges and nodes are numbered from 0
    */
    int getEdgeHead(int node);
    int getEdgeNext(int edge);
    int getEdgeTo(int edge);
    int getEdgeWeight(int edge);
    bool isValid(int edge);
    int getweightfromto(int from, int to);

    /*
    Print information of all edges have been stored
    */
    void print();
};

#endif // __GRAPH_H__