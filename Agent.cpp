//
// Created by spl211 on 10/11/2020.
//

#include "Agent.h"

Agent::Agent(){}

Agent * Agent::clone() {}



ContactTracer::ContactTracer() {}

void ContactTracer::act(Session &session) {

}

Agent* ContactTracer::clone() {
    return new ContactTracer(*this);
}



Virus::Virus(int nodeInd) : nodeInd(nodeInd) {}

void Virus::act(Session &session) {

}

Agent* Virus::clone() {
    return new Virus(*this);
}