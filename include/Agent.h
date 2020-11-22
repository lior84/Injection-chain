#ifndef AGENT_H_
#define AGENT_H_

#include <vector>

class Session;

class Agent{
public:
    Agent();
    virtual ~Agent();
    virtual Agent *const clone()const = 0;
    virtual void act(Session& session) = 0;
    virtual int getNodeInd()=0;//not surely needed
};

class ContactTracer: public Agent{
public:
    ContactTracer();
    virtual ~ContactTracer();
    virtual Agent *const clone()const;
    virtual void act(Session& session);
    virtual int getNodeInd();//not surely needed
};

class Virus: public Agent{
public:
    Virus(int nodeInd);
    virtual ~Virus();
    virtual Agent *const clone()const;
    virtual void act(Session& session);
    virtual int getNodeInd();//not surely needed
private:
    const int nodeInd;
};

#endif
