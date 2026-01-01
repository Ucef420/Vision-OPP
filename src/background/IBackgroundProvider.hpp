#pragma once
#include <opencv2/core.hpp>

class IBackgroundProvider {
public:
    virtual ~IBackgroundProvider() = default;
    // t = temps (secondes), w/h = taille frame caméra
    virtual cv::Mat next(int w, int h, double t) = 0;
};
