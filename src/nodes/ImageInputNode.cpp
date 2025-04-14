#include "ImageInputNode.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>

// Constructor to initialize node name and file path
ImageInputNode::ImageInputNode(const std::string& name, const std::string& filePath)
    : Node(), filePath(filePath), name(name) {}

// Load image from the given file path
void ImageInputNode::process() {
    input = cv::imread(filePath);
    if (input.empty()) {
        std::cerr << "Failed to load image: " << filePath << std::endl;
    } else {
        output = input.clone();  // Initialize the output as a copy of the input
    }
}

// Set the modified image as the output
void ImageInputNode::setOutput(const cv::Mat& newOutput) {
    output = newOutput;  // Update the output image with the modified one
}

// Get the current output image (after modifications, or original if no modifications)
cv::Mat ImageInputNode::getOutput() const {
    return output;
}

// Implement renderUI() method here
void ImageInputNode::renderUI() {
    // Code for rendering the UI for this node (image input node)
    std::cout << "Rendering UI for Image Input Node: " << name << std::endl;
}

// Convert the entire image to grayscale
void ImageInputNode::convertToGrayscale() {
    if (!input.empty()) {
        cv::cvtColor(input, output, cv::COLOR_BGR2GRAY);  // Convert the entire image to grayscale
        std::cout << "Image converted to grayscale." << std::endl;
    } else {
        std::cout << "No image loaded to convert to grayscale." << std::endl;
    }
}
