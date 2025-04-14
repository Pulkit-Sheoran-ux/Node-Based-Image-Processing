#include "NoiseGenerationNode.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>

NoiseGeneratorNode::NoiseGeneratorNode(const std::string& id, const std::string& name) {
    this->id = id;
    this->name = name;
    this->nodeType = NodeType::Processing;
}

void NoiseGeneratorNode::setNoiseType(NoiseType type) {
    noiseType = type;
    switch (noiseType) {
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

void NoiseGeneratorNode::setScale(float scale) {
    this->scale = scale;
    fastNoiseLite.SetFrequency(scale);  
}

void NoiseGeneratorNode::setOctaves(int octaves) {
    this->octaves = octaves;
    fastNoiseLite.SetFractalOctaves(octaves);  
}

void NoiseGeneratorNode::setPersistence(float persistence) {
    this->persistence = persistence;
    fastNoiseLite.SetFractalGain(persistence);  
}

void NoiseGeneratorNode::setUseAsDisplacement(bool useAsDisplacement) {
    this->useAsDisplacement = useAsDisplacement;
}

void NoiseGeneratorNode::process() {
    generateNoise();
}

void NoiseGeneratorNode::renderUI() {
    std::cout << "Rendering UI for Noise Generator Node" << std::endl;
}

cv::Mat NoiseGeneratorNode::getOutput() const {
    return output;
}

void NoiseGeneratorNode::generateNoise() {
    int width = 512, height = 512;
    output = cv::Mat(height, width, CV_32F);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            float noiseValue = fastNoiseLite.GetNoise(static_cast<float>(x), static_cast<float>(y));
            output.at<float>(y, x) = noiseValue;
        }
    }
    cv::normalize(output, output, 0, 1, cv::NORM_MINMAX);
}
