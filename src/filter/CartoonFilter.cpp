
#include "CartoonFilter.hpp"
#include <opencv2/imgproc.hpp>

void CartoonFilter::apply(cv::Mat& frame) {
    cv::Mat gray, edges, color;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
    cv::medianBlur(gray, gray, 7);
    cv::adaptiveThreshold(gray, edges, 255,
        cv::ADAPTIVE_THRESH_MEAN_C,
        cv::THRESH_BINARY, 9, 2);
    cv::bilateralFilter(frame, color, 9, 75, 75);
    cv::bitwise_and(color, color, frame, edges);
}
