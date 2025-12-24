#include "CameraSource.hpp"
#include <stdexcept>

CameraSource::CameraSource(int index) : cap_(index) {
    if (!cap_.isOpened()) {
        throw std::runtime_error("Impossible d'ouvrir la caméra");
    }
}

bool CameraSource::getFrame(cv::Mat& frame) {
    return cap_.read(frame);
}
