#pragma once
#include "IFilter.hpp"
#include "IHasUI.hpp"
#include <string>

class EdgeFilter : public IFilter, public IHasUI {
public:
    void apply(cv::Mat& frame) override;
    void setupUI(const std::string& windowName,
                 const std::string& prefix) override;
    void syncFromUI() override;

private:
    std::string uiWindowCreatedFor_;

    int t1_ = 80;
    int t2_ = 160;
    int blurK_ = 5;
    int sigma10_ = 15;
};
