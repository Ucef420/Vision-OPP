#pragma once
#include <memory>
#include <string>
#include <vector>

#include <opencv2/core.hpp>

#include "source/IFrameSource.hpp"
#include "output/IOutputSink.hpp"
#include "filter/IFilter.hpp"
#include "filter/FilterFactory.hpp"
#include "filter/IHasUI.hpp"

class VisionApp {
public:
    VisionApp(std::unique_ptr<IFrameSource> source,
              std::unique_ptr<IOutputSink> sink);

    void run();

private:
    // Source / Sink
    std::unique_ptr<IFrameSource> source_;
    std::unique_ptr<IOutputSink> sink_;

    // IDs des filtres actifs (gauche / droite)
    FilterId leftId_  = FilterId::Negative;
    FilterId rightId_ = FilterId::PseudoColor;

    // Chaînes de filtres (plus tard tu peux en mettre 2, 3, 4…)
    std::vector<std::unique_ptr<IFilter>> leftFilters_;
    std::vector<std::unique_ptr<IFilter>> rightFilters_;

    // Split drag
    int splitX_ = 320;
    bool dragging_ = false;

    // UI info
    std::string leftName_ = "Left";
    std::string rightName_ = "Right";
    double fps_ = 0.0;


    // Mouse + HUD
    static void onMouse(int event, int x, int y, int flags, void* userdata);
    void drawHUD(cv::Mat& out);

    // Reconstruit les chaines de filtres quand tu changes au clavier
    void rebuildChains();

    // Applique une chaine (séquentiel)
    void applyChain(std::vector<std::unique_ptr<IFilter>>& chain, cv::Mat& frame);

    // UI générique : si un filtre a une UI -> setup + sync
    void applyUIForChain(std::vector<std::unique_ptr<IFilter>>& chain,const std::string& ctrlWindow, const std::string& prefix);

    // Gestion clavier AZERTY (1..7 / a..u)
    void handleKey(int key);
        // Window name (important pour trackbars)

    const std::string win_  = "Vision OOP";
    const std::string ctrlL_ = "Controls L";
    const std::string ctrlR_ = "Controls R";

};
