#include "graph/NodeGraph.hpp"
#include "nodes/EdgeDetectionNode.hpp"
#include "nodes/ImageInputNode.hpp"
#include "nodes/ColorChannelSplitterNode.hpp"
#include "nodes/OutputNode.hpp"
#include "nodes/BrightnessContrastNode.hpp"
#include "nodes/BlurNode.hpp"
#include "nodes/ThresholdNode.hpp"
#include "nodes/EdgeDetectionNode.hpp"
#include "nodes/BlendNode.hpp"
#include "nodes/NoiseGenerationNode.hpp"
#include "nodes/ConvolutionFilterNode.hpp"
#include <iostream>
#include <imgui.h>
#include <memory>

void showMenu();
void loadImage(std::shared_ptr<ImageInputNode> &inputNode, bool &imageLoaded);
void splitChannels(std::shared_ptr<ColorChannelSplitterNode> &splitterNode, std::shared_ptr<ImageInputNode> &inputNode, bool &channelsSplit, bool imageLoaded);
void convertToGrayscale(std::shared_ptr<ImageInputNode> &inputNode);
void adjustBrightnessContrast(std::shared_ptr<BrightnessContrastNode> &bcNode, std::shared_ptr<ImageInputNode> &inputNode, bool imageLoaded);
void saveOutputImage(std::shared_ptr<OutputNode> &outputNode, bool channelsSplit, std::shared_ptr<ImageInputNode> &inputNode, std::shared_ptr<ColorChannelSplitterNode> &splitterNode);
void mergeChannels(std::shared_ptr<ColorChannelSplitterNode> &splitterNode);
void applyBlur(std::shared_ptr<BlurNode> &blurNode, std::shared_ptr<ImageInputNode> &inputNode);
void applyThreshold(std::shared_ptr<ThresholdNode> &thresholdNode, std::shared_ptr<ImageInputNode> &inputNode);
void applyEdgeDetection(std::shared_ptr<EdgeDetectionNode> &edgeNode, std::shared_ptr<ImageInputNode> &inputNode);
void applyBlend(std::shared_ptr<BlendNode> &blendNode, std::shared_ptr<ImageInputNode> &inputNode);
void applyNoise(std::shared_ptr<NoiseGeneratorNode> &noiseNode, std::shared_ptr<ImageInputNode> &inputNode);
void convolutionfilter(std::shared_ptr<ConvolutionFilterNode> &convoNode, std::shared_ptr<ImageInputNode> &inputNode);

