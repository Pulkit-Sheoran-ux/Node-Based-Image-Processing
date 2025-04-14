#pragma once
#include "../graph/Node.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>

class BlurNode : public Node {
private:
    cv::Mat inputImage;
    cv::Mat outputImage;
    int radius = 3;  // Default radius
    bool directional = false;  // Flag for directional blur
    float angle = 0.0f; // Angle for directional blur (in degrees)

    // Function to generate a directional blur kernel
    cv::Mat generateDirectionalKernel(int radius, float angle);

    // Function to generate a Gaussian blur kernel
    cv::Mat generateGaussianKernel(int radius);

public:
    BlurNode(const std::string& name);

    // Set the input image
    void setInput(const cv::Mat& input) override;

    // Apply the blur effect
    void process() override;

    // Render the UI with sliders and kernel preview
    void renderUI() override;

    // Get the output image
    cv::Mat getOutput() const override;

    // Set the blur radius
    void setRadius(int newRadius);

    // Set the angle for directional blur
    void setAngle(float newAngle);

    // Set whether blur is directional or uniform
    void setDirectional(bool isDirectional);
};
