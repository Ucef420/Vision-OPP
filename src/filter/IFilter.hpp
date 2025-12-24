#pragma once
#include <opencv2/opencv.hpp>

class IFilter {
public:
    virtual ~IFilter() = default;
    virtual void apply(cv::Mat& frame) = 0;
};