int main()
{
    NodeGraph graph;

    auto inputNode = std::make_shared<ImageInputNode>("MyImage", "../assets/a.jpg");
    auto splitterNode = std::make_shared<ColorChannelSplitterNode>("Splitter", false);
    auto outputNode = std::make_shared<OutputNode>("Result", "output_image", "jpg", 85);
    auto bcNode = std::make_shared<BrightnessContrastNode>("BrightnessContrast", 1.0, 0);
    auto blurNode = std::make_shared<BlurNode>("GaussianBlur");
    auto thresholdNode = std::make_shared<ThresholdNode>("ThresholdNode");
    auto edgedetectionNode = std::make_shared<EdgeDetectionNode>("EdgeDetection");
    auto blendNode = std::make_shared<BlendNode>("Blend");
    auto noiseNode = std::make_shared<NoiseGeneratorNode>("NoiseID", "NoiseGenerator");
    auto convoNode = std::make_shared<ConvolutionFilterNode>("conv1", "ConvFilter");

    graph.addNode(inputNode);
    graph.addNode(splitterNode);
    graph.addNode(outputNode);
    graph.addNode(bcNode);
    graph.addNode(blurNode);
    graph.addNode(thresholdNode);
    graph.addNode(edgedetectionNode);
    graph.addNode(blendNode);
    graph.addNode(noiseNode);
    graph.addNode(convoNode);

    int choice = 0;
    bool imageLoaded = false;
    bool channelsSplit = false;

    while (true)
    {
        showMenu();
        std::cout << "Enter your choice (1-13): ";
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
            convertToGrayscale(inputNode);
            break;
        case 4:
            adjustBrightnessContrast(bcNode, inputNode, imageLoaded);
            break;
        case 5:
            saveOutputImage(outputNode, channelsSplit, inputNode, splitterNode);
            break;
        case 6:
            mergeChannels(splitterNode);
            break;
        case 7:
            applyBlur(blurNode, inputNode);
            break;
        case 8:
            applyThreshold(thresholdNode, inputNode);
            break;
        case 9:
            applyEdgeDetection(edgedetectionNode, inputNode);
            break;
        case 10:
            applyBlend(blendNode, inputNode);
            break;
        case 11:
            applyNoise(noiseNode, inputNode);
            break;
        case 12:
            convolutionfilter(convoNode, inputNode);
            break;
        case 13:
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
    std::cout << "3. Convert to Grayscale" << std::endl;
    std::cout << "4. Adjust Brightness and Contrast" << std::endl;
    std::cout << "5. Save Output Image" << std::endl;
    std::cout << "6. Merge Channels" << std::endl;
    std::cout << "7. Apply Blur to Image" << std::endl;
    std::cout << "8. Apply Thresholding" << std::endl;
    std::cout << "9. Edge Detection" << std::endl;
    std::cout << "10. Blend Images" << std::endl;
    std::cout << "11. Generate Noise" << std::endl;
    std::cout << "12. Apply Convolution Noise" << std::endl;
    std::cout << "13. Exit" << std::endl;
}

void loadImage(std::shared_ptr<ImageInputNode> &inputNode, bool &imageLoaded)
{
    if (!imageLoaded)
    {
        std::string imagename;
        std::string s = "../assets/";

        while (true)
        {
            std::cout << "Enter the image file name: ";
            std::cin >> imagename;
            std::string imagePath = s + imagename;

            cv::Mat img = cv::imread(imagePath, cv::IMREAD_COLOR);
            if (img.empty())
            {
                std::cout << "Error: Image not found or could not be loaded. Please try again.\n";
            }
            else
            {
                inputNode = std::make_shared<ImageInputNode>("MyImage", imagePath);
                inputNode->process();
                std::cout << "Image loaded successfully!" << std::endl;
                imageLoaded = true;
                break;
            }
        }
    }
    else
    {
        std::cout << "Image is already loaded." << std::endl;
    }
}

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

void convertToGrayscale(std::shared_ptr<ImageInputNode> &inputNode)
{
    inputNode->convertToGrayscale();
}

void adjustBrightnessContrast(std::shared_ptr<BrightnessContrastNode> &bcNode, std::shared_ptr<ImageInputNode> &inputNode, bool imageLoaded)
{
    if (imageLoaded)
    {
        bcNode->setInput(inputNode->getOutput());
        float alpha, beta;
        std::cout << "Enter contrast (alpha) value (1.0 is normal): ";
        std::cin >> alpha;
        std::cout << "Enter brightness (beta) value (0 is normal): ";
        std::cin >> beta;
        bcNode->setParams(alpha, beta);
        bcNode->process();
        std::cout << "Brightness and contrast adjusted." << std::endl;
        cv::Mat out = bcNode->getOutput();
        if (!out.empty())
        {
            cv::imwrite("Bright.png", out);
            std::cout << "Bright image saved." << std::endl;
        }
    }

    else
    {
        std::cout << "Please load an image first." << std::endl;
    }
}

void saveOutputImage(std::shared_ptr<OutputNode> &outputNode, bool channelsSplit, std::shared_ptr<ImageInputNode> &inputNode, std::shared_ptr<ColorChannelSplitterNode> &splitterNode)
{
    if (!channelsSplit)
    {
        outputNode->setInput(inputNode->getOutput());
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
    cv::Mat mergedImage = splitterNode->mergeChannels();
    if (!mergedImage.empty())
    {
        cv::imwrite("Merged_Image.png", mergedImage);
        std::cout << "Merged image saved." << std::endl;
    }
    else
    {
        std::cout << "Failed to merge channels." << std::endl;
    }
}

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
        blurNode->setRadius(radius);
        blurNode->setDirectional(false);
        blurNode->process();
    }
    cv::Mat blurredImage = blurNode->getOutput();
    if (!blurredImage.empty())
    {
        cv::imwrite("Blurred_Image.png", blurredImage);
        std::cout << "Blurred image saved." << std::endl;
    }
    else
    {
        std::cout << "Failed to apply blur." << std::endl;
    }
}

