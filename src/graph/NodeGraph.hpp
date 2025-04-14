#pragma once
#include <vector>
#include <memory>
#include "Node.hpp"

class NodeGraph {
public:
    // Add node to the graph
    void addNode(const std::shared_ptr<Node>& node);

    // Run the node graph (process nodes in the correct order)
    void run();

    // Manage connections between nodes
    void connectNodes(const std::shared_ptr<Node>& fromNode, const std::shared_ptr<Node>& toNode);

    // Optionally, clear nodes and connections
    void clear();

    // Get all nodes in the graph
    const std::vector<std::shared_ptr<Node>>& getNodes() const;

private:
    std::vector<std::shared_ptr<Node>> nodes;  // List of nodes in the graph
    std::vector<std::pair<std::shared_ptr<Node>, std::shared_ptr<Node>>> connections;  // Node connections
};
