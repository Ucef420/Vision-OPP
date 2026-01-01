#pragma once
#include "IBackgroundProvider.hpp"
#include <opencv2/videoio.hpp>
#include <string>

class VideoLoopBackground : public IBackgroundProvider {
public:
    explicit VideoLoopBackground(const std::string& path);

    cv::Mat next(int w, int h, double t) override;

    bool isOpened() const { return cap_.isOpened(); }

private:
    cv::VideoCapture cap_;
    cv::Mat last_;
};