void applyThreshold(std::shared_ptr<ThresholdNode> &thresholdNode, std::shared_ptr<ImageInputNode> &inputNode)
{
    thresholdNode->setInput(inputNode->getOutput());
    std::cout << "Which type of Threhold you want? (B/A/O)" << std::endl;
    std::string thresholdtype;
    std::cin >> thresholdtype;
    if (thresholdtype == "B")
    {
        thresholdNode->setThresholdType(ThresholdNode::BINARY);
    }
    if (thresholdtype == "A")
    {
        thresholdNode->setThresholdType(ThresholdNode::ADAPTIVE);
    }
    if (thresholdtype == "O")
    {
        thresholdNode->setThresholdType(ThresholdNode::OTSU);
    }

    thresholdNode->process();

    cv::Mat thresholdedImage = thresholdNode->getOutput();
    cv::Mat originalImage = inputNode->getOutput();

    if (originalImage.channels() != 1)
    {
        cv::cvtColor(originalImage, originalImage, cv::COLOR_BGR2GRAY);
    }

    cv::imshow("Original Image", originalImage);
    cv::imshow("Thresholded Image", thresholdedImage);
    cv::waitKey(0);
    cv::Mat diffImage;
    cv::absdiff(originalImage, thresholdedImage, diffImage);

    // Show the difference image
    cv::imshow("Difference Image", diffImage);
    cv::waitKey(0);

    cv::imwrite("Difference_Image.png", diffImage);

    std::cout << "Difference image saved." << std::endl;
}

void applyEdgeDetection(std::shared_ptr<EdgeDetectionNode> &edgeNode, std::shared_ptr<ImageInputNode> &inputNode)
{
    edgeNode->setInput(inputNode->getOutput());
    std::cout << "Give type of Edge Detection Algorithm. (Sobel/Canny)" << std::endl;
    std::string Algotype;
    std::cin >> Algotype;
    if (Algotype == "Sobel" || Algotype == "sobel")
    {
        edgeNode->setEdgeDetectionType(EdgeDetectionNode::SOBEL);
        std::cout << "Give Sobel Kernel Size: ";
        int kernelsize;
        std::cin >> kernelsize;
        edgeNode->setSobelKernelSize(kernelsize);
    }
    else if (Algotype == "Canny" || Algotype == "canny")
    {
        edgeNode->setEdgeDetectionType(EdgeDetectionNode::CANNY);
        std::cout << "Give Canny Threshold values." << std::endl;
        int threshold1, threshold2;
        std::cout << "Threshold1 value: " << std::endl;
        std::cin >> threshold1;
        std::cout << "Threshold2 value: " << std::endl;
        std::cin >> threshold2;
        edgeNode->setCannyThresholds(threshold1, threshold2);
    }
    else
    {
        std::cout << "Invalid choice! Defaulting to Sobel." << std::endl;
        edgeNode->setEdgeDetectionType(EdgeDetectionNode::SOBEL);
        std::cout << "Give Sobel Kernel Size: ";
        int kernelsize;
        std::cin >> kernelsize;
        edgeNode->setSobelKernelSize(kernelsize);
    }

    std::cout << "Do you want to overlay edges on the original image? (yes/no): ";
    std::string overlayInput;
    std::cin >> overlayInput;

    bool overlay = (overlayInput == "yes" || overlayInput == "Yes" || overlayInput == "y");
    edgeNode->setOverlayEdges(overlay);

    edgeNode->process();

    cv::Mat edgeDetectedImage = edgeNode->getOutput();

    if (!edgeDetectedImage.empty())
    {
        cv::GaussianBlur(edgeDetectedImage, edgeDetectedImage, cv::Size(5, 5), 0);
        // cv::bitwise_not(edgeDetectedImage, edgeDetectedImage);
        cv::imwrite("Soft_Edge_Detected_Image.png", edgeDetectedImage);
        std::cout << "Edge detection applied, softened";
        if (overlay)
            std::cout << ", overlaid";
        std::cout << ", and saved as Soft_Edge_Detected_Image.png." << std::endl;

        cv::imshow("Final Edge Detected Image", edgeDetectedImage);
        cv::waitKey(0);
    }
    else
    {
        std::cout << "Edge detection failed. The output image is empty." << std::endl;
    }
}

