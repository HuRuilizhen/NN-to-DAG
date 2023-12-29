#include <iostream>
#include <time.h>
#include "Graph.h"
#include "GraphKit.h"

int main(int argc, char const *argv[])
{
    int n, m;

    time_t startTime = clock();
    std::cin >> n >> m;
    Graph graph(n, m);

    for (int i = 0; i < m; i++)
    {
        int u, v, w;
        std::cin >> u >> v >> w;
        graph.addEdge(u, v, w);
    }
    time_t endTime = clock();

    std::cout << "Loading Time:\t" << (double)(endTime - startTime) / CLOCKS_PER_SEC << std::endl;

    GraphKit graphKit(graph);
    graphKit.runDfs();
    graphKit.runIdfs();
    graphKit.runGreedy();
    graphKit.printResult();
    graphKit.printDfsSequence();
    graphKit.printIdfsSequence();
    graphKit.printGreedySequence();

    return 0;
}
