#pragma once
#include "IFilter.hpp"

class PixelFilter : public IFilter {
public:
    void apply(cv::Mat& frame) override;
};