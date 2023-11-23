# NN-to-DAG

---

### Stages of Project:

- Compute the peak memory through different algorithms
  - Normal DFS
  - Greedy Algorithm
  - Dynamic Progaramming
- Compute the shortest run time under the given constrain of memory

---

### Project Code Structure

- **Graph.h**: The class used to store graphs, using the method of chain forward stars

```c++
class Graph
{
private:

    // Private member

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

    
    /*
    Print information of all edges have been stored
    */
    void print();
};
```
  
- **GraphKit.h**: The class used to calculate peak memory uses a different algorithm

```cpp
class GraphKit
{
private:
    
    // private members

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
```