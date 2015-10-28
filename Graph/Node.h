/*
 * Node.h
**/
#ifndef NODE_H_
#define NODE_H_

#include "llvm/Support/raw_ostream.h"
#include <string.h>
#include <iostream>
#include <map>
#include <cstdlib>
#include <queue>
#include <sstream>
#include <set>
#include <vector>
#include <string>
#include "Edge.h"

class Edge;

/////////////////    Node Class   //////////////////////

class Node {
    private:
        std::string label;
        std::vector<Edge*> *outEdges;
        std::vector<Edge*> *inEdges;

    public:
        // copy constructor
        Node(Node &node);

        // make a Node with no outgoing edges
        Node(std::string label);

        // make a Node with an outgoing edge to a target Node
        Node(std::string label, Node* target);

        // deconstructor
        ~Node();

        std::string getLabel();
        void takeLabel(Node* other);

        std::vector<Edge*> *getOutEdges();
        std::vector<Edge*> *getInEdges();

        // Get all the nodes pointed by this Node
        // i.e the nodes the outedges point to
        std::vector<Node*> *getOutNodes();

        void removeInEdge(Edge* edge);
        void removeOutEdge(Edge* edge);

        // add an edge to a pre-constructed target Node
        void addTarget(Node* targetNode);
        // add an edge to a newly constructed Node
        void addTarget(std::string targetVar);
        // given another Node, this method copies every edge outgoing
        // from the other Node and adds to this one
        void addTargetsOfOther(Node* otherNode);

        // check if a Node has an edge to the input target Node
        // (used so don't add duplicates in addTargetsOfOther
        bool alreadyHasEdge(Node* targetNode);

        std::string toString();
};
#endif
