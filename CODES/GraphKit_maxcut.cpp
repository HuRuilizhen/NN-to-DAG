#include "GraphKit.h"
#include<cstring>
#include <time.h>
#include <queue>
#include <stack>
#include <vector>
#include <algorithm>
#include <iostream>

bool GraphKit::bfs(int s, int t, Graph *newGraph, int *depth){
    std::queue<int> q;
    int depLength = newGraph->getNumNodes();
    for (int i = 0; i < depLength; ++i) {
        depth[i]=0;
    }
    q.push(s);
    depth[s]=1;
    while(!q.empty()){
        int from = q.front();
        q.pop();
        for (int edge = newGraph->getEdgeHead(from); newGraph->isValid(edge); edge = newGraph->getEdgeNext(edge)){
            int to = newGraph->getEdgeTo(edge);
            int weight = newGraph->getEdgeWeight(edge);
            if(weight>0&&depth[to]==0){
                depth[to] = depth[from]+1;
                q.push(to); 
            }
        }
    }
    if(depth[t])return 1;
    return 0;
}

long long GraphKit::dinic(int node, int flow, int s, int t, Graph *newGraph, int *depth){
    if (node == t)return flow;
    long long rest = 0;

    for (int edge = newGraph->getEdgeHead(node); newGraph->isValid(edge); edge = newGraph->getEdgeNext(edge))
    {
        if(flow<=0)break;
        int from = node;
        int to = newGraph->getEdgeTo(edge);
        int weight = newGraph->getEdgeWeight(edge);

        if(weight>0&&depth[to]==depth[from]+1){
            int flowmin = dinic(to, std::min(flow, weight), s, t, newGraph, depth);
            newGraph->setEdgeWeight(edge,weight-flowmin);
            newGraph->setEdgeWeight(edge^1,newGraph->getEdgeWeight(edge ^ 1)+flowmin);
            flow-=flowmin; 
            rest+=flowmin; 
        }
    }
    return rest;
}

void *GraphKit::getFlowGraph(Graph* newgraph, int currentNode, int endNode, std::vector<bool>& visited, std::vector<int>& path, long long& totalWeight)
{
    visited[currentNode] = true;
    path.push_back(currentNode);

    if (currentNode == endNode) {
        // find the path from the start point to the end point and update the weights
        // std::cout << "Path found: ";
        for (int i = 0; i < path.size() - 1; ++i) {
            int fromNode = path[i];
            int toNode = path[i + 1];

            // 更新路径上的每条边的权重
            for (int edge = newgraph->getEdgeHead(fromNode); newgraph->isValid(edge); edge = newgraph->getEdgeNext(edge)) {
                int to = newgraph->getEdgeTo(edge);
                int weight = newgraph->getEdgeWeight(edge);
                if (to == toNode) {
                    newgraph->setEdgeWeight(edge, weight + totalWeight);
                    break;
                }
            }
        }
        // for (int node : path) {
        //     std::cout << node << " ";
        // }
        // std::cout << std::endl;
    } 
    else {
        for (int edge = newgraph->getEdgeHead(currentNode); newgraph->isValid(edge); edge = newgraph->getEdgeNext(edge)) {
            int to = newgraph->getEdgeTo(edge);
            int weight = newgraph->getEdgeWeight(edge);

            if (!visited[to]) {
                getFlowGraph(newgraph, to, endNode, visited, path, totalWeight);
            }
        }
    }

    // backtracking
    visited[currentNode] = false;
    path.pop_back();
}

