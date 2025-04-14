#include "graph/NodeGraph.hpp"
#include "nodes/ImageInputNode.hpp"
#include "nodes/ColorChannelSplitterNode.hpp"
#include "nodes/OutputNode.hpp"
#include "nodes/BrightnessContrastNode.hpp" // Include the new node
#include <iostream>
#include <imgui.h>

// Function prototypes for better code organization
void showMenu();
void loadImage(std::shared_ptr<ImageInputNode> &inputNode, bool &imageLoaded);
void splitChannels(std::shared_ptr<ColorChannelSplitterNode> &splitterNode, std::shared_ptr<ImageInputNode> &inputNode, bool &channelsSplit, bool imageLoaded);
void convertToGrayscale(std::shared_ptr<ImageInputNode> &inputNode); // Update this function to convert the entire image
void adjustBrightnessContrast(std::shared_ptr<BrightnessContrastNode> &bcNode, bool imageLoaded);
void saveOutputImage(std::shared_ptr<OutputNode> &outputNode, bool channelsSplit, std::shared_ptr<ImageInputNode> &inputNode, std::shared_ptr<ColorChannelSplitterNode> &splitterNode);
void mergeChannels(std::shared_ptr<ColorChannelSplitterNode> &splitterNode); // Merge the channels back

int main()
{
    NodeGraph graph;

    // Create nodes
    auto inputNode = std::make_shared<ImageInputNode>("MyImage", "../assets/a.jpg");
    auto splitterNode = std::make_shared<ColorChannelSplitterNode>("Splitter", false);    // Disable grayscale initially
    auto outputNode = std::make_shared<OutputNode>("Result", "output_image", "jpg", 85);  // Quality is 85 for JPG
    auto bcNode = std::make_shared<BrightnessContrastNode>("BrightnessContrast", 1.0, 0); // Default contrast and brightness

    // Add to graph
    graph.addNode(inputNode);
    graph.addNode(splitterNode);
    graph.addNode(outputNode);
    graph.addNode(bcNode); // Add the brightness/contrast node

    int choice = 0;
    bool imageLoaded = false;
    bool channelsSplit = false;

    while (true)
    {
        showMenu();
        std::cout << "Enter your choice (1-6): ";
        std::cin >> choice;

        switch (choice)
        {
        case 1:
            loadImage(inputNode, imageLoaded);
            break;
        case 2:
            splitChannels(splitterNode, inputNode, channelsSplit, imageLoaded);
            break;
        case 3:
            convertToGrayscale(inputNode); // Convert the whole image to grayscale
            break;
        case 4:
            adjustBrightnessContrast(bcNode, imageLoaded);
            break;
        case 5:
            saveOutputImage(outputNode, channelsSplit, inputNode, splitterNode);
            break;
        case 6:
            mergeChannels(splitterNode); // Merge the channels back together
            break;
        case 7:
            std::cout << "Exiting program." << std::endl;
            return 0;
        default:
            std::cout << "Invalid choice. Please select a valid option." << std::endl;
        }
    }

    return 0;
}

void showMenu()
{
    std::cout << "\nChoose an action:" << std::endl;
    std::cout << "1. Load Image" << std::endl;
    std::cout << "2. Split Color Channels" << std::endl;
    std::cout << "3. Convert to Grayscale" << std::endl; // Convert the whole image to grayscale
    std::cout << "4. Adjust Brightness and Contrast" << std::endl;
    std::cout << "5. Save Output Image" << std::endl;
    std::cout << "6. Merge Channels" << std::endl; // New option to merge channels back
    std::cout << "7. Exit" << std::endl;
}

// Function to load the image
void loadImage(std::shared_ptr<ImageInputNode> &inputNode, bool &imageLoaded)
{
    if (!imageLoaded)
    {
        inputNode->process();
        std::cout << "Image loaded successfully!" << std::endl;
        imageLoaded = true;
    }
    else
    {
        std::cout << "Image is already loaded." << std::endl;
    }
}

// Function to split the color channels
void splitChannels(std::shared_ptr<ColorChannelSplitterNode> &splitterNode, std::shared_ptr<ImageInputNode> &inputNode, bool &channelsSplit, bool imageLoaded)
{
    if (imageLoaded && !channelsSplit)
    {
        splitterNode->setInput(inputNode->getOutput());
        splitterNode->process();
        std::cout << "Color channels split successfully!" << std::endl;
        channelsSplit = true;
    }
    else if (!imageLoaded)
    {
        std::cout << "Please load an image first." << std::endl;
    }
    else
    {
        std::cout << "Channels are already split." << std::endl;
    }
}

// Function to convert the entire image to grayscale
void convertToGrayscale(std::shared_ptr<ImageInputNode> &inputNode)
{
    inputNode->convertToGrayscale(); // Convert the entire image to grayscale
}

// Function to adjust brightness and contrast
void adjustBrightnessContrast(std::shared_ptr<BrightnessContrastNode> &bcNode, bool imageLoaded)
{
    if (imageLoaded)
    {
        float alpha, beta;

        // Alpha for contrast, beta for brightness
        std::cout << "Enter contrast (alpha) value (1.0 is normal): ";
        std::cin >> alpha;
        std::cout << "Enter brightness (beta) value (0 is normal): ";
        std::cin >> beta;

        // Apply the brightness and contrast adjustments to the whole image
        bcNode->setParams(alpha, beta); // Update the contrast and brightness parameters
        bcNode->process();              // Process the image with new settings
        std::cout << "Brightness and contrast adjusted." << std::endl;
    }
    else
    {
        std::cout << "Please load an image first." << std::endl;
    }
}

// Function to save the output image
void saveOutputImage(std::shared_ptr<OutputNode> &outputNode, bool channelsSplit, std::shared_ptr<ImageInputNode> &inputNode, std::shared_ptr<ColorChannelSplitterNode> &splitterNode)
{
    if (!channelsSplit)
    {
        // If channels are not split, save the entire adjusted image (after brightness/contrast)
        outputNode->setInput(inputNode->getOutput()); // Use the adjusted image from the input node
        outputNode->process();
        std::cout << "Output image saved (adjusted brightness/contrast)." << std::endl;
    }
    else
    {
        std::cout << "Please split the color channels first to save specific channels." << std::endl;
    }
}

void mergeChannels(std::shared_ptr<ColorChannelSplitterNode> &splitterNode)
{
    cv::Mat mergedImage = splitterNode->mergeChannels(); // Merge the split channels
    if (!mergedImage.empty())
    {
        cv::imwrite("Merged_Image.png", mergedImage); // Save the merged image
        std::cout << "Merged image saved." << std::endl;
    }
    else
    {
        std::cout << "Failed to merge channels." << std::endl;
    }
}
