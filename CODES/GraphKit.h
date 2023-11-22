#ifndef __GRAPHKIT_H__
#define __GRAPHKIT_H__

#include "Graph.h"

class GraphKit
{
private:
    Graph *graph;
    int *inDegree;
    int *inSum;
    int *outSum;
    int *startNodes;
    int numStartNodes;

    int dfsResult;
    int result;

    void dfs(int node, int &currentMemory, int &peakMemory, bool *visit, int *copyIndegree);

public:
    GraphKit();
    GraphKit(Graph graph);

    void load(Graph graph);
    void runDfs();

    int getDfsResult();
    int getResult();
    void printResult();
};

#endif // __GRAPHKIT_H__