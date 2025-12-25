#pragma once
#include "IFilter.hpp"

class PseudoColorFilter : public IFilter {
public:
    void apply(cv::Mat& frame) override;
};