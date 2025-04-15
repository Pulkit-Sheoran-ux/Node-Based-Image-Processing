#include "ConvolutionFilterNode.hpp"
#include <iostream>

ConvolutionFilterNode::ConvolutionFilterNode(const std::string &id, const std::string &name)
{
    this->id = id;
    this->name = name;
    this->nodeType = NodeType::Processing;
}

void ConvolutionFilterNode::setKernelSize(int size)
{
    if (size == 3 || size == 5)
    {
        kernelSize = size;
        kernelData.resize(size * size, 0.0f);
    }
}

void ConvolutionFilterNode::setCustomKernel(const std::vector<float> &data)
{
    if (data.size() == kernelSize * kernelSize)
    {
        kernelData = data;
        preset = PresetType::Custom;
    }
}

void ConvolutionFilterNode::setPreset(PresetType type)
{
    preset = type;
    loadPreset(type);
}

void ConvolutionFilterNode::setInput(const cv::Mat &input)
{
    inputImage = input.clone();
}

void ConvolutionFilterNode::process()
{
    applyKernel();
}

void ConvolutionFilterNode::renderUI()
{
    std::cout << "Rendering UI for Convolution Filter Node\n";
}

cv::Mat ConvolutionFilterNode::getOutput() const
{
    return outputImage;
}

void ConvolutionFilterNode::applyKernel()
{
    if (inputImage.empty())
        return;

    cv::Mat kernel(kernelSize, kernelSize, CV_32F, const_cast<float *>(kernelData.data()));
    cv::filter2D(inputImage, outputImage, -1, kernel);
}

void ConvolutionFilterNode::loadPreset(PresetType type)
{
    switch (type)
    {
    case PresetType::Sharpen:
        kernelSize = 3;
        kernelData = {
            0, -1, 0,
            -1, 5, -1,
            0, -1, 0};
        break;
    case PresetType::Emboss:
        kernelSize = 3;
        kernelData = {
            -2, -1, 0,
            -1, 1, 1,
            0, 1, 2};
        break;
    case PresetType::EdgeEnhance:
        kernelSize = 3;
        kernelData = {
            0, 0, 0,
            -1, 1, 0,
            0, 0, 0};
        break;
    default:
        break;
    }
}
