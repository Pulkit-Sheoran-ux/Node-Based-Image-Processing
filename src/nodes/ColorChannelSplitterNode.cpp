#include "ColorChannelSplitterNode.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <imgui.h>

ColorChannelSplitterNode::ColorChannelSplitterNode(const std::string& name, bool outputGrayscale)
    : Node(), outputGrayscale(outputGrayscale) {
    this->name = name;
    this->id = "color_splitter_" + name;
}

void ColorChannelSplitterNode::setInput(const cv::Mat& input) {
    inputImage = input;
}

void ColorChannelSplitterNode::process() {
    if (inputImage.empty()) {
        std::cerr << "No input image for ColorChannelSplitterNode: " << name << std::endl;
        return;
    }

    // Convert the image to grayscale if required
    cv::Mat grayscale;
    if (outputGrayscale) {
        cv::cvtColor(inputImage, grayscale, cv::COLOR_BGR2GRAY); // Update input image to grayscale image
        cv::imwrite("GrayScale.png", grayscale);
        std::cout << "Image converted to grayscale." << std::endl;
    }

    std::vector<cv::Mat> channels;
    if (inputImage.channels() == 3) {
        cv::split(inputImage, channels);  // Split into RGB channels
        redChannel = channels[2];   // Red channel
        greenChannel = channels[1]; // Green channel
        blueChannel = channels[0];  // Blue channel
    } else if (inputImage.channels() == 4) {
        cv::split(inputImage, channels);  // Split into RGBA channels
        redChannel = channels[2];
        greenChannel = channels[1];
        blueChannel = channels[0];
        alphaChannel = channels[3];  // Alpha channel
    }

    // Save the individual channels
    if (!redChannel.empty()) {
        cv::imwrite("Red_Channel.png", redChannel);  // Save Red Channel
    }
    if (!greenChannel.empty()) {
        cv::imwrite("Green_Channel.png", greenChannel);  // Save Green Channel
    }
    if (!blueChannel.empty()) {
        cv::imwrite("Blue_Channel.png", blueChannel);  // Save Blue Channel
    }

    // If grayscale is enabled, save grayscale images of channels
    if (outputGrayscale) {
        cv::imwrite("Red_Grayscale.png", redChannel);  // Save Red Channel as Grayscale
        cv::imwrite("Green_Grayscale.png", greenChannel);  // Save Green Channel as Grayscale
        cv::imwrite("Blue_Grayscale.png", blueChannel);  // Save Blue Channel as Grayscale
    }

    // Display the channels using OpenCV's imshow for debugging or visualization
    if (!redChannel.empty()) {
        cv::imshow("Red Channel", redChannel);  // Display Red Channel
    }
    if (!greenChannel.empty()) {
        cv::imshow("Green Channel", greenChannel);  // Display Green Channel
    }
    if (!blueChannel.empty()) {
        cv::imshow("Blue Channel", blueChannel);  // Display Blue Channel
    }
    if (!alphaChannel.empty()) {
        cv::imshow("Alpha Channel", alphaChannel);  // Display Alpha Channel
    }

    cv::waitKey(0);  // Wait until a key is pressed to close the windows
}

// Merge the 3 channels (Red, Green, Blue) back into one image
cv::Mat ColorChannelSplitterNode::mergeChannels() {
    if (redChannel.empty() || greenChannel.empty() || blueChannel.empty()) {
        std::cerr << "One or more channels are empty, cannot merge." << std::endl;
        return cv::Mat();  // Return an empty matrix if any channel is missing
    }

    std::vector<cv::Mat> channels = {blueChannel, greenChannel, redChannel};  // BGR order
    cv::Mat mergedImage;
    cv::merge(channels, mergedImage);  // Merge the channels back into a single image

    return mergedImage;
}

void ColorChannelSplitterNode::renderUI() {
    std::cout << "[ColorChannelSplitterNode: " << name << "]" << std::endl;

    // Checkbox to toggle grayscale output
    if (ImGui::Checkbox("Output Grayscale", &outputGrayscale)) {
        process();  // Re-process when the grayscale option is toggled
    }

    // Display preview of each channel
    if (!redChannel.empty()) {
        ImGui::Text("Red Channel");
        cv::Mat redChannelDisplay;
        cv::cvtColor(redChannel, redChannelDisplay, cv::COLOR_GRAY2BGRA);  // Convert to 4 channels (RGBA)
        ImTextureID redTexture = reinterpret_cast<ImTextureID>(redChannelDisplay.data);
        ImGui::Image(redTexture, ImVec2(redChannel.cols, redChannel.rows));  // Show red channel image
    }

    if (!greenChannel.empty()) {
        ImGui::Text("Green Channel");
        cv::Mat greenChannelDisplay;
        cv::cvtColor(greenChannel, greenChannelDisplay, cv::COLOR_GRAY2BGRA);  // Convert to 4 channels (RGBA)
        ImTextureID greenTexture = reinterpret_cast<ImTextureID>(greenChannelDisplay.data);
        ImGui::Image(greenTexture, ImVec2(greenChannel.cols, greenChannel.rows));  // Show green channel image
    }

    if (!blueChannel.empty()) {
        ImGui::Text("Blue Channel");
        cv::Mat blueChannelDisplay;
        cv::cvtColor(blueChannel, blueChannelDisplay, cv::COLOR_GRAY2BGRA);  // Convert to 4 channels (RGBA)
        ImTextureID blueTexture = reinterpret_cast<ImTextureID>(blueChannelDisplay.data);
        ImGui::Image(blueTexture, ImVec2(blueChannel.cols, blueChannel.rows));  // Show blue channel image
    }

    if (!alphaChannel.empty()) {
        ImGui::Text("Alpha Channel");
        cv::Mat alphaChannelDisplay;
        cv::cvtColor(alphaChannel, alphaChannelDisplay, cv::COLOR_GRAY2BGRA);  // Convert to 4 channels (RGBA)
        ImTextureID alphaTexture = reinterpret_cast<ImTextureID>(alphaChannelDisplay.data);
        ImGui::Image(alphaTexture, ImVec2(alphaChannel.cols, alphaChannel.rows));  // Show alpha channel image
    }
}

cv::Mat ColorChannelSplitterNode::getOutput() const {
    // Depending on the output mode, return the appropriate channel or merged image
    if (outputGrayscale) {
        return redChannel;  // Grayscale channel output (you could customize this for more complex outputs)
    } else {
        return inputImage;  // Return the original input image if not in grayscale mode
    }
}

void ColorChannelSplitterNode::setOutputGrayscale(bool enable) {
    outputGrayscale = enable;
    process();  // Re-process the image with the new setting
}

void ColorChannelSplitterNode::resetParams() {
    outputGrayscale = false;
    process();  // Reset the processing with default parameters
}
