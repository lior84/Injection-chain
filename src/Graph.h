#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>


class Graph{
public:
    Graph(std::vector<std::vector<int>> matrix);
    virtual ~Graph();
    Graph(const Graph& other);//copy ctor
    Graph(Graph&& other);//move ctor
    const Graph& operator = (const Graph& other);//copy assignment operator
    const Graph& operator = ( Graph&& other );//move assignment operator

    std::vector<std::vector<int>>& getEdges();
    void deleteAllNeighbours(int nodeToDelete);

private:
    std::vector<std::vector<int>> edges;
};

#endif

