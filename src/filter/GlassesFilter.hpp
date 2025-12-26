#pragma once
#include "IFilter.hpp"
#include <opencv2/objdetect.hpp>

class GlassesFilter : public IFilter {
public:
    GlassesFilter(const std::string& faceCascadePath,
                  const std::string& eyeCascadePath,
                  const std::string& glassesPngPath);

    void apply(cv::Mat& frame) override;

private:
    cv::CascadeClassifier faceCascade_;
    cv::CascadeClassifier eyeCascade_;
    cv::Mat glassesRGBA_; // PNG avec alpha
};
