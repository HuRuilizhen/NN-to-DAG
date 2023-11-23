#ifndef __GRAPHKIT_H__
#define __GRAPHKIT_H__

#include "Graph.h"

class GraphKit
{
private:
    Graph graph;
    int *inDegree;
    int *inSum;
    int *outSum;
    int *startNodes;
    int numStartNodes;

    int dfsResult;
    int greedyResult;
    int result;

    void load(Graph graph);
    void dfs(int node, int &currentMemory, int &peakMemory, int *copyIndegree);
    void greedy(int &currentMemory, int &peakMemory, int *copyIndegree);

public:
    /*
    Construction method to initialize graph date of GraphKit
    */
    GraphKit();
    GraphKit(Graph graph);

    /*
    the method to run DFS and recode the result of the peak memory
    */
    void runDfs();

    /*
    the method to run greedy algorithm and recode the result of the peak memory
    */
    void runGreedy();

    /*
    the method to get or print the result
    */
    int getDfsResult();
    int getGreedyResult();
    int getResult();
    void printResult();
};

#endif // __GRAPHKIT_H__