void applyBlend(std::shared_ptr<BlendNode> &blendNode, std::shared_ptr<ImageInputNode> &inputNode)
{
    std::string secondImageName;
    std::cout << "Enter path of second image: ";
    std::cin >> secondImageName;

    std::shared_ptr<ImageInputNode> secondInput = std::make_shared<ImageInputNode>("SecondImage", secondImageName);
    secondInput->process();

    blendNode->setInputA(inputNode->getOutput());
    blendNode->setInputB(secondInput->getOutput());

    std::string blendMode;
    std::cout << "Enter blend mode (normal/multiply/screen/overlay/difference): ";
    std::cin >> blendMode;
    if (blendMode == "normal")
    {
        blendNode->setBlendMode(BlendNode::NORMAL);
    }
    if (blendMode == "multiply")
    {
        blendNode->setBlendMode(BlendNode::MULTIPLY);
    }
    if (blendMode == "screen")
    {
        blendNode->setBlendMode(BlendNode::SCREEN);
    }
    if (blendMode == "overlay")
    {
        blendNode->setBlendMode(BlendNode::OVERLAY);
    }
    if (blendMode == "diffrence")
    {
        blendNode->setBlendMode(BlendNode::DIFFERENCE);
    }

    float opacity;
    std::cout << "Enter mix/opacity (0.0 - 1.0): ";
    std::cin >> opacity;
    blendNode->setOpacity(opacity);

    blendNode->process();

    cv::Mat blendedImage = blendNode->getOutput();
    if (!blendedImage.empty())
    {
        cv::imwrite("Blended_Image.png", blendedImage);
        std::cout << "Blended image saved as Blended_Image.png" << std::endl;
        cv::imshow("Blended Image", blendedImage);
        cv::waitKey(0);
    }
    else
    {
        std::cout << "Failed to blend images" << std::endl;
    }
}

