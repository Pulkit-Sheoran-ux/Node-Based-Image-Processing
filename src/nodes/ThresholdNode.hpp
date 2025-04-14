#pragma once
#include "../graph/Node.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>

class ThresholdNode : public Node {
private:
    cv::Mat inputImage;
    cv::Mat outputImage;
    int thresholdValue = 128;  // Default threshold value
    int maxThresholdValue = 255;  // Maximum threshold value
    int blockSize = 11;  // For adaptive thresholding
    int C = 2;  // For adaptive thresholding

    enum ThresholdType {
        BINARY,
        ADAPTIVE,
        OTSU
    };
    ThresholdType thresholdType = BINARY;  // Default to binary threshold

public:
    ThresholdNode(const std::string& name);

    // Set the input image
    void setInput(const cv::Mat& input) override;

    // Apply the thresholding effect
    void process() override;

    // Render the UI with sliders and histogram preview
    void renderUI() override;

    // Get the output image
    cv::Mat getOutput() const override;

    // Set the threshold type
    void setThresholdType(ThresholdType type);

    // Set the threshold value (for binary thresholding)
    void setThresholdValue(int value);

    // Set the block size for adaptive thresholding
    void setBlockSize(int size);

    // Set the constant C for adaptive thresholding
    void setC(int constant);
};
