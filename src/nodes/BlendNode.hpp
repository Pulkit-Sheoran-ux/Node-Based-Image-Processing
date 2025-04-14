#pragma once
#include "../graph/Node.hpp"
#include <opencv2/opencv.hpp>

class BlendNode : public Node {
public:
    enum BlendMode {
        NORMAL,
        MULTIPLY,
        SCREEN,
        OVERLAY,
        DIFFERENCE
    };

    BlendNode(const std::string& name);

    void setInputA(const cv::Mat& image);
    void setInputB(const cv::Mat& image);
    void setBlendMode(BlendMode mode);
    void setOpacity(float value);  // 0.0 to 1.0

    void process() override;
    void renderUI() override;
    cv::Mat getOutput() const override;

private:
    cv::Mat inputA;
    cv::Mat inputB;
    cv::Mat outputImage;
    BlendMode blendMode = NORMAL;
    float opacity = 1.0f;
};
