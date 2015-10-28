/*
* Edge.h
*/
#ifndef EDGE_H_
#define EDGE_H_
#include <string>
#include "Node.h"

class Node;

// No need to manually create edges. Use Node methods for that
class Edge {
    friend class Node;

    private:
        Node* target;
        Node* source;

    public:
        Node * getTarget();
        Node * getSource();

        Edge(Node * source, Node * target);
};

#endif
