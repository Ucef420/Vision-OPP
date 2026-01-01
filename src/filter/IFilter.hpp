#pragma once
#include <opencv2/core.hpp>

class IFilter {
public:
    virtual ~IFilter() = default;
    virtual void apply(cv::Mat& frame) = 0;
};
