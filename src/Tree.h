#ifndef ASSIGMENT_1_TREE_H
#define ASSIGMENT_1_TREE_H

#include "Graph.h"
#include <vector>
#include "Session.h"
#include <queue>
#include <iostream>
#include <algorithm>

class Session;

class Tree{
public:
    Tree(int rootLabel);
    virtual ~Tree();
    Tree(const Tree &other);//copy ctor
    Tree(Tree&& other);// move ctor
    Tree& operator = (const Tree& other);//copy assignment operator
    Tree& operator = ( Tree&& other );//move assignment operator

    void deleteAllChildren();
    void addChild(const Tree& child);
    void setDepth(int depth);
    void clear();
    void copyAllChildren(const Tree& other);
    int getDepth();
    int getNode();
    int getChildrenSize();
    static Tree* createTree(const Session& session, int rootLabel);
    static Tree * BFS(Session& session , int rootLabel );
    virtual int traceTree() = 0;
    virtual Tree *const clone()const = 0;
    std::vector<Tree*>& getChildren();

protected:
    std::vector<Tree*> children;
    int node;
    int depth ;
    int nodesCounter;
};

class CycleTree : public Tree{
public:
    CycleTree(int rootLabel, int currCycle);
    virtual int traceTree();
    virtual Tree *const clone()const;
private:
    int currCycle;
};

class MaxRankTree: public Tree{
public:
    MaxRankTree(int rootLabel);
    virtual int traceTree();
    virtual Tree *const clone()const;
};

class RootTree: public Tree{
public:
    RootTree(int rootLabel);
    virtual int traceTree();
    virtual Tree *const clone()const;
};


#endif