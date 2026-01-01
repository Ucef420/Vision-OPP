#pragma once
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/dnn.hpp>

class DnnMatteEstimator {
public:
    explicit DnnMatteEstimator(const std::string& onnxPath);

    // Retourne alpha en float32 dans [0..1], taille = bgr.size()
    cv::Mat computeAlpha(const cv::Mat& bgr);

    // Debug
    void setVerbose(bool v) { verbose_ = v; }

    // ✅ Ajoutés
    bool isValid() const { return valid_; }
    const std::string& lastError() const { return lastError_; }

private:
    cv::dnn::Net net_;
    bool verbose_ = false;

    bool valid_ = false;
    std::string lastError_;

    int inputW_ = 512;
    int inputH_ = 512;

    static cv::Mat squeezeToHW(const cv::Mat& out);
};
