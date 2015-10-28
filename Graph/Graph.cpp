#include "Graph.h"
#include "llvm/Support/raw_ostream.h"
#include <fstream>
#include <iostream>

//===-----------------------------------------------------------------===//
//////////////////////////    Graph Class   ///////////////////////////////
//===-----------------------------------------------------------------===//

// Graph Constructor
Graph::Graph(void) {
    nodes = new std::vector<Node*>();
    edges = new std::vector<Edge*>();
    nodeMap = new std::map<std::string, Node*>;
}

// Graph copy constructor
Graph::Graph(const Graph &graph) {
    nodes = graph.getNodes();
    edges = graph.getEdges();
    nodeMap = graph.getNodeMap();
}

// Graph destructor
Graph::~Graph(void) {
    delete nodes;
    delete edges;
    delete nodeMap;
}

//===-----------------------------------------------------------------===//
// Methods to create nodes/edges on the graph
// to create a new Node without edges and add it to the graph.
Node* Graph::makeNode(std::string label) {
    std::cerr << "\n" << __FUNCTION__ << std::endl;
    // check if already exists
    Node* newNode = getNodeAtLabel(label);
    if (newNode == NULL) {
        newNode = new Node(label);
        (*nodeMap)[label] = newNode;
        nodes->push_back(newNode);
    }
    return newNode;
}

// to create a new node which is the source. Finds the node corresponding
// to the input target label and attaches the new source node to it.
// Node is added to the graph.
void Graph::makeNode(std::string source, std::string target) {
    Node* targetNode = (*nodeMap)[target];

    // check if source already exists
    Node* sourceNode = getNodeAtLabel(source);
    if (sourceNode == NULL) {
        Node * newNode = new Node(source, targetNode);
        (*nodeMap)[source] = newNode;
        nodes->push_back(newNode);
    }
    else {
        createEdge(sourceNode, targetNode);
    }
}

void Graph::makeNodes(std::string source, std::string target) {
    makeNode(target);
    makeNode(source, target);
}

// given labels corresponding to nodes, this method adds an edge from the
// source to the target
bool Graph::createEdge(std::string sourceVar, std::string targetVar) {
    Node* sourceNode = (*nodeMap)[sourceVar];
    Node* targetNode = (*nodeMap)[targetVar];
    if (sourceNode == NULL || targetNode == NULL) {
        return false;
    }
    createEdge(sourceNode, targetNode);
    return true;
}

// Creates a subtyping edge from src to tgt
// given nodes, this method adds an edge from the source to the target
void Graph::createEdge(Node* src, Node* tgt) {
    assert(src && "No src node");
    assert(tgt && "No tgt node");
    std::cerr << "\n" << __FUNCTION__ << std::endl;

//    if (unionize(src, tgt)) {
//        // the target Node was merged with one of the sources's children
//        return;
//    }
//    else {
        // no merge happened so add the target as normal
        src->addTarget(tgt);
        return;
//    }
}

//===-----------------------------------------------------------------===//
// Removers/Cloners of nodes

void Graph::removeNode(std::string label) {
    Node * thisNode = (*nodeMap)[label];
    if (thisNode == NULL) {
        return;
    } else {
        removeNode(thisNode);
        return;
    }
}

void Graph::removeNode(Node* thisNode) {
    std::size_t i;
    for (i = 0; i < nodes->size(); i++) {
        if ((*nodes)[i] == thisNode) {
            // we found the Node
            break;
        }
    }
    nodes->erase(nodes->begin() + i);
    delete thisNode;
}

// given the label of a current Node and a new label, this method
// constructs a newNode with the same outgoing edges as the old Node.
// Adds to graph
bool Graph::cloneNode(std::string newLabel, std::string oldLabel) {
    Node* oldNode = (*nodeMap)[oldLabel];
    if(oldNode == NULL) {
        return false;
    }
    Node* newNode = makeNode(newLabel);
    std::vector<Edge*> oldOutEdges = *(oldNode->getOutEdges());

    unsigned int i;
    for(i = 0; i < oldOutEdges.size(); i++) {
        Edge currentEdge = *(oldOutEdges[i]);
        newNode->addTarget(currentEdge.getTarget());
    }
    return true;
}

//===----------------------------------------------------------------===//
// Accesses Node

Node* Graph::getNodeAtLabel(std::string label) {
    // note: this may need a safety check since apparently
    // throws an exception
    if (nodeMap->find(label) != nodeMap->end()) {
        return nodeMap->at(label);
    } else {
        return NULL;
    }
    //return (*nodeMap)[label];
}

//===-----------------------------------------------------------------===//

