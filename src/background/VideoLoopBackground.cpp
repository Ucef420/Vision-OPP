#include "VideoLoopBackground.hpp"
#include <opencv2/imgproc.hpp>

VideoLoopBackground::VideoLoopBackground(const std::string& path) {
    cap_.open(path);
}

cv::Mat VideoLoopBackground::next(int w, int h, double /*t*/) {
    cv::Mat frame;
    if (cap_.isOpened()) {
        if (!cap_.read(frame) || frame.empty()) {
            cap_.set(cv::CAP_PROP_POS_FRAMES, 0);
            cap_.read(frame);
        }
    }

    if (frame.empty()) {
        // fallback si vidéo pas dispo
        frame = cv::Mat(h, w, CV_8UC3, cv::Scalar(30,30,30));
    }

    cv::resize(frame, frame, cv::Size(w, h));
    last_ = frame;
    return frame;
}
