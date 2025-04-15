#pragma once

#include "../libs/FastNoiseLite.h"
#include "../graph/Node.hpp"
#include <opencv2/opencv.hpp>
#include <string>

class NoiseGeneratorNode : public Node {
    public:
        enum class NoiseType { Perlin, Simplex, Worley };
    
        NoiseGeneratorNode(const std::string& id, const std::string& name);
    
        void setNoiseType(NoiseType type);
        void setScale(float scale);
        void setOctaves(int octaves);
        void setPersistence(float persistence);
        void setUseAsDisplacement(bool useAsDisplacement);
    
        void process() override;
        void renderUI() override;

        cv::Mat getOutput() const override;
        void setInput(const cv::Mat& input) override;
    private:
        void generateNoise(); 
    
        NoiseType noiseType = NoiseType::Perlin;
        float scale = 1.0f;
        int octaves = 4;
        float persistence = 0.5f;
        bool useAsDisplacement = false;
        cv::Mat inputImage;
        cv::Mat output;
    
        FastNoiseLite fastNoiseLite;
    };