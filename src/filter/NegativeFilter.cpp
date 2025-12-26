#include "NegativeFilter.hpp"
#include <opencv2/imgproc.hpp>
#include <algorithm>
#include <opencv2/highgui.hpp>
void NegativeFilter::setupUI(const std::string& windowName,
                             const std::string& prefix) {
    if (uiWindowCreatedFor_ == windowName) return;
    uiWindowCreatedFor_ = windowName;

    cv::createTrackbar(prefix + "Negative ON", windowName, &enabled_, 1);
    cv::createTrackbar(prefix + "Strength %",  windowName, &strength_, 100);
}

void NegativeFilter::syncFromUI() {
    enabled_  = enabled_ ? 1 : 0;
    strength_ = std::clamp(strength_, 0, 100);
}

void NegativeFilter::apply(cv::Mat& frame) {
    if (!enabled_) return;

    cv::Mat neg;
    cv::bitwise_not(frame, neg);

    double a = strength_ / 100.0;
    cv::addWeighted(neg, a, frame, 1.0 - a, 0.0, frame);
}
