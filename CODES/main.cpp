#include <iostream>
#include "Graph.h"
#include "GraphKit.h"

int main(int argc, char const *argv[])
{
    int n, m;

    std::cin >> n >> m;
    Graph graph(n, m);

    for (int i = 0; i < m; i++)
    {
        int u, v, w;
        std::cin >> u >> v >> w;
        graph.addEdge(u, v, w);
    }

    GraphKit graphKit(graph);
    graphKit.runDfs();
    graphKit.runIdfs();
    graphKit.runIdfs();
    graphKit.runGreedy();
    graphKit.runDp(false, true, false, 500);
    graphKit.printResult();
    return 0;
}
