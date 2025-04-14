#pragma once
#include "../graph/Node.hpp"
#include <opencv2/opencv.hpp>  // For OpenCV Mat operations
#include <imgui.h> // Make sure you have ImGui integrated

class BrightnessContrastNode : public Node {
private:
    cv::Mat inputImage;
    cv::Mat outputImage;
    double alpha = 1.0; // contrast
    int beta = 0;       // brightness

public:
    BrightnessContrastNode(const std::string& name);
    BrightnessContrastNode(const std::string& name, double alpha, int beta);
    
    void setInput(const cv::Mat& input) override;
    void setParams(double contrast, int brightness);
    void process() override;
    void renderUI() override;
    cv::Mat getOutput() const override;
    void resetParams();  // Method to reset parameters to default
};
