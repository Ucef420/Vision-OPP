#include "VisionApp.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>
VisionApp::VisionApp(std::unique_ptr<IFrameSource> source,
                     Pipeline&& pipeline,
                     std::unique_ptr<IOutputSink> sink)
    : source_(std::move(source)),
      pipeline_(std::move(pipeline)),
      sink_(std::move(sink)) {}

void VisionApp::run() {
    cv::Mat frame;

    while (true) {
        bool ok = source_->getFrame(frame);

        if (!ok) {
    std::cerr << "getFrame=false\n"; break; 
        }
        if (frame.empty()) {
       std::cerr << "empty frame\n"; break;
        }

        pipeline_.process(frame);
        sink_->handleFrame(frame);

        if (cv::waitKey(1) == 27) break; // ESC
    }
}