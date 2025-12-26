#include "MirrorFilter.hpp"
#include <opencv2/imgproc.hpp>

void MirrorFilter::apply(cv::Mat& frame) {
    cv::flip(frame, frame, 1);
}
