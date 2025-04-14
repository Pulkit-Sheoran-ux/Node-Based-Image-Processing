#include "BlendNode.hpp"
#include <opencv2/opencv.hpp>
#include <imgui.h>
#include <iostream>

BlendNode::BlendNode(const std::string& name) {
    this->name = name;
    this->id = "blend_node_" + name;
}

void BlendNode::setInputA(const cv::Mat& image) {
    inputA = image.clone();
}

void BlendNode::setInputB(const cv::Mat& image) {
    inputB = image.clone();
}

void BlendNode::setBlendMode(BlendMode mode) {
    blendMode = mode;
    process();
}

void BlendNode::setOpacity(float value) {
    opacity = std::clamp(value, 0.0f, 1.0f);
    process();
}

cv::Mat BlendNode::getOutput() const {
    return outputImage;
}

void BlendNode::process() {
    if (inputA.empty() || inputB.empty()) {
        std::cerr << "One or both input images are empty in BlendNode: " << name << std::endl;
        return;
    }

    cv::Mat resizedB;
    cv::resize(inputB, resizedB, inputA.size()); // ensure same size

    cv::Mat blendA, blendB;
    inputA.convertTo(blendA, CV_32F, 1.0 / 255.0);
    resizedB.convertTo(blendB, CV_32F, 1.0 / 255.0);

    cv::Mat result;
    switch (blendMode) {
        case NORMAL:
            result = blendB;
            break;
        case MULTIPLY:
            cv::multiply(blendA, blendB, result);
            break;
        case SCREEN:
            result = 1.0 - (1.0 - blendA).mul(1.0 - blendB);
            break;
        case OVERLAY:
            result = cv::Mat(blendA.size(), blendA.type());
            for (int y = 0; y < blendA.rows; y++) {
                for (int x = 0; x < blendA.cols; x++) {
                    for (int c = 0; c < blendA.channels(); c++) {
                        float a = blendA.at<cv::Vec3f>(y, x)[c];
                        float b = blendB.at<cv::Vec3f>(y, x)[c];
                        result.at<cv::Vec3f>(y, x)[c] = (a < 0.5f) ? (2 * a * b) : (1 - 2 * (1 - a) * (1 - b));
                    }
                }
            }
            break;
        case DIFFERENCE:
            result = cv::abs(blendA - blendB);
            break;
    }

    // Blend with opacity
    result = opacity * result + (1.0f - opacity) * blendA;

    result.convertTo(outputImage, CV_8U, 255.0);
}

void BlendNode::renderUI() {
    const char* blendNames[] = { "Normal", "Multiply", "Screen", "Overlay", "Difference" };

    if (ImGui::Combo("Blend Mode", reinterpret_cast<int*>(&blendMode), blendNames, IM_ARRAYSIZE(blendNames))) {
        process();
    }

    if (ImGui::SliderFloat("Opacity", &opacity, 0.0f, 1.0f)) {
        process();
    }
}
