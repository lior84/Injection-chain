#include "../include/Tree.h"

Tree::Tree(int rootLabel):node(rootLabel), children(std::vector<Tree*>()), depth(0), nodesCounter(0) {}

Tree::Tree(const Tree &other):node(other.node), children(std::vector<Tree*>()), depth(other.depth), nodesCounter(other.nodesCounter) {// copy ctor
    copyAllChildren(other);
}

Tree::Tree(Tree &&other) : node(other.node), children(std::vector<Tree*>()), depth(other.depth), nodesCounter(other.nodesCounter) {//move ctor
    for (int i = 0; i < other.children.size(); i++) {
        children.at(i) = other.children.at(i);
        other.children.at(i)= nullptr ;
    }
}

Tree &Tree::operator=(const Tree &other) {//copy assignment operator
    if(this != &other){
        node = other.node ;
        if(!children.empty()){
            children.clear();
        }
        copyAllChildren(other);
    }
    return *this;
}

Tree &Tree::operator=(Tree &&other) {// move assigment operator
    node = other.node;
    if(!children.empty()){
        children.clear();
    }
    for (int i = 0; i < other.children.size(); i++) {
        children.at(i) = other.children.at(i);
        other.children.at(i)= nullptr ;
    }
    return *this ;
}

void Tree::addChild(const Tree &child) {
    Tree *newchild = child.clone();
    children.push_back(newchild);
}

void Tree::deleteAllChildren() {
    delete children.at(0)->children.at(0);

}

Tree::~Tree() {
    if(!children.empty()){
        for(int i = children.size()-1; i >= 0 ; i--){
            delete children.at(i);
            children.pop_back();
        }
    }
}

CycleTree::CycleTree( int rootLabel, int currCycle)  : Tree(rootLabel), currCycle(currCycle){} // constructor for tree (cycle tree class)

Tree *const CycleTree::clone() const {return new CycleTree(node, currCycle);}

int CycleTree::traceTree() {
    Tree* curr = this ;
    while (currCycle>0){
        if(!curr->getChildren().empty()) {
            curr = curr->getChildren().at(0);
        }
        currCycle = currCycle -1 ;
    }
    return curr->getNode();
}

MaxRankTree::MaxRankTree(int rootLabel) : Tree(rootLabel) {} //constructor for tree (ranked tree class)

Tree *const MaxRankTree::clone() const {return new MaxRankTree(node);}

int MaxRankTree::traceTree() {
    int maxVal = 0, maxInd = -1;
    std::queue<Tree*> q;
    std::vector<int> rankCounter;
    for(int i = 0; i < this->nodesCounter; i++){
        rankCounter.push_back(0);
    }
    q.push(this);
    rankCounter.at(q.front()->getNode()) = q.front()->getChildrenSize();

    while ((!q.empty())){
        for(int i = 0; i < q.front()->getChildrenSize(); i++){
            rankCounter.at(q.front()->getChildren().at(i)->getNode()) = q.front()->getChildren().at(i)->getChildrenSize();
            q.push(q.front()->getChildren().at(i));
        }
        q.pop();
    }

    for(int i = 0; i < rankCounter.size(); i++){
        if(rankCounter.at(i) > maxVal) {
            maxVal = rankCounter.at(i);
            maxInd = i;
        }
    }
    return maxInd;
}

RootTree::RootTree(int rootLabel) : Tree(rootLabel) {} //constructor for tree (root tree class)

int RootTree::traceTree() {return node;}

Tree *const RootTree::clone() const {return new RootTree(node);}

void Tree::copyAllChildren(const Tree& other){
    for(auto i : other.children){
        children.push_back(i->clone());
    }
}

Tree *Tree::createTree(const Session &session, int rootLabel) {
    BFS(const_cast<Session &>(session), rootLabel);
}

Tree * Tree::BFS(Session &session, int rootLabel) {
    std::queue<Tree*> q;
    std::vector<bool> visited ;
    visited.reserve(session.getGraph().getEdges().size());
    for(int i = 0 ; i < session.getGraph().getEdges().size();i++){
        visited.push_back(false);
    }

    Tree* newtree;
    if(session.getTreeType() == Cycle){
        newtree = new CycleTree(rootLabel, session.getSimulationCycleForTreeCycle());//check if it works!!
        q.push(newtree);
    }else if(session.getTreeType()== MaxRank){
        newtree = new MaxRankTree(rootLabel);
        q.push(newtree);
    }else{
        newtree = new RootTree(rootLabel);
        q.push(newtree);
    }
    q.front()->nodesCounter = session.getGraph().getEdges().size();
    q.front()->depth = 0;
    visited.at(rootLabel)=true ;
    Tree* newtree1;
    while ((!q.empty())){
        for(int i = 0 ;i<session.getGraph().getEdges().size(); i++){
            if(session.getGraph().getEdges().at(q.front()->node).at(i)==1 && !visited.at(i)){
                if(session.getTreeType() == Cycle){
                    newtree1 = new CycleTree(i,session.getSimulationCycleForTreeCycle());//check if it works!!(need to add private session field)
                    newtree1->setDepth(q.front()->getDepth()+1);
                    q.front()->children.push_back(newtree1);
                    q.push(newtree1);
                }else if(session.getTreeType()== MaxRank){
                    newtree1 = new MaxRankTree(i);
                    newtree1->setDepth(q.front()->getDepth()+1);
                    q.front()->children.push_back(newtree1);
                    q.push(newtree1);
                }else{
                    newtree1 = new RootTree(i);
                    newtree1->setDepth(q.front()->getDepth()+1);
                    q.front()->children.push_back(newtree1);
                    q.push(newtree1);
                }
                visited.at(i)=true ;
            }
        }
        q.pop();
    }

    //delete newtree1;
    newtree1 = nullptr;
    visited.clear();
    return newtree;
}

int Tree::getDepth() {return depth;}

vector<Tree *>& Tree::getChildren() {return children;}

int Tree::getNode() {return node ;}

int Tree::getChildrenSize() {return children.size();}

void Tree::setDepth(int depth_) {depth = depth_;}
