#include "BrightnessContrastNode.hpp"
#include <iostream>

BrightnessContrastNode::BrightnessContrastNode(const std::string& name) {
    this->name = name;
    this->id = "brightness_contrast_" + name;
}

BrightnessContrastNode::BrightnessContrastNode(const std::string& name, double alpha, int beta) {
    this->name = name;
    this->id = "bc_" + name;
    this->alpha = alpha;
    this->beta = beta;
}

// Set input image
void BrightnessContrastNode::setInput(const cv::Mat& input) {
    inputImage = input;
}

// Set parameters for contrast and brightness
void BrightnessContrastNode::setParams(double contrast, int brightness) {
    this->alpha = contrast;
    this->beta = brightness;
}

// Reset parameters to default
void BrightnessContrastNode::resetParams() {
    this->alpha = 1.0;  // Default contrast
    this->beta = 0;     // Default brightness
    std::cout << "Reset parameters to default: α = " << alpha << ", β = " << beta << std::endl;
}

// Process image (apply brightness/contrast adjustments)
void BrightnessContrastNode::process() {
    if (inputImage.empty()) {
        std::cerr << "No input image for BrightnessContrastNode: " << name << std::endl;
        return;
    }

    inputImage.convertTo(outputImage, -1, alpha, beta);
    std::cout << "Applied Brightness/Contrast to: " << name << std::endl;
}

// Render UI with sliders and reset button
void BrightnessContrastNode::renderUI() {
    std::cout << "[BrightnessContrastNode: " << name 
              << "] α = " << alpha << ", β = " << beta << std::endl;

    float alphaFloat = static_cast<float>(alpha); // Convert alpha to float for ImGui slider
    if (ImGui::SliderFloat("Contrast (α)", &alphaFloat, 0.0f, 3.0f)) {
        alpha = static_cast<double>(alphaFloat); // Update the alpha value from the slider
    }

    if (ImGui::SliderInt("Brightness (β)", &beta, -100, 100)) {
        // Sliders automatically update the beta value
    }

    if (ImGui::Button("Reset Parameters")) {
        resetParams();  // Reset the contrast and brightness to defaults
        alphaFloat = static_cast<float>(alpha); // Update slider value after reset
    }
}

// Get output image
cv::Mat BrightnessContrastNode::getOutput() const {
    return outputImage;
}
