#include "BackgroundCompositeFilter.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <algorithm>
#include <chrono>
#include <iostream>

double BackgroundCompositeFilter::nowSec() {
    using namespace std::chrono;
    return duration<double>(steady_clock::now().time_since_epoch()).count();
}

int BackgroundCompositeFilter::makeOdd(int x) {
    return (x % 2 == 0) ? (x + 1) : x;
}

BackgroundCompositeFilter::BackgroundCompositeFilter(std::shared_ptr<IBackgroundProvider> bg,
                                                     const std::string& onnxPath)
    : bg_(std::move(bg)) {
    matte_ = std::make_unique<DnnMatteEstimator>(onnxPath);
    t0_ = nowSec();
}

void BackgroundCompositeFilter::setupUI(const std::string& windowName, const std::string& prefix) {
    if (uiWindowCreatedFor_ == windowName) return;
    uiWindowCreatedFor_ = windowName;

    cv::createTrackbar(prefix + "BG ON",        windowName, &enabled_,   1);
    cv::createTrackbar(prefix + "ShowAlpha",    windowName, &showAlpha_, 1);
    cv::createTrackbar(prefix + "Feather",      windowName, &feather_,   40);
    cv::createTrackbar(prefix + "Cutoff x10",   windowName, &cutoffX10_, 10);
    cv::createTrackbar(prefix + "EMA mix x10",  windowName, &emaX10_,    10);
    cv::createTrackbar(prefix + "Downscale %",  windowName, &downPct_,   100);
}

void BackgroundCompositeFilter::syncFromUI() {
    enabled_   = enabled_ ? 1 : 0;
    showAlpha_ = showAlpha_ ? 1 : 0;

    feather_   = std::clamp(feather_,   0, 40);
    cutoffX10_ = std::clamp(cutoffX10_, 0, 10);
    emaX10_    = std::clamp(emaX10_,    0, 10);
    downPct_   = std::clamp(downPct_,  30, 100);
}

cv::Mat BackgroundCompositeFilter::postProcessAlpha(const cv::Mat& alpha01) {
    cv::Mat a = alpha01;

    const float cutoff = float(cutoffX10_) / 10.0f;
    if (cutoff > 0.0f) {
        cv::threshold(a, a, cutoff, 1.0, cv::THRESH_TOZERO);
        if (cutoff < 1.0f) a = (a - cutoff) * (1.0f / (1.0f - cutoff));
        cv::max(a, 0.0, a);
        cv::min(a, 1.0, a);
    }

    if (feather_ > 0) {
        int k = makeOdd(feather_);
        cv::GaussianBlur(a, a, cv::Size(k, k), 0);
        cv::max(a, 0.0, a);
        cv::min(a, 1.0, a);
    }

    const float ema = float(emaX10_) / 10.0f;
    if (ema > 0.0f) {
        if (alphaPrev_.empty() || alphaPrev_.size() != a.size()) {
            alphaPrev_ = a.clone();
        } else {
            alphaPrev_ = alphaPrev_ * ema + a * (1.0f - ema);
        }
        a = alphaPrev_;
    } else {
        alphaPrev_.release();
    }

    return a;
}

void BackgroundCompositeFilter::apply(cv::Mat& frame) {
    if (!enabled_ || frame.empty() || !bg_ || !matte_) return;

    // 1) Optionnel : downscale pour perf
    cv::Mat small = frame;
    if (downPct_ < 100) {
        cv::resize(frame, small, cv::Size(),
                   downPct_ / 100.0, downPct_ / 100.0, cv::INTER_LINEAR);
    }

    // 2) IMPORTANT: MODNet + OpenCV DNN -> entrée stable (sinon mismatch 16 vs 21)
    // On force 512x512 (tu peux essayer 256 aussi si tu veux + rapide)
    const int NET_W = 512;
    const int NET_H = 512;

    cv::Mat netIn;
    cv::resize(small, netIn, cv::Size(NET_W, NET_H), 0, 0, cv::INTER_LINEAR);

    // 3) Alpha sur image réseau
    cv::Mat alphaNet = matte_->computeAlpha(netIn);
    if (alphaNet.empty()) {
        std::cerr << "[BackgroundCompositeFilter] MODNet forward failed: "
                  << matte_->lastError() << "\n";
        return;
    }

    // 4) Remettre alpha à la taille "small", puis à la taille frame
    cv::Mat alphaSmall;
    cv::resize(alphaNet, alphaSmall, small.size(), 0, 0, cv::INTER_LINEAR);

    cv::Mat alpha;
    if (alphaSmall.size() != frame.size()) {
        cv::resize(alphaSmall, alpha, frame.size(), 0, 0, cv::INTER_LINEAR);
    } else {
        alpha = alphaSmall;
    }

    alpha = postProcessAlpha(alpha);

    if (showAlpha_) {
        cv::Mat vis;
        alpha.convertTo(vis, CV_8U, 255.0);
        cv::cvtColor(vis, frame, cv::COLOR_GRAY2BGR);
        return;
    }

    // 5) Background + composite
    const double t = nowSec() - t0_;
    cv::Mat bg = bg_->next(frame.cols, frame.rows, t);
    if (bg.empty()) return;
    if (bg.size() != frame.size()) cv::resize(bg, bg, frame.size());

    cv::Mat f32, b32;
    frame.convertTo(f32, CV_32F);
    bg.convertTo(b32, CV_32F);

    std::vector<cv::Mat> chF, chB;
    cv::split(f32, chF);
    cv::split(b32, chB);

    for (int c = 0; c < 3; ++c) {
        chF[c] = chF[c].mul(alpha) + chB[c].mul(1.0 - alpha);
    }

    cv::Mat out;
    cv::merge(chF, out);
    out.convertTo(frame, CV_8U);
}
