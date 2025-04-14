#pragma once
#include "../graph/Node.hpp"
#include <opencv2/opencv.hpp>

class ImageInputNode : public Node {
public:
    // Constructor accepting a file path for image
    ImageInputNode(const std::string& name, const std::string& filePath);

    // Load image into the input
    void process() override;

    // Convert the entire image to grayscale
    void convertToGrayscale();
    

    // Set the output image after applying changes like brightness and contrast
    void setOutput(const cv::Mat& newOutput);

    // Get the current output image
    cv::Mat getOutput() const override;

    // Implement the renderUI() method, as required by the abstract base class Node
    void renderUI() override;

private:
    std::string name;
    std::string filePath;
    cv::Mat input;  // The original loaded image
    cv::Mat output; // The modified image (output)
};
