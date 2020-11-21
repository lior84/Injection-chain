//
// Created by spl211 on 10/11/2020.
//

#include "Session.h"
#include <algorithm>
#include <sstream>
#include <iterator>
#include <iostream>
#include <fstream>
#include <iomanip>

std::string path = "path to json file";
std::string file_name = "json_file_name";

template<typename Base, typename T>
inline bool instanceof(const T*) {
    return is_base_of<Base, T>::value;
}

Session::Session(const std::string &path) : g(std::vector<std::vector<int>>()), simulationCycle(0),typeCounter(0), simulationCycleForCycleTree(0), infectedNodes(std::vector<int>()), nextToBeInfected(std::vector<int>()), agentsType(std::vector<string>()), flag(false) {
    ifstream i(path);
    json j;
    i >> j;

    g = Graph(j["graph"]);
    setTreeType(j["tree"]);

    for(int i = 0; i < j["agents"].size(); i++){
        if( j["agents"].at(i).at(0) == ("V") ){
            Agent* agentV = new Virus(j["agents"].at(i).at(1));
            agents.push_back(agentV);
            agentsType.push_back("V");
        }
        else{
            Agent* agentC = new ContactTracer();
            agents.push_back(agentC);
            agentsType.push_back("C");
        }
    }
}

Session::~Session() {
    for(int i = 0; i < agents.size(); i++){
        delete agents.at(i);
    }
    agents.clear();
    infectedNodes.clear();
    nextToBeInfected.clear();
    nextToBeInfectedHistory.clear();
    agentsType.clear();
    for(int i = 0; i < g.getEdges().size(); i++){
        g.getEdges().at(i).clear();
    }
    g.getEdges().clear();
}

//copy ctor
Session::Session(const Session &other) : g(std::vector<std::vector<int>>()), treeType(other.treeType), agents(), agentsType(), flag() {
    for(int i = 0; i < other.agents.size(); i++){
        Agent* newAgent = other.agents.at(i)->clone();
        agents.push_back( newAgent );
    }
    g = other.g;
    treeType = other.treeType;
    simulationCycle = other.simulationCycle;
    simulationCycleForCycleTree = other.simulationCycleForCycleTree;
    flag = other.flag;

    for(int i = 0; i < other.infectedNodes.size(); i++){
        infectedNodes.push_back( other.infectedNodes.at(i) );
    }

    for(int i = 0; i < other.nextToBeInfected.size(); i++){
        nextToBeInfected.push_back( other.nextToBeInfected.at(i) );
    }

    for(int i = 0; i < other.agentsType.size(); i++){
        agentsType.push_back( other.agentsType.at(i) );
    }
}

/*
 * move ctor
 */
Session::Session(Session&& other) : g(std::vector<std::vector<int>>()), agents(other.agents), simulationCycle(other.simulationCycle), simulationCycleForCycleTree(other.simulationCycleForCycleTree), infectedNodes(other.infectedNodes), nextToBeInfected(other.nextToBeInfected), agentsType(other.agentsType), flag(other.flag){
    g = Graph(other.g);
    other.agents.clear();
    other.infectedNodes.clear();//not sure
    other.nextToBeInfected.clear();//not sure
    other.nextToBeInfectedHistory.clear();
    other.agentsType.clear();
}

//copy assignment operator
const Session & Session::operator=(const Session &other) {
    if(this != &other){
        g = other.g;
        treeType = other.treeType;
        simulationCycle = other.simulationCycle;
        simulationCycleForCycleTree = other.simulationCycleForCycleTree;
        flag = other.flag;

        if(!agents.empty())
            agents.clear();
        for(int i = 0; i < other.agents.size(); i++){
            agents.push_back( other.agents.at(i)->clone() );
        }

        if(!infectedNodes.empty())
            infectedNodes.clear();
        for(int i = 0; i < other.infectedNodes.size(); i++){
            infectedNodes.push_back( other.infectedNodes.at(i) );
        }

        if(!nextToBeInfected.empty())
            nextToBeInfected.clear();
        for(int i = 0; i < other.nextToBeInfected.size(); i++){
            nextToBeInfected.push_back( other.nextToBeInfected.at(i) );
        }

        if(!nextToBeInfectedHistory.empty())
            nextToBeInfectedHistory.clear();
        for(int i = 0; i < other.nextToBeInfectedHistory.size(); i++){
            nextToBeInfectedHistory.push_back( other.nextToBeInfectedHistory.at(i) );
        }

        if(!agentsType.empty())
            agentsType.clear();
        for(int i = 0; i < other.agentsType.size(); i++){
            agentsType.push_back( other.agentsType.at(i) );
        }
    }
    return *this;
}

//move assignment operator
const Session & Session::operator=(Session &&other) {
    g = Graph(other.g);
    treeType = other.treeType;
    simulationCycle = other.simulationCycle;
    simulationCycleForCycleTree = other.simulationCycleForCycleTree;
    flag = other.flag;

    if(!agents.empty())
        agents.clear();
    agents = other.agents;
    other.agents.clear();

    if(!infectedNodes.empty())
        infectedNodes.clear();
    infectedNodes = other.infectedNodes;
    other.infectedNodes.clear();

    if(!nextToBeInfected.empty())
        nextToBeInfected.clear();
    nextToBeInfected = other.nextToBeInfected;
    other.nextToBeInfected.clear();

    if(!nextToBeInfectedHistory.empty())
        nextToBeInfectedHistory.clear();
    nextToBeInfectedHistory = other.nextToBeInfectedHistory;
    other.nextToBeInfectedHistory.clear();

    if(!agentsType.empty())
        agentsType.clear();
    agentsType = other.agentsType;
    other.agentsType.clear();

    return *this;
}

