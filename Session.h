#ifndef SESSION_H_
#define SESSION_H_

#include <vector>
#include <string>
#include "Graph.h"

#include "Tree.h"
#include "Agent.h"

#include "json.hpp"
#include<iostream>
#include<fstream>
#include "string"

using json = nlohmann::json;
using namespace std;


class Agent;

enum TreeType{
  Cycle,
  MaxRank,
  Root
};

class Session{
public:
    Session(const std::string& path);
    Session(const Session& other);//copy ctor
    Session(Session&& other);//move ctor
    const Session& operator = (const Session& other);//copy assignment operator
    const Session& operator = ( Session&& other );//move assignment operator
    virtual ~Session();

    void simulate();
    void addAgent(const Agent& agent);
    void setGraph(const Graph& graph);
    
    void enqueueInfected(int);
    int dequeueInfected();
    TreeType getTreeType() const;

    void setTreeType(string typeOf);

private:
    Graph g;
    TreeType treeType;
    std::vector<Agent*> agents;
};

#endif