void GraphKit::runMaxcut(){
    int inf = 0x7fffffff;
    long long fmax = 1;
    int numNodes = graph.getNumNodes();
    int numEdges = graph.getNumEdges();
    int s = graph.getNumNodes();
    int t = graph.getNumNodes() + 1;
    for (int node = 0; node < numNodes; node++)
    {
        for (int edge = graph.getEdgeHead(node); graph.isValid(edge); edge = graph.getEdgeNext(edge))
        {
            fmax += graph.getEdgeWeight(edge);
        }
    }
    Graph *newgraph = new Graph(numNodes + 2, numEdges  + (numStartNodes + numEndNodes));
    Graph tempgraph = Graph(numNodes + 2, 2*numEdges  + 2*(numStartNodes + numEndNodes));
    for (int node = 0; node < numNodes; node++)
    {
        for (int edge = graph.getEdgeHead(node); graph.isValid(edge); edge = graph.getEdgeNext(edge))
        {
            newgraph->addEdge(node, graph.getEdgeTo(edge), 0);
            tempgraph.addEdge(node, graph.getEdgeTo(edge), graph.getEdgeWeight(edge));
        }
    }

    for (int index = 0; index < numStartNodes; index++)
    {
        int startNode = startNodes[index];
        newgraph->addEdge(s, startNode, 0);
        tempgraph.addEdge(s, startNode, 0);
    }

    for (int index = 0; index < numEndNodes; index++)
    {
        int endNode = endNodes[index];
        newgraph->addEdge(endNode, t, 0);
        tempgraph.addEdge(endNode, t, 0);
    }
    std::vector<bool> visited(numNodes, false);
    std::vector<int> path;
    long long totalWeight = fmax;
    getFlowGraph(newgraph,s,t,visited,path,totalWeight);
    long long flowsum = 0;
    for (int edge = newgraph->getEdgeHead(s); newgraph->isValid(edge); edge = newgraph->getEdgeNext(edge))
        {
            flowsum+=newgraph->getEdgeWeight(edge);
        }
    // std::cout<<flowsum<<std::endl;

    Graph *g_plus = new Graph(numNodes + 2, 2*numEdges  + 2*(numStartNodes + numEndNodes));
    for (int node = 0; node < numNodes+2; node++)
    {
        for (int edge = newgraph->getEdgeHead(node); newgraph->isValid(edge); edge = newgraph->getEdgeNext(edge))
        {
            g_plus->addEdge(node, newgraph->getEdgeTo(edge), newgraph->getEdgeWeight(edge)-tempgraph.getEdgeWeight(edge));
            g_plus->addEdge(newgraph->getEdgeTo(edge), node, 0);
        }
    }
    
    long long maxflow=0;
    int *depth = new int[g_plus->getNumNodes()];
    while(bfs(s, t, g_plus, depth)){
        while(long long d = dinic(s,inf,s,t,g_plus,depth)){
            maxflow+=d;
        }
    }
    
    // std::cout<<maxflow<<std::endl;
    Graph residual_network(numNodes + 2, 2 * numEdges  + 2 * (numStartNodes + numEndNodes));
    
    for (int node = 0; node < numNodes+2; node++)
    {
        for (int edge = newgraph->getEdgeHead(node); newgraph->isValid(edge); edge = newgraph->getEdgeNext(edge))
        {
            if(g_plus->getweightfromto(newgraph->getEdgeTo(edge),node)<newgraph->getEdgeWeight(edge)-tempgraph.getEdgeWeight(edge)){
                residual_network.addEdge(node, newgraph->getEdgeTo(edge), g_plus->getweightfromto(newgraph->getEdgeTo(edge),node));
            }
        }
    }
    
    std::vector<int> S;
    int reached[residual_network.getNumNodes()] = {0};
    std::stack<int> nodeStack;
    nodeStack.push(s);
    while(!nodeStack.empty()){
        int currentNode = nodeStack.top();
        nodeStack.pop();

        if(reached[currentNode]==0){
            reached[currentNode]=1;
            S.push_back(currentNode);

            for (int edge = residual_network.getEdgeHead(currentNode); residual_network.isValid(edge); edge = residual_network.getEdgeNext(edge))
            {
                int nextNode = residual_network.getEdgeTo(edge);
                if (reached[nextNode]==0){
                    nodeStack.push(nextNode);
                }
            }
        }
    }
   
    S.erase(S.begin());
    topological_cut = S;
    // std::cout<<S.size()<<std::endl;
    // for(int i : S){
    //     std::cout<<i<< " ";
    // }
    // std::cout<<std::endl;
    int maxcut = 0;
    for(int node : S){
        for (int edge = graph.getEdgeHead(node); graph.isValid(edge); edge = graph.getEdgeNext(edge))
        {
            int to = graph.getEdgeTo(edge);
            auto it = std::find(S.begin(), S.end(), to);
            bool inT;
            if (it != S.end()) {
                inT = false;
            } else {
                inT = true;
            }
            if(inT){
                maxcut+=graph.getEdgeWeight(edge);
            }
        }
        
    }

    maxcutMemory = maxcut;

}

std::vector<int> GraphKit::RESPECTORDER(std::vector<int> sequential_schedule) {
    std::vector<int> S = topological_cut;
    std::vector<int> T;
    
    for (int i = 0; i<graph.getNumNodes(); i++){
        if (std::find(S.begin(), S.end(), i) == S.end()) { 
            T.push_back(i);
        }
    }
    std::vector<int> result;
    result.push_back(0);
    result.push_back(0);
    for (int j = 0; j<sequential_schedule.size()-1; j++){
        bool inT=false;
        for (int nodej : T){
            if(sequential_schedule[j]==nodej){
                inT = true;
                break;
            }
        }
        // std::cout<<inT<<std::endl;
        if(inT){
            for (int i = j+1; i<sequential_schedule.size(); i++){
                bool inS=false;
                for (int nodei : S){
                    if(sequential_schedule[i]==nodei){
                        inS = true;
                        break;
                    }
                }
                if(inS){
                    if((i-j)>(result[1]-result[0])){
                        result[1]=i;
                        result[0]=j;
                    }
                }
            }
        }
    }
    return result;
}

// int main(int argc, char* argv[]) {
//     int n, m;
//     std::cin >> n >> m;
//     Graph d(n, m);
//     std::vector<std::vector<int>> mat;
//     mat.assign(n, std::vector<int>(n, 0));
//     for (int i = 0; i < m; i++)
//     {
//         int u, v, w;
//         std::cin >> u >> v >> w;
//         if(mat[u][v]==0){
//             d.addEdge(u, v, w);
//             mat[u][v]=1;
//         }
//         // d.addEdge(u, v, w);
//     }

//     GraphKit k(d);
//     k.runMaxcut();
//     std::cout<<"memory using maxtopcut: "<<k.getmaxcutMemory();

// }

