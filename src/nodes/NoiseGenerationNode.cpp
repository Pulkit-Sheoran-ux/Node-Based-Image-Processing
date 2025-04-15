#include "NoiseGenerationNode.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>

NoiseGeneratorNode::NoiseGeneratorNode(const std::string &id, const std::string &name)
{
    this->id = id;
    this->name = name;
    this->nodeType = NodeType::Processing;
}

void NoiseGeneratorNode::setNoiseType(NoiseType type)
{
    noiseType = type;
    switch (noiseType)
    {
    case NoiseType::Perlin:
        fastNoiseLite.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
        break;
    case NoiseType::Simplex:
        fastNoiseLite.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
        break;
    case NoiseType::Worley:
        fastNoiseLite.SetNoiseType(FastNoiseLite::NoiseType_Cellular);
        break;
    }
}

void NoiseGeneratorNode::setInput(const cv::Mat &input)
{
    inputImage = input;
}

void NoiseGeneratorNode::setScale(float scale)
{
    this->scale = scale;
    fastNoiseLite.SetFrequency(scale);
}

void NoiseGeneratorNode::setOctaves(int octaves)
{
    this->octaves = octaves;
    fastNoiseLite.SetFractalOctaves(octaves);
}

void NoiseGeneratorNode::setPersistence(float persistence)
{
    this->persistence = persistence;
    fastNoiseLite.SetFractalGain(persistence);
}

void NoiseGeneratorNode::setUseAsDisplacement(bool useAsDisplacement)
{
    this->useAsDisplacement = useAsDisplacement;
}

void NoiseGeneratorNode::process()
{
    if (inputImage.empty())
        return;

    generateNoise();

    cv::Mat inputFloat;
    inputImage.convertTo(inputFloat, CV_32FC3, 1.0 / 255.0); // force 3-channel float

    if (useAsDisplacement)
    {
        cv::Mat noiseResized;
        cv::resize(output, noiseResized, inputImage.size());
        cv::Mat displacementMap;
        cv::merge(std::vector<cv::Mat>{noiseResized, noiseResized, noiseResized}, displacementMap);
        displacementMap.convertTo(displacementMap, CV_32FC3);

        float displacementStrength = 20.0f; // in pixels
        cv::Mat mapX(inputImage.size(), CV_32FC1);
        cv::Mat mapY(inputImage.size(), CV_32FC1);

        for (int y = 0; y < inputImage.rows; ++y)
        {
            for (int x = 0; x < inputImage.cols; ++x)
            {
                float disp = (displacementMap.at<cv::Vec3f>(y, x)[0] - 0.5f) * 2.0f * displacementStrength;
                mapX.at<float>(y, x) = x + disp;
                mapY.at<float>(y, x) = y + disp;
            }
        }

        cv::Mat warped;
        cv::remap(inputFloat, warped, mapX, mapY, cv::INTER_LINEAR, cv::BORDER_REFLECT);
        warped.convertTo(output, CV_8UC3, 255.0);
    }
    else
    {
        cv::Mat noiseResized;
        cv::resize(output, noiseResized, inputImage.size());
        cv::Mat noiseColor;
        cv::merge(std::vector<cv::Mat>{noiseResized, noiseResized, noiseResized}, noiseColor);
        noiseColor.convertTo(noiseColor, CV_32FC3);

        float noiseStrength = 0.2f;
        cv::Mat combined = inputFloat + noiseColor * noiseStrength;
        cv::normalize(combined, combined, 0, 1, cv::NORM_MINMAX);
        combined.convertTo(output, CV_8UC3, 255.0);
    }
}

void NoiseGeneratorNode::renderUI()
{
    std::cout << "Rendering UI for Noise Generator Node" << std::endl;
}

cv::Mat NoiseGeneratorNode::getOutput() const
{
    return output;
}

void NoiseGeneratorNode::generateNoise()
{
    int width = inputImage.cols;
    int height = inputImage.rows;
    output = cv::Mat(height, width, CV_32F);

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            float noiseValue = fastNoiseLite.GetNoise(static_cast<float>(x), static_cast<float>(y));
            output.at<float>(y, x) = noiseValue;
        }
    }

    cv::normalize(output, output, 0, 1, cv::NORM_MINMAX, CV_32F);
}
