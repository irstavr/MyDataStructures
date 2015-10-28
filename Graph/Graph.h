#include <vector>
#include <string>
#include <map>

#include "Edge.h"
#include "Node.h"

class Node;

//////////////////////////    Graph Class    ///////////////////////////////////

class Graph {
private:
    std::vector<Node*> *nodes;
    std::vector<Edge*> *edges;
    std::map<std::string, Node*> *nodeMap;

public:
    // default constructor. Makes an empty graph
    Graph();

    // copy constructor
    Graph(const Graph &graph);

    // destructor
    ~Graph();

    // getter methods
    std::vector<Node*> * getNodes() const{
        return nodes;
    }

    std::vector<Edge*> * getEdges() const{
        return edges;
    }

    std::map<std::string, Node*> * getNodeMap() const{
        return nodeMap;
    }

    // to create a new Node without edges and add it to the graph.
    Node* makeNode(std::string label);

    // to create a new Node which is the source.
    // Finds the Node corresponding to the input target label and attaches
    // the new source Node to it. Node is added to the graph.
    void makeNode(std::string sourceVar, std::string targetVar);

    // given two variables and the edge type (MAY or MUST), this method
    // will construct two new vertices, construct an edge between them,
    //and add the  vertices to the graph
    void makeNodes(std::string sourceVar, std::string targetVar);

    // given labels corresponding to vertices, this method adds an edge
    // from the source to the target
    bool createEdge(std::string sourceVar, std::string targetVar);

    // given vertices, this method adds an edge from the source
    // to the target
    void createEdge(Node* sourceNode, Node* targetNode);

    // to remove a Node from the graph. this will delete all edges that
    // include this Node from any other Node, remove this Node from the
    // list of vertices, and delete the Node
    void removeNode(std::string label);
    void removeNode(Node* thisNode);

    // given the label of a current Node and a new label, this method
    // constructs a new Node with the same outgoing edges as the old Node.
    // Adds to graph if clong is successful, return true, otherwise false
    bool cloneNode(std::string newLabel, std::string oldLabel);

    //private:
    // given a label, returns the Node which the label corresponds to
    Node* getNodeAtLabel(std::string label);

    // given another Node, this method copies every edge outgoing from
    // the other Node and adds to this one
    void addTargetsOfOther(Node* thisNode, Node* otherNode);

    // given another Node, this method copies every edge incoming from
    // the other Node and adds to this one
    void addSourcesOfOther(Node* thisNode, Node* otherNode);

    // given vertices A and B, this method merges Node B into Node A.
    // Node A will now have all incoming and outgoing edges that B had.
    // B is removed from the graph.
    void merge(Node* A, Node* B);

    // given vertixes A and B, this method takes all the labels from B and
    // adds them to the list of A's labels.
    // it also updates the NodeMap with this new information
    void takeLabels(Node* A, Node* B);

    // creates a dot file of the graph for visual inspection
    void createDotFile(std::string fileName);

    // print functions
    void printAllNodes();
    void printGraph();
};
