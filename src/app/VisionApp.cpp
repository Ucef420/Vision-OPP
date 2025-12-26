#include "app/VisionApp.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <chrono>
#include <algorithm>

// =====================
// Constructor
// =====================
VisionApp::VisionApp(std::unique_ptr<IFrameSource> source,
                     std::unique_ptr<IOutputSink> sink)
    : source_(std::move(source)),
      sink_(std::move(sink)) {}

// =====================
// Mouse callback
// =====================
void VisionApp::onMouse(int event, int x, int y, int flags, void* userdata) {
    auto* app = reinterpret_cast<VisionApp*>(userdata);
    if (!app) return;

    if (event == cv::EVENT_LBUTTONDOWN) app->dragging_ = true;
    if (event == cv::EVENT_LBUTTONUP)   app->dragging_ = false;
    if (event == cv::EVENT_MOUSEMOVE && app->dragging_) app->splitX_ = x;
}

// =====================
// HUD
// =====================
void VisionApp::drawHUD(cv::Mat& out) {
    const int font = cv::FONT_HERSHEY_SIMPLEX;

    cv::putText(out, "L: " + leftName_,  cv::Point(10, 25), font, 0.7, cv::Scalar(255,255,255), 2);
    cv::putText(out, "R: " + rightName_, cv::Point(10, 55), font, 0.7, cv::Scalar(255,255,255), 2);

    char buf[64];
    std::snprintf(buf, sizeof(buf), "FPS: %.1f", fps_);
    cv::putText(out, buf, cv::Point(10, 85), font, 0.7, cv::Scalar(255,255,255), 2);

    cv::putText(out,
                "Gauche: 1..7 | Droite: a z e r t y u | Drag split | ESC quit",
                cv::Point(10, out.rows - 15),
                font, 0.55, cv::Scalar(255,255,255), 2);
}

// =====================
// Rebuild chains when filter changes
// =====================
void VisionApp::rebuildChains() {
    leftFilters_.clear();
    rightFilters_.clear();

    // 1 filtre pour l’instant (plus tard tu peux push_back plusieurs)
    if (auto f = makeFilter(leftId_)) {
        leftFilters_.push_back(std::move(f));
    }
    if (auto f = makeFilter(rightId_)) {
        rightFilters_.push_back(std::move(f));
    }

    leftName_  = filterName(leftId_);
    rightName_ = filterName(rightId_);
}

// =====================
// Apply one chain sequentially
// =====================
void VisionApp::applyChain(std::vector<std::unique_ptr<IFilter>>& chain, cv::Mat& frame) {
    for (auto& f : chain) {
        if (f) f->apply(frame);
    }
}

// =====================
// UI auto for chain (IHasUI)
// =====================
void VisionApp::applyUIForChain(std::vector<std::unique_ptr<IFilter>>& chain,
                                const std::string& ctrlWindow,
                                const std::string& prefix) {
    for (auto& f : chain) {
        if (!f) continue;
        if (auto ui = dynamic_cast<IHasUI*>(f.get())) {
            ui->setupUI(ctrlWindow, prefix);
            ui->syncFromUI();
        }
    }
}

// =====================
// Keyboard handler (AZERTY)
// =====================
void VisionApp::handleKey(int key) {
    bool changedLeft  = false;
    bool changedRight = false;

    // ===== LEFT : 1..7 =====
    if (key == '1') { leftId_ = FilterId::Mirror;      changedLeft = true; }
    if (key == '2') { leftId_ = FilterId::Negative;    changedLeft = true; }
    if (key == '3') { leftId_ = FilterId::Pixel;       changedLeft = true; }
    if (key == '4') { leftId_ = FilterId::PseudoColor; changedLeft = true; }
    if (key == '5') { leftId_ = FilterId::Cartoon;     changedLeft = true; }
    if (key == '6') { leftId_ = FilterId::Edge;        changedLeft = true; }
    if (key == '7') { leftId_ = FilterId::Glasses;     changedLeft = true; }

    // ===== RIGHT : a z e r t y u =====
    if (key == 'a') { rightId_ = FilterId::Mirror;      changedRight = true; }
    if (key == 'z') { rightId_ = FilterId::Negative;    changedRight = true; }
    if (key == 'e') { rightId_ = FilterId::Pixel;       changedRight = true; }
    if (key == 'r') { rightId_ = FilterId::PseudoColor; changedRight = true; }
    if (key == 't') { rightId_ = FilterId::Cartoon;     changedRight = true; }
    if (key == 'y') { rightId_ = FilterId::Edge;        changedRight = true; }
    if (key == 'u') { rightId_ = FilterId::Glasses;     changedRight = true; }

    // ===== RESET SPLIT =====
    if (key == '0') splitX_ = 320;

    // ===== RESET UI WINDOWS SI FILTRE CHANGE =====
    if (changedLeft) {
        cv::destroyWindow(ctrlL_);
        cv::namedWindow(ctrlL_);
    }

    if (changedRight) {
        cv::destroyWindow(ctrlR_);
        cv::namedWindow(ctrlR_);
    }

    // ===== REBUILD CHAINS =====
    if (changedLeft || changedRight) {
        rebuildChains();
    }
}
// =====================
// Run loop
// =====================
void VisionApp::run() {
    std::cerr << "[VisionApp] start\n";

    cv::namedWindow(win_);
    cv::namedWindow(ctrlL_);
    cv::namedWindow(ctrlR_);

    cv::setMouseCallback(win_, VisionApp::onMouse, this);

    rebuildChains();

    auto lastT = std::chrono::high_resolution_clock::now();
    int frames = 0;

    while (true) {
        cv::Mat frame;
        if (!source_->getFrame(frame) || frame.empty()) break;

        // UI auto (si filtre configurable)
        applyUIForChain(leftFilters_,  ctrlL_, "L ");
        applyUIForChain(rightFilters_, ctrlR_, "R ");

        // clones
        cv::Mat left  = frame.clone();
        cv::Mat right = frame.clone();

        // apply chains
        applyChain(leftFilters_, left);
        applyChain(rightFilters_, right);

        // compose split
        int splitX = std::clamp(splitX_, 0, frame.cols);
        cv::Mat out = right;
        if (splitX > 0) {
            left(cv::Rect(0, 0, splitX, frame.rows))
                .copyTo(out(cv::Rect(0, 0, splitX, frame.rows)));
        }

        // border line
        cv::line(out, cv::Point(splitX, 0), cv::Point(splitX, out.rows - 1),
                 cv::Scalar(255,255,255), 2);

        // FPS
        frames++;
        auto now = std::chrono::high_resolution_clock::now();
        double dt = std::chrono::duration<double>(now - lastT).count();
        if (dt >= 0.5) {
            fps_ = frames / dt;
            frames = 0;
            lastT = now;
        }

        drawHUD(out);

        // Show (via sink)
        sink_->handleFrame(out);
        // Alternative si tu veux bypass sink:
        // cv::imshow(win_, out);

        int key = cv::waitKey(1);
        if (key == 27) break; // ESC
        handleKey(key);
        
    }

    cv::destroyAllWindows();
    std::cerr << "[VisionApp] end\n";
}
