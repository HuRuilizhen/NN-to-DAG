#ifndef __GRAPHKIT_H__
#define __GRAPHKIT_H__

#include "Graph.h"

class GraphKit
{
private:
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

    int maxcutMemory;

    int memory;

    void load(Graph graph);

    /*
    the methods of maxflow to get maximum peak memory
    */

    /*
    algorithms to get the peak memory
    */
    void dfs(int node, int &currentMemory, int &peakMemory, int *copyIndegree, int &cnt);
    void idfs(int node, int &currentMemory, int &peakMemory, int *copyIndegree, int &cnt, Graph *newGraph);
    void greedy(int &currentMemory, int &peakMemory, int *copyIndegree);
    void dp(int &currentMemory, int &peakMemory, bool multithreading, int calculation);

public:
    /*
    construction method to initialize graph date of GraphKit
    */
    GraphKit();
    GraphKit(Graph graph);

    /*
    the method to run algorithms and recode the results of the peak memory
    */
    void runDfs();
    void runIdfs();
    void runGreedy();
    void runDp(bool multithreading, int calculation);

    /*
    the method to get or print the result
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

    int getmaxcutMemory();

    int getMemory();
    void printResult();
    void printResultCSV();
};

#endif // __GRAPHKIT_H__