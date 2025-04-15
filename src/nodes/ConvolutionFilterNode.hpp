#pragma once

#include "../graph/Node.hpp"
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

class ConvolutionFilterNode : public Node {
public:
    enum class PresetType { Custom, Sharpen, Emboss, EdgeEnhance };

    ConvolutionFilterNode(const std::string& id, const std::string& name);

    void setKernelSize(int size); // 3 or 5
    void setCustomKernel(const std::vector<float>& kernelData);
    void setPreset(PresetType type);

    void setInput(const cv::Mat& input) override;
    void process() override;
    void renderUI() override;
    cv::Mat getOutput() const override;

private:
    void applyKernel();
    void loadPreset(PresetType type);

    int kernelSize = 3;
    std::vector<float> kernelData;
    PresetType preset = PresetType::Custom;

    cv::Mat inputImage;
    cv::Mat outputImage;
};
