#pragma once
#include <opencv2/opencv.hpp>

class IOutputSink {
public:
    virtual ~IOutputSink() = default;
    virtual void handleFrame(const cv::Mat& frame) = 0;
};
