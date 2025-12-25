#pragma once
#include "IFilter.hpp"

class CartoonFilter : public IFilter {
public:
    void apply(cv::Mat& frame) override;
};