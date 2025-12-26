#pragma once
#include "IFilter.hpp"

class MirrorFilter : public IFilter {
public:
    void apply(cv::Mat& frame) override;
};
