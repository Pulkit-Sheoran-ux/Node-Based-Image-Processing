#pragma once

#include "../libs/FastNoiseLite.h"
#include "../graph/Node.hpp"
#include <opencv2/opencv.hpp>
#include <string>

class NoiseGeneratorNode : public Node {
    public:
        enum class NoiseType { Perlin, Simplex, Worley };
    
        NoiseGeneratorNode(const std::string& id, const std::string& name);
    
        // Configure noise parameters
        void setNoiseType(NoiseType type);
        void setScale(float scale);
        void setOctaves(int octaves);
        void setPersistence(float persistence);
        void setUseAsDisplacement(bool useAsDisplacement);
    
        // Override methods
        void process() override;
        void renderUI() override;
        cv::Mat getOutput() const override;
    
    private:
        void generateNoise(); // Generates the noise pattern
    
        NoiseType noiseType = NoiseType::Perlin; // Default noise type
        float scale = 1.0f;
        int octaves = 4;
        float persistence = 0.5f;
        bool useAsDisplacement = false;
    
        cv::Mat output; // The generated noise map
    
        // FastNoiseLite instance
        FastNoiseLite fastNoiseLite;
    };