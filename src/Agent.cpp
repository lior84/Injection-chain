//
// Created by spl211 on 10/11/2020.
//

#include "Agent.h"
#include "Graph.h"
#include "Session.h"

Agent::Agent(){}

Agent::~Agent(){}

Agent *const Agent::clone() const {}


ContactTracer::ContactTracer() {}

ContactTracer::~ContactTracer()  {
    //delete[] this;
}

void ContactTracer::act(Session &session) {
    int nodeInd;
    int currTypeCount = session.getTypeCounter()-1;
    if(currTypeCount-1 > 0 && session.getAgentsType().at(currTypeCount-1)=="V") {
        while (session.getAgentsType().at(currTypeCount) == "V") {
            currTypeCount--;
        }
        currTypeCount++;
        nodeInd = session.getAgents().at(currTypeCount)->getNodeInd();
    }
    else {
        nodeInd = session.dequeueInfected();
    }
    Tree *currTree;
    currTree = currTree->createTree(session, nodeInd);
    int nodeToDelete = currTree->traceTree();
//    if(currTree->getchildren().size() == 0)
//        session.setSimulationCycleForTreeCycle(session.getSimulationCycleForTreeCycle() - 1);
    if (nodeToDelete != -1){
        session.getGraph().deleteAllNeighbours(nodeToDelete);
    }
    session.addAgent(ContactTracer());
    session.setAgentsType("C");

    delete currTree;
}

Agent *const ContactTracer::clone() const {
    return new ContactTracer(*this);
}

int ContactTracer::getNodeInd() {
    return -1;
}

Virus::Virus(int nodeInd) : nodeInd(nodeInd) {}

Virus::~Virus(){
    //delete this;
}

void Virus::act(Session &session) {
    session.enqueueInfected(nodeInd);
    for(int i = 0;  i < session.getInfectedNodes().size(); i++){
        for(int j = 0; j < session.getGraph().getEdges().at(session.getInfectedNodes().at(i)).size(); j++){
            if( session.getGraph().getEdges().at(session.getInfectedNodes().at(i)).at(j) == 1 && !session.isInfected(j) && !session.isInNextInfected(j) && !session.isNextToBeInfectedHistory(j) )
            {
                session.setNextToBeInfectedHistory(j);
                session.setNextToBeInfected(j);
                break;
            }
        }
    }
    if(!session.getNextToBeInfected().empty())
        session.setFlag(true);
    else
        session.setFlag(false);

    while(!session.getNextToBeInfected().empty()) {
        session.addAgent(Virus(session.getNextToBeInfected().at(0)));
        session.setAgentsType("V");
        session.popFromNextToBeInfected();
    }

//    session.enqueueInfected(nodeInd);
//    session.infectNext(nodeInd);
//    if(!session.getNextToBeInfected().empty()) {
//        session.addAgent(Virus(session.getNextToBeInfected().back()));
//        session.popFromNextToBeInfected();
//    }
}

Agent *const Virus::clone() const {
    return new Virus(*this);
}

int Virus::getNodeInd() {return nodeInd;}