void Session::setTreeType(string typeOf) {
    if(typeOf == "M")
        treeType = MaxRank;
    else if(typeOf == "R")
        treeType = Root;
    else
        treeType = Cycle;
}

TreeType Session::getTreeType() const {return treeType;}

void Session::addAgent(const Agent &agent) {
    Agent* newAgent = agent.clone();
    agents.push_back(newAgent);
}

void Session::setGraph(const Graph &graph) {
    g = Graph(graph);
}

Graph& Session::getGraph() {
    return g;
}

void Session::simulate() {
    agents.at(simulationCycle)->act(*this);
    simulationCycle++;
    typeCounter++;
    int counter = 0;
    do{
        agents.at(simulationCycle)->act(*this);
        typeCounter++;
        if(agentsType.at(typeCounter) == "C"){
            simulationCycleForCycleTree++;
        }
        simulationCycle++;
        if(agentsType.at(typeCounter) == "C"){
            continue;
        }
        counter++;
    }while( flag || virusAgentLeft(typeCounter) );

    createJsonFile("/home/spl211/jsonAns/result.json");
}

bool Session::virusAgentLeft(int typeCounter){
    for (int i = typeCounter; i < agentsType.size(); i++) {
        if (agentsType.at(i) == "V")
            return true;
    }
    return false;
}

void Session::enqueueInfected(int nodeInd) {
    infectedNodes.push_back(nodeInd);
}

void Session::infectNext(int nodeInd) {
    bool isInfected = false;
    for(int i = g.getEdges().size() - 1; i >= 0; i--) {
        if (g.getEdges().at(nodeInd).at(i) == 1) {
            for(int j = 0; j < infectedNodes.size() && !isInfected; j++){
                if(i == infectedNodes.at(j) )
                    isInfected = true;
            }
            if(!isInfected)
                nextToBeInfected.push_back(i);
        }
    }
}

int Session::dequeueInfected() {
    int nodeInd = -1;
    if(!infectedNodes.empty()) {
        nodeInd = infectedNodes.at(infectedNodes.size() - 1);
    }
    return nodeInd;
}

int Session::getSimulationCycle() {
    return simulationCycle;
}

std::vector<int> Session::getNextToBeInfected() {
    return nextToBeInfected;
}

std::vector<int> Session::getInfectedNodes() {
    return infectedNodes;
}

void Session::popFromNextToBeInfected() {
    if(!nextToBeInfected.empty()){
        for(int i = 0; i < nextToBeInfected.size() - 1 ; i++){
            nextToBeInfected.at(i) = nextToBeInfected.at(i+1);
        }
        nextToBeInfected.pop_back();
    }
}

bool Session::notConcluded() {
    for(int i = 0; i < g.getEdges().size(); i++){
        for(int j = 0; j < g.getEdges().at(i).size(); j++){
            if(g.getEdges().at(i).at(j) == 1 && ( isInfected(i) != isInfected(j) ))
                return true;
        }
    }
    return false;
}

bool Session::isInfected(int nodeInd) {
    for (int i = 0; i < infectedNodes.size(); i++) {
        if (infectedNodes.at(i) == nodeInd)
            return true;
    }
    return false;
}

bool Session::isInNextInfected(int nodeInd) {
    for (int i = 0; i < nextToBeInfected.size(); i++) {
        if (nextToBeInfected.at(i) == nodeInd)
            return true;
    }
    return false;
}

void Session::createJsonFile(string path) {
    std::ostringstream vts;
    std::ostringstream infectedJ;

    vts << "[";
    for(int i = 0; i < g.getEdges().size(); i++){
        vts << "[";
        std::copy(g.getEdges().at(i).begin(), g.getEdges().at(i).end()-1,
                  std::ostream_iterator<int>(vts, ", "));
        vts << g.getEdges().at(i).back();
        vts << "],";
    }
    vts << "]";

    infectedJ << "[";
    std::copy(infectedNodes.begin(), infectedNodes.end()-1,
              std::ostream_iterator<int>(infectedJ, ", "));
    infectedJ << infectedNodes.back();
    infectedJ << "]";

    std::ofstream file(path);
    file << json{{"graph", vts.str()}, {"infected", infectedJ.str()}};
}

int Session::getSimulationCycleForTreeCycle() {
    return simulationCycleForCycleTree;
}

void Session::setSimulationCycleForTreeCycle(int simCycle) {
    simulationCycleForCycleTree = simCycle;
}

void Session::setNextToBeInfected(int nodeInd) {
    nextToBeInfected.push_back(nodeInd);
}

void Session::setAgentsType(string type) {
    agentsType.push_back(type);
}

void Session::setFlag(bool state) {
    flag = state;
}

void Session::setNextToBeInfectedHistory(int nodeInd) {
    nextToBeInfectedHistory.push_back(nodeInd);
}

std::vector<int> Session::getNextToBeInfectedHistory() {
    return nextToBeInfectedHistory;
}

bool Session::isNextToBeInfectedHistory(int nodeInd) {
    for (int i = 0; i < nextToBeInfectedHistory.size(); i++) {
        if (nextToBeInfectedHistory.at(i) == nodeInd)
            return true;
    }
    return false;
}

void Session::orderInfectedNodes(int ind) {
    int tmp;
    for(int i = 0; i < infectedNodes.size(); i++){
        if(ind == infectedNodes.at(i)){
            for(int j = infectedNodes.size(); j > i; j-- ){
                tmp = infectedNodes.at(i);
                infectedNodes.at(i) = infectedNodes.at(j);
                infectedNodes.at(j) = tmp;
                i++;
            }
            break;
        }
    }
}

int Session::getTypeCounter() {
    return typeCounter;
}

std::vector<string> Session::getAgentsType(){
    return agentsType;
}

std::vector<Agent *> Session::getAgents() {
    return agents;
}
