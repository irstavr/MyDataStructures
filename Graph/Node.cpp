/*
* Node.cpp
*/
#include "Node.h"

//===-----------------------------------------------------------------===//
/////////////////////////////////  Node Class  ////////////////////////////
//===-----------------------------------------------------------------===//

// Constructors / Destructor

// Copy constructor
Node::Node(Node &node) 
{
    label = node.getLabel();
    outEdges = node.getOutEdges();
    inEdges = node.getInEdges();
}

Node::Node(std::string labelName) : label(labelName) 
{
    this->outEdges = new std::vector<Edge*>();
    this->inEdges = new std::vector<Edge*>();
}

Node::Node(std::string label, Node* initialTarget) 
{
    this->outEdges = new std::vector<Edge*>();
    this->inEdges = new std::vector<Edge*>();
    this->label = label;
    this->addTarget(label);
}

Node::~Node() 
{
    int i;
    if (inEdges == NULL) 
    {
        std::cerr << "Inedges null" <<std::endl;
    }

    for(i = 0; i < (int) inEdges->size(); i++) 
    {
        Node* source = (*inEdges)[i]->getSource();
        source->removeOutEdge((*inEdges)[i]);
        delete (*inEdges)[i];
    }

    for(i = 0; i < (int) outEdges->size(); i++) 
    {
        Node * target = (*outEdges)[i]->getTarget();
        target->removeInEdge((*outEdges)[i]);
        delete (*outEdges)[i];
    }
    delete inEdges;
    delete outEdges;
}

//===-----------------------------------------------------------------===//
// Setters / Getters

std::string 
Node::getLabel() 
{
    return label;
}

void 
Node::takeLabel(Node * node) 
{
    this->label = node->getLabel();
}

std::vector<Edge*> * 
Node::getOutEdges() 
{
    return outEdges;
}

std::vector<Edge*> * 
Node::getInEdges() 
{
    return inEdges;
}

std::vector<Node*> * 
Node::getOutNodes() 
{
    std::vector<Node *> * outVertices = new std::vector<Node *>();
    std::vector<Edge *>::iterator outIterator = outEdges->begin();
    for(; outIterator != outEdges->end(); outIterator++) 
    {
        outVertices->push_back((*outIterator)->getTarget());
    }
    return outVertices;
}

//===-----------------------------------------------------------------===//
// Helper methods: add/remove edges

void 
Node::removeInEdge(Edge* edge) 
{
    std::vector<Edge *>::iterator inEdgesIterator = inEdges->begin();
    int i = 0;
    for(; inEdgesIterator != inEdges->end(); inEdgesIterator++, i++) 
    {
        if(*inEdgesIterator == edge) 
        {
            break;
        }
    }

    if(i < (int) inEdges->size())
        inEdges->erase(inEdges->begin() + i);
}

void 
Node::removeOutEdge(Edge * edge) 
{
    std::vector<Edge *>::iterator outEdgesIterator = outEdges->begin();
    int i = 0;
    for(; outEdgesIterator != outEdges->end(); outEdgesIterator++, i++) 
    {
        if(*outEdgesIterator == edge)
            break;
    }

    if(i < (int) outEdges->size())
        outEdges->erase(outEdges->begin() + i);
}

void 
Node::addTarget(Node* targetNode) 
{
    Edge* newEdge = new Edge(this, targetNode);

    if (!alreadyHasEdge(targetNode)) 
    {
        this->outEdges->push_back(newEdge);
        targetNode->getInEdges()->push_back(newEdge);
    }
}

void
Node::addTarget(std::string targetVar) 
{
    Node * targetNode = new Node(targetVar);
    this->addTarget(targetNode);
}

// given another Node, this method copies every edge outgoing from
// the other Node and adds to this one
void 
Node::addTargetsOfOther(Node * otherNode) 
{
    std::vector<Edge*> copyEdges = *(otherNode->getOutEdges());
    for (int i = 0; i < (int) copyEdges.size(); i++) 
    {
        Edge* edgeToCopy = copyEdges[i];
        this->addTarget(edgeToCopy->getTarget());
    }
}

// check if a Node has an edge to the input target Node
// (used so don't add duplicates in addTargetsOfOther
bool 
Node::alreadyHasEdge(Node * targetNode) 
{
    std::vector<Node *> outVertices =  *(this->getOutNodes());
    std::vector<Node *>::iterator it = outVertices.begin();
    for (; it != outVertices.end(); it++) 
    {
        if (*it == targetNode) 
        {
            return true;
        }
    }
    return false;
}

std::string 
Node::toString() 
{
    return label;
}
