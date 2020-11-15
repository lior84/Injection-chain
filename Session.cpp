//
// Created by spl211 on 10/11/2020.
//

#include "Session.h"


Session::Session(const std::string &path) : g(std::vector<std::vector<int>>())  {
        ifstream i(path);
        json j;
        i >> j;

        g = Graph(j["graph"]);

        setTreeType(j["tree"]);

        Agent* agentC = new ContactTracer();
        agents.push_back(agentC);

        Agent* agentV = new Virus(j["agents"].at(0).at(1));
        agents.push_back(agentV);
}

Session::~Session() {
    agents.clear();
}

//copy ctor
Session::Session(const Session &other) : g(std::vector<std::vector<int>>()), treeType(other.treeType), agents() {
    for(int i = 0; i < other.agents.size(); i++){
        //Agent* newAgent = other.agents.at(i)->clone();
        agents.push_back( other.agents.at(i)->clone() );
    }
    g = other.g;
    treeType = other.treeType;
}

/*
 * move ctor
 */
Session::Session(Session&& other) : g(std::vector<std::vector<int>>()), agents(other.agents) {
    g = Graph(other.g);
    other.agents.clear();
}

//copy assignment operator
const Session & Session::operator=(const Session &other) {
    if(this != &other){
        g = other.g;
        treeType = other.treeType;
        if(!agents.empty())
            agents.clear();

        for(int i = 0; i < other.agents.size(); i++){
            agents.push_back( other.agents.at(i)->clone() );
        }
    }
    return *this;
}

//move assignment operator
const Session & Session::operator=(Session &&other) {
    g = Graph(other.g);
    treeType = other.treeType;
    if(!agents.empty())
        agents.clear();

    agents = other.agents;
    other.agents.clear();

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
    agents.push_back(const_cast<Agent *>(&agent));
}