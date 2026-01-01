#pragma once
#include "IFilter.hpp"
#include "IHasUI.hpp"

#include <string>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>

class PortraitBlurFilter : public IFilter, public IHasUI {
public:
    explicit PortraitBlurFilter(const std::string& faceCascadePath);
    void apply(cv::Mat& frame) override;

    void setupUI(const std::string& windowName,
                 const std::string& prefix) override;
    void syncFromUI() override;

private:
    std::string faceCascadePath_;
    cv::CascadeClassifier faceCascade_;

    // UI cache
    std::string uiWindowCreatedFor_;

    // Paramètres (trackbars)
    int enabled_      = 1;    // 0/1
    int blurK_        = 21;   // impair
    int feather_      = 25;   // douceur bords
    int expandX_      = 35;   // % d’expansion horizontale depuis le visage
    int expandTop_    = 20;   // % expansion vers le haut
    int expandBottom_ = 160;  // % expansion vers le bas (buste)
    int followFace_   = 1;    // 0/1

    // mémoire dernière zone
    cv::Rect lastROI_;
    bool hasLast_ = false;

    static int makeOdd(int v);
    static cv::Rect clampRect(const cv::Rect& r, int w, int h);
};
