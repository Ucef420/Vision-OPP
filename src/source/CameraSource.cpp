#include "CameraSource.hpp"
#include <opencv2/opencv.hpp>
#include <stdexcept>
#include <iostream>

CameraSource::CameraSource(int index) {
    std::cerr << "[CameraSource] trying index=" << index << " api=DSHOW\n";

    cap_ = cv::VideoCapture(index, cv::CAP_DSHOW); // EXACTEMENT comme camera_test

    if (!cap_.isOpened()) {
        throw std::runtime_error("Impossible d'ouvrir la camera (CAP_DSHOW)");
    }
}

bool CameraSource::getFrame(cv::Mat& frame) {
    return cap_.read(frame);
}
