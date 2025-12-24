#include "CameraSource.hpp"
#include <stdexcept>
#include <opencv2/opencv.hpp>

CameraSource::CameraSource(int index) {
    if (!cap_.open(index, cv::CAP_DSHOW)) {
        throw std::runtime_error("Impossible d'ouvrir la caméra (CAP_DSHOW)");
    }
}

bool CameraSource::getFrame(cv::Mat& frame) {
    return cap_.read(frame);
}
