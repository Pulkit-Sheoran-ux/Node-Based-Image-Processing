#pragma once
#include "../graph/Node.hpp"
#include <opencv2/opencv.hpp>
#include <vector>

class ColorChannelSplitterNode : public Node {
public:
    // Constructor accepting a name and whether to output grayscale
    ColorChannelSplitterNode(const std::string& name, bool outputGrayscale = false);

    void setInput(const cv::Mat& input) override;
    void process() override;
    void renderUI() override;
    cv::Mat getOutput() const override;
    cv::Mat mergeChannels();

    // Set whether to output grayscale representations of each channel
    void setOutputGrayscale(bool enable);
    void resetParams();
    cv::Mat inputImage;  // Input image
    cv::Mat redChannel;
    cv::Mat greenChannel;
    cv::Mat blueChannel;
    cv::Mat alphaChannel;  // Only for RGBA images
    bool outputGrayscale;  // Flag to control grayscale output
};
