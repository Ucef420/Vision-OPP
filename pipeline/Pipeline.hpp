#pragma once
#include <vector>
#include <memory>
#include "filter/IFilter.hpp"

class Pipeline {
public:
    void addFilter(std::unique_ptr<IFilter> filter);
    void process(cv::Mat& frame);

private:
    std::vector<std::unique_ptr<IFilter>> filters_;
};
