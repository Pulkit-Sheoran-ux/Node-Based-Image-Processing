#include "NodeGraph.hpp"
#include <iostream>
#include <algorithm>

void NodeGraph::addNode(const std::shared_ptr<Node>& node) {
    nodes.push_back(node);
}

void NodeGraph::connectNodes(const std::shared_ptr<Node>& fromNode, const std::shared_ptr<Node>& toNode) {
    // Check if the connection is valid (basic check here, you can enhance it)
    if (std::find(nodes.begin(), nodes.end(), fromNode) != nodes.end() &&
        std::find(nodes.begin(), nodes.end(), toNode) != nodes.end()) {
        connections.push_back(std::make_pair(fromNode, toNode));
    } else {
        std::cerr << "Invalid node connection!" << std::endl;
    }
}

void NodeGraph::run() {
    std::cout << "Node graph running with " << nodes.size() << " nodes...\n";

    // Process nodes (ensure the right order)
    for (auto& node : nodes) {
        node->process();  // Process each node (e.g., image manipulation)
    }

    // Now manage the connections and pass data from one node to another
    for (const auto& connection : connections) {
        const auto& fromNode = connection.first;
        const auto& toNode = connection.second;
        
        // Set the output of fromNode as the input to toNode
        toNode->setInput(fromNode->getOutput());
    }

    // Optionally, render the UI after all nodes are processed
    for (auto& node : nodes) {
        node->renderUI();  // Render UI for each node (using ImGui or other frameworks)
    }
}

void NodeGraph::clear() {
    nodes.clear();
    connections.clear();
}

const std::vector<std::shared_ptr<Node>>& NodeGraph::getNodes() const {
    return nodes;
}
