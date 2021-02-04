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
    void enqueueInfected(int nodeInd);
    void popFromNextToBeInfected();
    void orderInfectedNodes(int ind);
    void setTreeType(string typeOf);
    void setSimulationCycleForTreeCycle(int simCycle);
    void setNextToBeInfected(int nodeInd);
    void setNextToBeInfectedHistory(int nodeInd);
    void setFlag(bool state);
    void setAgentsType(string type);

    int dequeueInfected();
    int getSimulationCycle();
    int getSimulationCycleForTreeCycle();
    int getTypeCounter();

    bool virusAgentLeft(int typeCounter);
    bool isInfected(int nodeInd);
    bool isNextToBeInfectedHistory(int nodeInd);
    bool isInNextInfected(int nodeInd);
    bool notConcluded();

    std::vector<int> getInfectedNodes();
    std::vector<int> getNextToBeInfected();
    std::vector<int> getNextToBeInfectedHistory();
    std::vector<string> getAgentType();
    std::vector<string> getAgentsType();
    std::vector<Agent*> getAgents();

    Graph& getGraph();
    TreeType getTreeType() const;


    void deleteNextToBeInfected();

private:
    Graph g;
    TreeType treeType;
    std::vector<Agent*> agents;

    int simulationCycle;
    int typeCounter;
    int simulationCycleForCycleTree;
    std::vector<int> infectedNodes;
    std::vector<int> infectedNodesHistory;
    std::vector<int> nextToBeInfected;
    std::vector<int> nextToBeInfectedHistory;
    std::vector<int> nextToBeUsedByCT;
    std::vector<string> agentsType;

    void createJsonFile(string path);
};

#endif
