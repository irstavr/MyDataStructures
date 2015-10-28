/*
* Edge.cpp
*/

#include "Edge.h"

/////////////////    Edge class    ///////////////////////

Edge::Edge(Node * source, Node * target) {
    this->source = source;
    this->target = target;
}

Node* Edge::getTarget() {
    return target;
}

Node* Edge::getSource() {
    return source;
}
