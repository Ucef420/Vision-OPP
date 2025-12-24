#pragma once
#include "IFilter.hpp"

class DummyFilter : public IFilter {
public:
    void apply(cv::Mat& frame) override;
};
