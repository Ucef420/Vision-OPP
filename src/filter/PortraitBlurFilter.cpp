#include "PortraitBlurFilter.hpp"
#include <opencv2/highgui.hpp>   // createTrackbar
#include <algorithm>

PortraitBlurFilter::PortraitBlurFilter(const std::string& faceCascadePath)
    : faceCascadePath_(faceCascadePath) {
    faceCascade_.load(faceCascadePath_);
}

int PortraitBlurFilter::makeOdd(int v) {
    v = std::max(1, v);
    if (v % 2 == 0) v += 1;
    return v;
}

cv::Rect PortraitBlurFilter::clampRect(const cv::Rect& r, int w, int h) {
    int x = std::clamp(r.x, 0, w - 1);
    int y = std::clamp(r.y, 0, h - 1);
    int rw = std::clamp(r.width,  1, w - x);
    int rh = std::clamp(r.height, 1, h - y);
    return cv::Rect(x, y, rw, rh);
}

void PortraitBlurFilter::setupUI(const std::string& windowName,
                                 const std::string& prefix) {
    if (uiWindowCreatedFor_ == windowName) return;
    uiWindowCreatedFor_ = windowName;

    cv::createTrackbar(prefix + "Portrait ON",     windowName, &enabled_,      1);
    cv::createTrackbar(prefix + "Blur K",          windowName, &blurK_,        99);
    cv::createTrackbar(prefix + "Feather",         windowName, &feather_,      80);
    cv::createTrackbar(prefix + "Expand X %",      windowName, &expandX_,      200);
    cv::createTrackbar(prefix + "Expand Top %",    windowName, &expandTop_,    200);
    cv::createTrackbar(prefix + "Expand Bottom %", windowName, &expandBottom_, 300);
    cv::createTrackbar(prefix + "Follow face",     windowName, &followFace_,   1);
}

void PortraitBlurFilter::syncFromUI() {
    enabled_    = enabled_ ? 1 : 0;
    followFace_ = followFace_ ? 1 : 0;

    blurK_        = makeOdd(std::clamp(blurK_, 1, 99));
    feather_      = std::clamp(feather_, 0, 80);
    expandX_      = std::clamp(expandX_, 0, 200);
    expandTop_    = std::clamp(expandTop_, 0, 200);
    expandBottom_ = std::clamp(expandBottom_, 0, 300);
}

void PortraitBlurFilter::apply(cv::Mat& frame) {
    if (!enabled_ || frame.empty()) return;
    if (faceCascade_.empty()) return;

    cv::Rect roi;
    bool got = false;

    if (!followFace_ && hasLast_) {
        roi = lastROI_;
        got = true;
    } else {
        cv::Mat gray;
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        cv::equalizeHist(gray, gray);

        std::vector<cv::Rect> faces;
        faceCascade_.detectMultiScale(gray, faces, 1.1, 4, 0, cv::Size(60, 60));
        if (!faces.empty()) {
            roi = *std::max_element(faces.begin(), faces.end(),
                [](const cv::Rect& a, const cv::Rect& b) { return a.area() < b.area(); });
            got = true;
        }
    }

    if (!got) return;

    int fx = roi.x, fy = roi.y, fw = roi.width, fh = roi.height;
    int ex = (fw * expandX_) / 100;
    int et = (fh * expandTop_) / 100;
    int eb = (fh * expandBottom_) / 100;

    cv::Rect big(fx - ex, fy - et, fw + 2 * ex, fh + et + eb);
    big = clampRect(big, frame.cols, frame.rows);

    lastROI_ = big;
    hasLast_ = true;

    cv::Mat blurred;
    cv::GaussianBlur(frame, blurred, cv::Size(blurK_, blurK_), 0);

    cv::Mat mask(frame.rows, frame.cols, CV_8UC1, cv::Scalar(0));
    cv::Point center(big.x + big.width / 2, big.y + big.height / 2);
    cv::Size axes((int)(big.width * 0.50), (int)(big.height * 0.50));
    cv::ellipse(mask, center, axes, 0, 0, 360, cv::Scalar(255), -1);

    if (feather_ > 0) {
        int fk = makeOdd(std::max(1, feather_));
        cv::GaussianBlur(mask, mask, cv::Size(fk, fk), 0);
    }

    cv::Mat maskF;
    mask.convertTo(maskF, CV_32F, 1.0 / 255.0);
    cv::Mat invMaskF = 1.0 - maskF;

    cv::Mat frameF, blurredF;
    frame.convertTo(frameF, CV_32F);
    blurred.convertTo(blurredF, CV_32F);

    std::vector<cv::Mat> chF(3), chB(3);
    cv::split(frameF, chF);
    cv::split(blurredF, chB);

    for (int c = 0; c < 3; ++c)
        chF[c] = chF[c].mul(maskF) + chB[c].mul(invMaskF);

    cv::Mat outF;
    cv::merge(chF, outF);
    outF.convertTo(frame, CV_8U);
}
