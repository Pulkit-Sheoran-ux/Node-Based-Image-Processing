#include "graph/NodeGraph.hpp"
#include "nodes/ImageInputNode.hpp"
#include "nodes/ColorChannelSplitterNode.hpp"
#include "nodes/OutputNode.hpp"
#include "nodes/BrightnessContrastNode.hpp" // Include the new node
#include "nodes/BlurNode.hpp"
#include "nodes/ThresholdNode.hpp"
#include <iostream>
#include <imgui.h>

// Function prototypes for better code organization
void showMenu();
void loadImage(std::shared_ptr<ImageInputNode> &inputNode, bool &imageLoaded);
void splitChannels(std::shared_ptr<ColorChannelSplitterNode> &splitterNode, std::shared_ptr<ImageInputNode> &inputNode, bool &channelsSplit, bool imageLoaded);
void convertToGrayscale(std::shared_ptr<ImageInputNode> &inputNode); // Convert the entire image to grayscale
void adjustBrightnessContrast(std::shared_ptr<BrightnessContrastNode> &bcNode, bool imageLoaded);
void saveOutputImage(std::shared_ptr<OutputNode> &outputNode, bool channelsSplit, std::shared_ptr<ImageInputNode> &inputNode, std::shared_ptr<ColorChannelSplitterNode> &splitterNode);
void mergeChannels(std::shared_ptr<ColorChannelSplitterNode> &splitterNode);                                    // Merge the channels back together
void applyBlur(std::shared_ptr<BlurNode> &blurNode, std::shared_ptr<ImageInputNode> &inputNode);                // Apply blur to the image
void applyThreshold(std::shared_ptr<ThresholdNode> &thresholdNode, std::shared_ptr<ImageInputNode> &inputNode); // Apply Thresholding to the image

int main()
{
    NodeGraph graph;

    // Create nodes
    auto inputNode = std::make_shared<ImageInputNode>("MyImage", "../assets/a.jpg");
    auto splitterNode = std::make_shared<ColorChannelSplitterNode>("Splitter", false);    // Disable grayscale initially
    auto outputNode = std::make_shared<OutputNode>("Result", "output_image", "jpg", 85);  // Quality is 85 for JPG
    auto bcNode = std::make_shared<BrightnessContrastNode>("BrightnessContrast", 1.0, 0); // Default contrast and brightness
    auto blurNode = std::make_shared<BlurNode>("GaussianBlur");                           // New blur node for blurring functionality
    auto thresholdNode = std::make_shared<ThresholdNode>("ThresholdNode");

    // Add to graph
    graph.addNode(inputNode);
    graph.addNode(splitterNode);
    graph.addNode(outputNode);
    graph.addNode(bcNode);   // Add the brightness/contrast node
    graph.addNode(blurNode); // Add the blur node
    graph.addNode(thresholdNode);

    int choice = 0;
    bool imageLoaded = false;
    bool channelsSplit = false;

    while (true)
    {
        showMenu();
        std::cout << "Enter your choice (1-8): ";
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
            applyBlur(blurNode, inputNode); // Apply blur to the image
            break;
        case 8:
            applyThreshold(thresholdNode, inputNode); // Apply thresholding
            break;
        case 9:
            std::cout << "Exiting program." << std::endl;
            return 0;
        default:
            std::cout << "Invalid choice. Please select a valid option." << std::endl;
        }
    }

    return 0;
}

// Show menu options
void showMenu()
{
    std::cout << "\nChoose an action:" << std::endl;
    std::cout << "1. Load Image" << std::endl;
    std::cout << "2. Split Color Channels" << std::endl;
    std::cout << "3. Convert to Grayscale" << std::endl; // Convert the whole image to grayscale
    std::cout << "4. Adjust Brightness and Contrast" << std::endl;
    std::cout << "5. Save Output Image" << std::endl;
    std::cout << "6. Merge Channels" << std::endl;      // New option to merge channels back
    std::cout << "7. Apply Blur to Image" << std::endl; // New option to apply blur
    std::cout << "8. Apply Thresholding" << std::endl;  // Option for thresholding
    std::cout << "9. Exit" << std::endl;
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
        std::cout << "Enter contrast (alpha) value (1.0 is normal): ";
        std::cin >> alpha;
        std::cout << "Enter brightness (beta) value (0 is normal): ";
        std::cin >> beta;
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
        outputNode->setInput(inputNode->getOutput()); // Use the adjusted image from the input node
        outputNode->process();
        std::cout << "Output image saved (adjusted brightness/contrast)." << std::endl;
    }
    else
    {
        std::cout << "Please split the color channels first to save specific channels." << std::endl;
    }
}

// Function to merge the channels back into a single image
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

// Function to apply blur to the image
void applyBlur(std::shared_ptr<BlurNode> &blurNode, std::shared_ptr<ImageInputNode> &inputNode)
{
    blurNode->setInput(inputNode->getOutput());
    std::cout << "What type blur you require? (D/G)" << std::endl;
    std::string blurtype;
    std::cin >> blurtype;
    if (blurtype == "D" || blurtype == "d")
    {
        int radius;
        float angle;
        std::cout << "Specify Radius and Angle." << std::endl;
        std::cout << "Radius: ";
        std::cin >> radius;
        std::cout << "Angle: ";
        std::cin >> angle;
        blurNode->setDirectional(true);
        blurNode->setRadius(radius);
        blurNode->setAngle(angle);
        blurNode->process();
    }
    if (blurtype == "G" || blurtype == "g")
    {
        int radius;
        std::cout << "Specify Radius: ";
        std::cin >> radius;
        blurNode->setRadius(radius);     // Set default blur radius
        blurNode->setDirectional(false); // Set to uniform blur
        blurNode->process();             // Apply blur effect
    }
    cv::Mat blurredImage = blurNode->getOutput();
    if (!blurredImage.empty())
    {
        cv::imwrite("Blurred_Image.png", blurredImage); // Save the blurred image
        std::cout << "Blurred image saved." << std::endl;
    }
    else
    {
        std::cout << "Failed to apply blur." << std::endl;
    }
}

void applyThreshold(std::shared_ptr<ThresholdNode>& thresholdNode, std::shared_ptr<ImageInputNode>& inputNode) {
    // Set the input and apply thresholding
    thresholdNode->setInput(inputNode->getOutput());
    thresholdNode->process();

    // Get the thresholded output and original input
    cv::Mat thresholdedImage = thresholdNode->getOutput();
    cv::Mat originalImage = inputNode->getOutput();

    // Convert the original image to grayscale (if it's not already)
    if (originalImage.channels() != 1) {
        cv::cvtColor(originalImage, originalImage, cv::COLOR_BGR2GRAY); // Convert to grayscale
    }

    // Show the original and thresholded images side-by-side
    cv::imshow("Original Image", originalImage);
    cv::imshow("Thresholded Image", thresholdedImage);
    cv::waitKey(0);  // Wait for a key press to close the windows

    // Calculate the difference between the original and thresholded images
    cv::Mat diffImage;
    cv::absdiff(originalImage, thresholdedImage, diffImage);

    // Show the difference image
    cv::imshow("Difference Image", diffImage);
    cv::waitKey(0);  // Wait for a key press to close the window

    // Optionally save the difference image
    cv::imwrite("Difference_Image.png", diffImage);

    std::cout << "Difference image saved." << std::endl;
}