void applyNoise(std::shared_ptr<NoiseGeneratorNode> &noiseNode, std::shared_ptr<ImageInputNode> &inputNode)
{
    std::string type;
    int octaves;
    float persistence, scale;
    noiseNode->setInput(inputNode->getOutput());
    while (true)
    {
        std::cout << "Enter Noise Type (perlin/simplex/worley): ";
        std::cin >> type;
        if (type == "perlin")
        {
            noiseNode->setNoiseType(NoiseGeneratorNode::NoiseType::Perlin);
            break;
        }
        else if (type == "simplex")
        {
            noiseNode->setNoiseType(NoiseGeneratorNode::NoiseType::Simplex);
            break;
        }
        else if (type == "worley")
        {
            noiseNode->setNoiseType(NoiseGeneratorNode::NoiseType::Worley);
            break;
        }
        else
        {
            std::cout << "Invalid noise type. Please try again." << std::endl;
        }
    }

    while (true)
    {
        std::cout << "Enter Scale (float): ";
        std::cin >> scale;
        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input for scale. Please enter an integer." << std::endl;
        }
        else
        {
            noiseNode->setScale(scale);
            break;
        }
    }

    while (true)
    {
        std::cout << "Enter Octaves (int): ";
        std::cin >> octaves;
        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input for octaves. Please enter an integer." << std::endl;
        }
        else
        {
            noiseNode->setOctaves(octaves);
            break;
        }
    }

    while (true)
    {
        std::cout << "Enter Persistence (float): ";
        std::cin >> persistence;
        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input for persistence. Please enter a floating-point number." << std::endl;
        }
        else
        {
            noiseNode->setPersistence(persistence);
            break;
        }
    }

    std::string usage;
    while (true)
    {
        std::cout << "Use noise as color or displacement? (c/d): ";
        std::cin >> usage;
        if (usage == "c" || usage == "C")
        {
            noiseNode->setUseAsDisplacement(false);
            break;
        }
        else if (usage == "d" || usage == "D")
        {
            noiseNode->setUseAsDisplacement(true);
            break;
        }
        else
        {
            std::cout << "Invalid option. Please enter 'c' for color or 'd' for displacement." << std::endl;
        }
    }

    noiseNode->process();

    cv::Mat result = noiseNode->getOutput();
    if (!result.empty())
    {
        cv::Mat displayResult;
        result.convertTo(displayResult, CV_8U, 255.0);

        std::string filename = usage == "d" ? "Displacement_Noise.png" : "Color_Noise.png";
        cv::imwrite(filename, displayResult);
        std::cout << "Noise image saved as " << filename << std::endl;
    }
    else
    {
        std::cout << "Failed to generate noise." << std::endl;
    }
}

void convolutionfilter(std::shared_ptr<ConvolutionFilterNode> &convoNode, std::shared_ptr<ImageInputNode> &inputNode)
{
    convoNode->setInput(inputNode->getOutput());

    std::cout << "Choose a preset filter:\n";
    std::cout << "1. Sharpen\n2. Emboss\n3. Edge Enhance\n4. Custom\n";
    int choice;
    std::cin >> choice;

    switch (choice)
    {
    case 1:
        convoNode->setPreset(ConvolutionFilterNode::PresetType::Sharpen);
        break;
    case 2:
        convoNode->setPreset(ConvolutionFilterNode::PresetType::Emboss);
        break;
    case 3:
        convoNode->setPreset(ConvolutionFilterNode::PresetType::EdgeEnhance);
        break;
    case 4:
    {
        int kernelSize;
        std::cout << "Enter kernel size (3 or 5): ";
        std::cin >> kernelSize;
        if (kernelSize == 3 || kernelSize == 5)
        {
            std::vector<float> customKernel(kernelSize * kernelSize);
            std::cout << "Enter the values for the " << kernelSize << "x" << kernelSize << " kernel:\n";
            for (int i = 0; i < kernelSize * kernelSize; ++i)
            {
                std::cin >> customKernel[i];
            }
            convoNode->setCustomKernel(customKernel);
        }
        else
        {
            std::cout << "Invalid kernel size. Defaulting to sharpen filter.\n";
            convoNode->setPreset(ConvolutionFilterNode::PresetType::Sharpen);
        }
    }
    break;
    default:
        std::cout << "Invalid choice! Defaulting to sharpen filter.\n";
        convoNode->setPreset(ConvolutionFilterNode::PresetType::Sharpen);
        break;
    }

    convoNode->process();
    cv::Mat result = convoNode->getOutput();
    cv::imshow("Convolution Filter Effect", result);
    cv::waitKey(0);

    std::cout << "Filter applied! Saving the output image...\n";
    cv::imwrite("Filtered_Output.png", convoNode->getOutput());
}

