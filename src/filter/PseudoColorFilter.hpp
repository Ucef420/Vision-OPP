#pragma once
#include "IFilter.hpp"
#include "IHasUI.hpp"
#include <string>

class PseudoColorFilter : public IFilter, public IHasUI {
public:
    void apply(cv::Mat& frame) override;
    void setupUI(const std::string& windowName,
                 const std::string& prefix) override;
    void syncFromUI() override;

private:
    std::string uiWindowCreatedFor_;
    int mapId_ = 2;
    int blend_ = 100;
};
