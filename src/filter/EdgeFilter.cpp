#include "EdgeFilter.hpp"
#include <opencv2/imgproc.hpp>
#include <algorithm>
#include <opencv2/highgui.hpp>
void EdgeFilter::setupUI(const std::string& windowName,
                         const std::string& prefix) {
    if (uiWindowCreatedFor_ == windowName) return;
    uiWindowCreatedFor_ = windowName;

    cv::createTrackbar(prefix + "Canny T1", windowName, &t1_, 500);
    cv::createTrackbar(prefix + "Canny T2", windowName, &t2_, 500);
    cv::createTrackbar(prefix + "Blur K",   windowName, &blurK_, 31);
    cv::createTrackbar(prefix + "Sigma x10",windowName, &sigma10_, 100);
}

void EdgeFilter::syncFromUI() {
    blurK_ = std::max(1, blurK_);
    if (blurK_ % 2 == 0) blurK_++;
}

void EdgeFilter::apply(cv::Mat& frame) {
    cv::Mat gray, edges;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

    cv::GaussianBlur(gray, gray,
                     cv::Size(blurK_, blurK_),
                     sigma10_ / 10.0);

    cv::Canny(gray, edges, t1_, t2_);
    cv::cvtColor(edges, frame, cv::COLOR_GRAY2BGR);
}
