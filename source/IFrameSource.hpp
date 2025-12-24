#pragma once
#include <opencv2/opencv.hpp>

class IFrameSource {
public:
    virtual ~IFrameSource() = default;
    virtual bool getFrame(cv::Mat& frame) = 0;
};
