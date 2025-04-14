#pragma once
#include <string>
#include <opencv2/opencv.hpp>
#include <vector>
#include <memory>

class Node {
public:
    std::string id;      // Unique identifier for the node
    std::string name;    // User-defined name for the node

    virtual void process() = 0;  // Perform image processing
    virtual void renderUI() = 0;  // Render node-specific UI (ImGui)
    virtual cv::Mat getOutput() const = 0;  // Get processed output
    virtual void setInput(const cv::Mat& input) { this->input = input; }  // Set input for the node

    // Optionally get the input image (if needed in subclasses or for debugging)
    cv::Mat getInput() const { return input; }

    virtual ~Node() = default;  // Virtual destructor for proper cleanup

protected:
    cv::Mat input;  // Store the input image for processing

    // Optionally add a type or category to help classify the nodes
    enum class NodeType { Input, Processing, Output };
    NodeType nodeType;  // Type of node (could be used for UI or internal classification)
};
