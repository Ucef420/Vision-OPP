#include "Pipeline.hpp"

void Pipeline::addFilter(std::unique_ptr<IFilter> filter) {
    filters_.push_back(std::move(filter));
}

void Pipeline::process(cv::Mat& frame) {
    for (auto& f : filters_) {
        f->apply(frame);
    }
}
