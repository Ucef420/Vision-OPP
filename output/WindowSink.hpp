#pragma once
#include "IOutputSink.hpp"
#include <string>

class WindowSink : public IOutputSink {
public:
    explicit WindowSink(const std::string& name);
    void handleFrame(const cv::Mat& frame) override;

private:
    std::string windowName_;
};
