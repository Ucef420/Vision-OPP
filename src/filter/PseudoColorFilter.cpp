#include "PseudoColorFilter.hpp"
#include <opencv2/imgproc.hpp>
#include <algorithm>
#include <opencv2/highgui.hpp>
static int mapIdToCv(int id) {
    static int maps[] = {
        cv::COLORMAP_AUTUMN, cv::COLORMAP_BONE, cv::COLORMAP_JET,
        cv::COLORMAP_WINTER, cv::COLORMAP_RAINBOW, cv::COLORMAP_OCEAN,
        cv::COLORMAP_SUMMER, cv::COLORMAP_SPRING, cv::COLORMAP_COOL,
        cv::COLORMAP_HSV, cv::COLORMAP_PINK, cv::COLORMAP_HOT,
        cv::COLORMAP_PARULA, cv::COLORMAP_MAGMA, cv::COLORMAP_INFERNO,
        cv::COLORMAP_PLASMA, cv::COLORMAP_VIRIDIS, cv::COLORMAP_CIVIDIS,
        cv::COLORMAP_TWILIGHT, cv::COLORMAP_TWILIGHT_SHIFTED,
        cv::COLORMAP_TURBO
    };
    return maps[std::clamp(id, 0, 20)];
}

void PseudoColorFilter::setupUI(const std::string& windowName,
                                const std::string& prefix) {
    if (uiWindowCreatedFor_ == windowName) return;
    uiWindowCreatedFor_ = windowName;

    cv::createTrackbar(prefix + "Colormap", windowName, &mapId_, 20);
    cv::createTrackbar(prefix + "Blend %",  windowName, &blend_, 100);
}

void PseudoColorFilter::syncFromUI() {
    mapId_ = std::clamp(mapId_, 0, 20);
    blend_ = std::clamp(blend_, 0, 100);
}

void PseudoColorFilter::apply(cv::Mat& frame) {
    cv::Mat gray, color;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
    cv::applyColorMap(gray, color, mapIdToCv(mapId_));

    double a = blend_ / 100.0;
    cv::addWeighted(color, a, frame, 1.0 - a, 0.0, frame);
}
