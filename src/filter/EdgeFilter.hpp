#pragma once
#include "IFilter.hpp"

class EdgeFilter : public IFilter {
public:
    void apply(cv::Mat& frame) override;
};