#include "ThresholdNode.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <imgui.h>
#include <algorithm>  // for std::max_element

ThresholdNode::ThresholdNode(const std::string& name) {
    this->name = name;
    this->id = "threshold_" + name;
}

// Set the input image
void ThresholdNode::setInput(const cv::Mat& input) {
    inputImage = input;
}

// Apply the thresholding effect (binary, adaptive, or Otsu)
void ThresholdNode::process() {
    if (inputImage.empty()) {
        std::cerr << "No input image for ThresholdNode: " << name << std::endl;
        return;
    }

    // Convert to grayscale if the input is not grayscale
    if (inputImage.channels() != 1) {
        cv::cvtColor(inputImage, inputImage, cv::COLOR_BGR2GRAY);
    }

    // Apply the thresholding based on the selected method
    switch (thresholdType) {
        case BINARY:
            cv::threshold(inputImage, outputImage, thresholdValue, maxThresholdValue, cv::THRESH_BINARY);
            break;
        case ADAPTIVE:
            cv::adaptiveThreshold(inputImage, outputImage, maxThresholdValue, cv::ADAPTIVE_THRESH_MEAN_C,
                                  cv::THRESH_BINARY, blockSize, C);
            break;
        case OTSU:
            cv::threshold(inputImage, outputImage, 0, maxThresholdValue, cv::THRESH_BINARY | cv::THRESH_OTSU);
            break;
        default:
            std::cerr << "Unknown thresholding method!" << std::endl;
            break;
    }

    if (outputImage.empty()) {
        std::cerr << "Failed to apply thresholding." << std::endl;
    } else {
        std::cout << "Threshold applied using " << (thresholdType == BINARY ? "Binary" : 
                                                     (thresholdType == ADAPTIVE ? "Adaptive" : "Otsu"))
                  << " method." << std::endl;
    }
}

// Render the UI with sliders and histogram preview
void ThresholdNode::renderUI() {
    std::cout << "[ThresholdNode: " << name << "]" << std::endl;

    // Option to choose the threshold method
    if (ImGui::RadioButton("Binary", thresholdType == BINARY)) {
        thresholdType = BINARY;
        process();
    }
    if (ImGui::RadioButton("Adaptive", thresholdType == ADAPTIVE)) {
        thresholdType = ADAPTIVE;
        process();
    }
    if (ImGui::RadioButton("Otsu", thresholdType == OTSU)) {
        thresholdType = OTSU;
        process();
    }

    // Show the threshold value slider if Binary is selected
    if (thresholdType == BINARY) {
        if (ImGui::SliderInt("Threshold Value", &thresholdValue, 0, maxThresholdValue)) {
            process();  // Re-process the image when the threshold value changes
        }
    }

    // Show sliders for adaptive thresholding if Adaptive is selected
    if (thresholdType == ADAPTIVE) {
        if (ImGui::SliderInt("Block Size", &blockSize, 3, 21)) {
            if (blockSize % 2 == 0) blockSize++; // Block size should be odd
            process();  // Re-process the image when block size changes
        }
        if (ImGui::SliderInt("C Constant", &C, 1, 10)) {
            process();  // Re-process the image when constant C changes
        }
    }

    // Display histogram to assist in threshold selection
    if (!inputImage.empty()) {
        std::vector<int> histogram(256, 0);
        for (int i = 0; i < inputImage.rows; i++) {
            for (int j = 0; j < inputImage.cols; j++) {
                histogram[inputImage.at<uchar>(i, j)]++;
            }
        }

        // Convert to float for ImGui compatibility
        std::vector<float> histogramFloat(histogram.begin(), histogram.end());

        // Plot histogram
        ImGui::Text("Histogram");
        float maxVal = *std::max_element(histogramFloat.begin(), histogramFloat.end());
        ImGui::PlotHistogram("##Histogram", histogramFloat.data(), 256, 0, nullptr, 0.0f, maxVal, ImVec2(400, 150));
    }
}

// Get the output image
cv::Mat ThresholdNode::getOutput() const {
    return outputImage;
}

// Set the threshold type (binary, adaptive, otsu)
void ThresholdNode::setThresholdType(ThresholdType type) {
    thresholdType = type;
    process();
}

// Set the threshold value (for binary thresholding)
void ThresholdNode::setThresholdValue(int value) {
    thresholdValue = value;
    process();
}

// Set the block size for adaptive thresholding
void ThresholdNode::setBlockSize(int size) {
    blockSize = size;
    process();
}

// Set the constant C for adaptive thresholding
void ThresholdNode::setC(int constant) {
    C = constant;
    process();
}
