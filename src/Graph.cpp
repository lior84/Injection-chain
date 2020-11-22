//
// Created by spl211 on 09/11/2020.
//

#include <iostream>
#include"../include/Graph.h"

using namespace std;

Graph::Graph(std::vector<std::vector<int>> matrix) : edges(matrix){}

Graph::~Graph() {
    for(int i = 0; i < edges.size(); i++){
        edges.at(i).clear();
    }
    edges.clear();
}

/*
 * Copy ctor
 * */
Graph::Graph(const Graph &other) {
    std::vector<int> tmp;

    for(int i = 0; i < other.edges.size(); i++){
        for(int j = 0; j < other.edges.at(i).size(); j++) {
            tmp.push_back( other.edges.at(i).at(j) );
        }
        edges.push_back( tmp );
        tmp.clear();
    }
}

/*
 * move ctor
 */
Graph::Graph(Graph&& other) : edges(other.edges) {
    for(int i = 0; i < other.edges.size(); i++){
        other.edges.at(i).clear();
    }
    other.edges.clear();
}

/*
 * copy assignment operator
 */
const Graph& Graph::operator=(const Graph &other) {
    if(this != &other){
        if(!edges.empty()){
            for(int i = 0; i < edges.size(); i++){
                edges.at(i).clear();
            }
            edges.clear();
        }

        std::vector<int> tmp;
        for(int i = 0; i < other.edges.size(); i++){
            for(int j = 0; j < other.edges.at(i).size(); j++) {
                tmp.push_back( other.edges.at(i).at(j) );
            }
            edges.push_back( tmp );
            tmp.clear();
        }
    }
    return *this;
}

/*
 * move assignment operator
 */
const Graph & Graph::operator=(Graph &&other) {
    if(!edges.empty()){
        for(int i = 0; i < edges.size(); i++){
            edges.at(i).clear();
        }
        edges.clear();
    }

    edges = other.edges;

    for(int i = 0; i < other.edges.size(); i++){
        other.edges.at(i).clear();
    }
    other.edges.clear();

    return *this;
}

std::vector<std::vector<int>>& Graph::getEdges() {
    return edges;
}

void Graph::deleteAllNeighbours(int nodeToDelete) {
    for(int i = 0; i < edges.at(nodeToDelete).size(); i++){
        if( edges.at(nodeToDelete).at(i) == 1 )
            edges.at(nodeToDelete).at(i) = 0;
        edges.at(i).at(nodeToDelete) = 0;
    }
}