// Given a source Node and a target Node ,
// this method checks to see if the target needs to be unionized with
// any of the source's children (at most one),
// and if so it merges them into the other Node.
// All nodes that point to the target Node will have their target
// updated, and the target Node is removed from the graph
// return true if a merge happened, and false if no merge occurred
bool Graph::unionize(Node* source, Node* target) {
    //TODO:: this is currently segfaulting!!
    std::vector<Node *> children = *(source->getOutNodes());
    for (std::size_t i = 0; i < children.size(); i++) {
        Node * currentChild = children[i];
        merge(currentChild, target);
        return true;        // only possible to merge with one child
    }
    return false;
}

// Given nodes A and B, this method merges Node B into Node A.
// Node A will now have all incoming and outgoing edges that B had.
// B is removed from the graph.
void Graph::merge(Node * A, Node* B) {
    std::cerr << "merge " << A->getLabel() << " and "
                          << B->getLabel() << std::endl;
    addTargetsOfOther(A, B);
    addSourcesOfOther(A,B);
    takeLabels(A,B);
    std::cerr << " removing " << B->getLabel() << std::endl;
    removeNode(B);
    return;
}

// given vertixes A and B, this method takes all the labels from B and
// adds them to the list of A's labels. It also updates the nodeMap
// with this new information
void Graph::takeLabels(Node * A, Node * B) {
    std::string currentLabel = B->getLabel();
    (*nodeMap)[currentLabel] = A; // update the map for each label from B
    A->takeLabel(B);
    return;
}

// given another Node, this method copies every edge outgoing from the
// other Node and adds to this one
void Graph::addTargetsOfOther(Node * thisNode, Node * otherNode) {
    std::vector<Edge*> copyEdges = *(otherNode->getOutEdges());

    for (std::size_t i = 0; i < copyEdges.size(); i++) {
        Edge* edgeToCopy = copyEdges[i];
        Node * targetNode = edgeToCopy->getTarget();

        if (!thisNode->alreadyHasEdge(targetNode)) {
            createEdge(thisNode, targetNode);
        }
    }
}

// given another Node, this method copies every edge incoming to the other
// Node and adds to this one
void Graph::addSourcesOfOther(Node * thisNode, Node * otherNode) {
    std::vector<Edge*> copyEdges = *(otherNode->getInEdges());
    for (std::size_t i = 0; i < copyEdges.size(); i++) {
        Edge* edgeToCopy = copyEdges[i];
        // no need to be recursive:
        edgeToCopy->getSource()->addTarget(thisNode);
    }
}

//===-----------------------------------------------------------------===//

void Graph::createDotFile(std::string fileName) {
    std::ofstream outFile;
    outFile.open(fileName.c_str());

    outFile << "digraph {\n";
    outFile << "ordering=out;\nranksep=.3;\n  bgcolor=\"#292929\"; \
                            node [shape=box, fixedsize=false,\
                            fontsize=10, \
                            fontname=\"Courier-Bold\",\
                            fontcolor=\"grey4\"\n";
    outFile << "width=.15, height=.15, color=\"#00C389\",\
                            fillcolor=\"#00C389\",\
                            style=\"filled, solid, solid\"];\n";
    outFile << "edge [arrowsize=.4, color=\"firebrick1\",\
                        style=\"solid\"];\n";

    for (std::size_t i = 0; i < nodes->size(); i++) {
        Node* currNode = (*nodes)[i];
        if(currNode != NULL) {
            std::vector<Edge*>* currEdges = currNode->getOutEdges();

            for (int j = 0; j < (int) currEdges->size(); j++) {
                Edge* currEdge = (*currEdges)[j];
                if(currEdge != NULL) {
                    outFile << "\"" << currNode->toString()
                            << "\"" << " -> "
                            << "\"" << currEdge->getTarget()->toString()
                            << "\"" << "\n";
                }
            }
        }
    }
    outFile << "}\n";
    outFile.close();
}

void Graph::printAllNodes() {
    std::cerr << "\nAll Nodes:" << std::endl;
    for (std::size_t i = 0; i < nodes->size(); i++) {
        Node* currNode = (*nodes)[i];
        if(currNode != NULL) {
            std::cerr << currNode->toString() << std::endl;
        }
    }
}

void Graph::printGraph() {
    std::cerr << "\n" << __FUNCTION__ << std::endl;

    printAllNodes();

    for (std::size_t i = 0; i < nodes->size(); i++) {
        Node* currNode = (*nodes)[i];
        if(currNode != NULL) {
            std::vector<Edge*>* currEdges = currNode->getOutEdges();
            for (int j = 0; j < (int) currEdges->size(); j++) {
                Edge* currEdge = (*currEdges)[j];
                if(currEdge != NULL) {
                    std::cerr << "\"" << currNode->toString() << "\""
                              << " -> "
                              << "\"" << currEdge->getTarget()->toString()
                              << "\"" << std::endl;
                }
            }
        }
    }
    std::cerr << "" <<std::endl;
}
