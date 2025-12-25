#include "PseudoColorFilter.hpp"
#include <opencv2/imgproc.hpp>


void PseudoColorFilter::apply(cv::Mat& frame) {
    cv::Mat gray, color;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
    cv::applyColorMap(gray, color, cv::COLORMAP_JET);
    frame = color;
}