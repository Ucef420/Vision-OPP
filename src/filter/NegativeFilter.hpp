#pragma once
#include "IFilter.hpp"

class NegativeFilter : public IFilter {
public:
    void apply(cv::Mat& frame) override;
};