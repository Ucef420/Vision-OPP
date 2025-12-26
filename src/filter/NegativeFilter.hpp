#pragma once
#include "IFilter.hpp"
#include "IHasUI.hpp"
#include <string>

class NegativeFilter : public IFilter, public IHasUI {
public:
    void apply(cv::Mat& frame) override;
    void setupUI(const std::string& windowName,
                 const std::string& prefix) override;
    void syncFromUI() override;

private:
    std::string uiWindowCreatedFor_;

    int enabled_  = 1;
    int strength_ = 100;
};
