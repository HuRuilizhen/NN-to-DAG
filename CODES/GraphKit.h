#ifndef __GRAPHKIT_H__
#define __GRAPHKIT_H__

#include "Graph.h"
#include <vector>

class GraphKit
{
private:
    const int TIMEOUT = -1;
    const int NO_SOLUTION = 0;
    const int SOLUTION = 1;
    int softBudgetBinarySearchIteration = 20;
    double maximumTime = 60.0;

    Graph graph;
    int *inDegree;
    int *outDegree;
    int *inSum;
    int *outSum;
    int *startNodes;
    int *endNodes;
    int numStartNodes;
    int numEndNodes;

    int maximumPeak;

    int dfsMemory;
    double dfsTime;
    int *dfsSequence;

    int idfsMemory;
    double idfsTime;
    int *idfsSequence;

    int greedyMemory;
    double greedyTime;
    int *greedySequence;

    int dpMemory;
    double dpTime;
    int *dpSequence;

    std::vector<int> topological_cut; // topological cut C = (S,T). topological_cut contains the nodes of S
    int maxcutMemory;
    int memory;

    void load(Graph graph);

    /*
    the methods of maxflow to get maximum peak memory
    */
    bool bfs(int s, int t, Graph *newGraph, int *depth);
    void *getFlowGraph(Graph *newgraph, int currentNode, int endNode, std::vector<bool> &visited, std::vector<int> &path, long long &totalWeight);
    long long dinic(int node, int flow, int s, int t, Graph *newGraph, int *depth);

    /*
    algorithms to get the peak memory
    */
    void dfs(int node, int &currentMemory, int &peakMemory, int *copyIndegree, int &cnt);
    void idfs(int node, int &currentMemory, int &peakMemory, int *copyIndegree, int &cnt, Graph *newGraph);
    void greedy(int &currentMemory, int &peakMemory, int *copyIndegree);
    void dp(int &currentMemory, int &peakMemory, bool multithreading, bool bound, int calculation);
    int dpSoftBudget(int &currentMemory, int &peakMemory, bool multithreading, bool bound, int calculation, int budget, time_t startTime);

public:
    /*
    construction method to initialize graph date of GraphKit
    */
    GraphKit();
    GraphKit(Graph graph);

    /*
    the setter and getter methods of parameters
    */
    void setSoftBudgetBinarySearchIteration(int softBudgetBinarySearchIteration);
    void setMaximumTime(double maximumTime);
    int getSoftBudgetBinarySearchIteration();
    double getMaximumTime();

    /*
    the methods to run algorithms and recode the results of the peak memory
    */
    void runDfs();
    void runIdfs();
    void runGreedy();
    void runDp(bool multithreading, bool bound, bool softBudget, int calculation);
    void runMaxcut();

    /*
    the methods to get or print the result
    */
    int getDfsMemory();
    double getDfsTime();
    void printDfsSequence();

    int getIdfsMemory();
    double getIdfsTime();
    void printIdfsSequence();

    int getGreedyMemory();
    double getGreedyTime();
    void printGreedySequence();

    int getDpMemory();
    double getDpTime();
    void printDpSequence();

    int getMaxcutMemory();
    std::vector<int> RESPECTORDER(std::vector<int> sequential_schedule);

    int getMemory();
    void printResult();
    void printResultCSV();
};

#endif // __GRAPHKIT_H__