#include "BlurNode.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>
#include <imgui.h>

#define PI 3.14159265358979323846

// Constructor to initialize the node with a name
BlurNode::BlurNode(const std::string& name) {
    this->name = name;
    this->id = "blur_" + name;
}

// Set the input image
void BlurNode::setInput(const cv::Mat& input) {
    inputImage = input;
}

// Function to generate a directional blur kernel
cv::Mat BlurNode::generateDirectionalKernel(int radius, float angleDegrees) {
    int size = radius * 2 + 1;
    cv::Mat kernel = cv::Mat::zeros(size, size, CV_32F);

    float angleRad = angleDegrees * PI / 180.0f;
    float dx = cos(angleRad);
    float dy = sin(angleRad);

    float length = std::sqrt(dx * dx + dy * dy);
    dx /= length;
    dy /= length;

    float sum = 0.0f;
    for (int i = -radius; i <= radius; ++i) {
        float x = radius + i * dx;
        float y = radius + i * dy;

        int ix = static_cast<int>(std::round(x));
        int iy = static_cast<int>(std::round(y));

        if (ix >= 0 && ix < size && iy >= 0 && iy < size) {
            kernel.at<float>(iy, ix) = 1.0f;
            sum += 1.0f;
        }
    }

    kernel /= sum;
    return kernel;
}

// Function to generate a Gaussian blur kernel
cv::Mat BlurNode::generateGaussianKernel(int radius) {
    int size = 2 * radius + 1;
    cv::Mat kernel(size, size, CV_32F);

    // Generate Gaussian kernel
    float sigma = radius / 3.0f;
    float sum = 0.0f;
    for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius; x++) {
            kernel.at<float>(y + radius, x + radius) = std::exp(-(x * x + y * y) / (2 * sigma * sigma));
            sum += kernel.at<float>(y + radius, x + radius);
        }
    }

    // Normalize the kernel
    kernel /= sum;
    return kernel;
}

// Apply the blur effect (either directional or Gaussian)
void BlurNode::process() {
    if (inputImage.empty()) {
        std::cerr << "No input image for BlurNode: " << name << std::endl;
        return;
    }

    cv::Mat kernel;
    if (directional) {
        // Apply directional blur (45 degrees by default)
        kernel = generateDirectionalKernel(radius, angle);
        std::cout << "Generated Directional Kernel." << std::endl;
    } else {
        // Apply Gaussian blur (uniform)
        kernel = generateGaussianKernel(radius);
        std::cout << "Generated Gaussian Kernel." << std::endl;
    }

    // Apply the blur using filter2D
    cv::filter2D(inputImage, outputImage, -1, kernel);

    if (outputImage.empty()) {
        std::cerr << "Failed to apply blur to the image." << std::endl;
    } else {
        std::cout << "Blur applied with radius: " << radius << " and " << (directional ? "directional" : "uniform") << " blur." << std::endl;
    }
}

// Render the UI with sliders and kernel preview
void BlurNode::renderUI() {
    std::cout << "[BlurNode: " << name << "]" << std::endl;

    // Slider for blur radius
    if (ImGui::SliderInt("Radius", &radius, 1, 20)) {
        process();  // Re-process the image when radius changes
    }

    // Checkbox for directional or uniform blur
    if (ImGui::Checkbox("Directional Blur", &directional)) {
        process();  // Re-process the image when blur type changes
    }

    // Preview the kernel
    cv::Mat kernelPreview = generateGaussianKernel(radius);  // Preview Gaussian kernel
    if (directional) {
        kernelPreview = generateDirectionalKernel(radius, 45.0f);  // Preview Directional kernel
    }

    // Display the kernel matrix using OpenCV's imshow
    if (!kernelPreview.empty()) {
        cv::imshow("Kernel Preview", kernelPreview);  // Display the kernel preview for educational purposes
        cv::waitKey(0);
    }
}

// Get the output image
cv::Mat BlurNode::getOutput() const {
    return outputImage;
}

// Set blur radius
void BlurNode::setRadius(int newRadius) {
    radius = newRadius;
    process();  // Re-process the image with the new radius
}

void BlurNode::setAngle(float newAngle) {
    angle = newAngle;
    process();
}

// Set whether blur is directional or uniform
void BlurNode::setDirectional(bool isDirectional) {
    directional = isDirectional;
    process();  // Re-process the image with the new blur type
}
