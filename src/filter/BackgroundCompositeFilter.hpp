#pragma once
#include "IFilter.hpp"
#include "IHasUI.hpp"
#include "../background/IBackgroundProvider.hpp"
#include "../matting/DnnMatteEstimator.hpp"

#include <memory>
#include <string>
#include <opencv2/core.hpp>

class BackgroundCompositeFilter : public IFilter, public IHasUI {
public:
    BackgroundCompositeFilter(std::shared_ptr<IBackgroundProvider> bg,
                              const std::string& onnxPath);

    void apply(cv::Mat& frame) override;

    void setupUI(const std::string& windowName, const std::string& prefix) override;
    void syncFromUI() override;

private:
    std::shared_ptr<IBackgroundProvider> bg_;
    std::unique_ptr<DnnMatteEstimator> matte_;

    std::string uiWindowCreatedFor_;

    // UI
    int enabled_   = 1;   // 0/1
    int showAlpha_ = 1;   // 0/1
    int feather_   = 7;   // 0..40
    int cutoffX10_ = 2;   // 0..10  => 0.0..1.0
    int emaX10_    = 7;   // 0..10  => 0.0..1.0
    int downPct_   = 70;  // 30..100 (perf)

    // temps
    double t0_ = 0.0;

    // alpha smoothing
    cv::Mat alphaPrev_;

    static int makeOdd(int v);
    cv::Mat postProcessAlpha(const cv::Mat& alpha01);
    static double nowSec();
};
