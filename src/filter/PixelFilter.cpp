#include "PixelFilter.hpp"
#include <opencv2/imgproc.hpp>

void PixelFilter::apply(cv::Mat& frame) {
    cv::Mat small;
    cv::resize(frame, small, cv::Size(), 0.1, 0.1, cv::INTER_LINEAR);
    cv::resize(small, frame, frame.size(), 0, 0, cv::INTER_NEAREST);
}
