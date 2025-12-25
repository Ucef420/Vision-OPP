#include "NegativeFilter.hpp"
#include <opencv2/imgproc.hpp>

void NegativeFilter::apply(cv::Mat& frame) {
    // frame arrive souvent en BGR (webcam)
    cv::bitwise_not(frame, frame);
}