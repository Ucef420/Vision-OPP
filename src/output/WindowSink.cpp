#include "WindowSink.hpp"
#include <opencv2/opencv.hpp>

WindowSink::WindowSink(const std::string& name)
    : windowName_(name) {
    cv::namedWindow(windowName_);
}

void WindowSink::handleFrame(const cv::Mat& frame) {
    cv::imshow(windowName_, frame);
}
