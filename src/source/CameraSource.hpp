#pragma once
#include "IFrameSource.hpp"
#include <opencv2/opencv.hpp>

class CameraSource : public IFrameSource {
public:
    explicit CameraSource(int index = 0);
    bool getFrame(cv::Mat& frame) override;

private:
    cv::VideoCapture cap_;
};
