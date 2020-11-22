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
    int nodeInd= session.dequeueInfected();

    Tree *currTree;
    currTree = currTree->createTree(session, nodeInd);
    int nodeToDelete = currTree->traceTree();

    if (nodeToDelete != -1){
        session.getGraph().deleteAllNeighbours(nodeToDelete);
    }

    delete currTree;
}

Agent *const ContactTracer::clone() const {
    return new ContactTracer(*this);
}

int ContactTracer::getNodeInd() {return -1;}

Virus::Virus(int nodeInd) : nodeInd(nodeInd) {}

Virus::~Virus(){}

void Virus::act(Session &session) {
    int newVirusesCount = 0;
    session.enqueueInfected(nodeInd);
    for(int i = 0;  i < session.getInfectedNodes().size(); i++){
        for(int j = 0; j < session.getGraph().getEdges().at(session.getInfectedNodes().at(i)).size(); j++){
            if( session.getGraph().getEdges().at(session.getInfectedNodes().at(i)).at(j) == 1 && !session.isInfected(j) && !session.isInNextInfected(j) && !session.isNextToBeInfectedHistory(j) )
            {
                session.setNextToBeInfectedHistory(j);
                session.setNextToBeInfected(j);
                newVirusesCount++;
                break;
            }
        }
    }

    int loopCount = 0;
    while(!session.getNextToBeInfected().empty()) {
        session.addAgent(Virus(session.getNextToBeInfected().at(0)));
        session.setAgentsType("V");
        if(loopCount==0) {
            session.addAgent(ContactTracer());
            session.setAgentsType("C");
        }
        newVirusesCount++;
        session.popFromNextToBeInfected();
        loopCount++;
    }
}

Agent *const Virus::clone() const {return new Virus(*this);}

int Virus::getNodeInd() {return nodeInd;}