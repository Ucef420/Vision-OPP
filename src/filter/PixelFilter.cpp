#include "PixelFilter.hpp"
#include <opencv2/imgproc.hpp>
#include <algorithm>
#include <opencv2/highgui.hpp>
void PixelFilter::setupUI(const std::string& windowName,
                          const std::string& prefix) {
    if (uiWindowCreatedFor_ == windowName) return;
    uiWindowCreatedFor_ = windowName;

    cv::createTrackbar(prefix + "Pixel size", windowName, &blockSize_, 80);
    cv::createTrackbar(prefix + "Nearest",    windowName, &nearest_,   1);
}

void PixelFilter::syncFromUI() {
    blockSize_ = std::max(1, blockSize_);
    nearest_   = nearest_ ? 1 : 0;
}

void PixelFilter::apply(cv::Mat& frame) {
    int w = std::max(1, frame.cols / blockSize_);
    int h = std::max(1, frame.rows / blockSize_);

    cv::Mat small;
    cv::resize(frame, small, cv::Size(w, h), 0, 0, cv::INTER_AREA);
    cv::resize(small, frame, frame.size(), 0, 0,
               nearest_ ? cv::INTER_NEAREST : cv::INTER_LINEAR);
}
