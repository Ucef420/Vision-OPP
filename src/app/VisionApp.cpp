#include "VisionApp.hpp"
#include <opencv2/opencv.hpp>

VisionApp::VisionApp(std::unique_ptr<IFrameSource> source,
                     Pipeline pipeline,
                     std::unique_ptr<IOutputSink> sink)
    : source_(std::move(source)),
      pipeline_(std::move(pipeline)),
      sink_(std::move(sink)) {}

void VisionApp::run() {
    cv::Mat frame;

    while (true) {
        if (!source_->getFrame(frame) || frame.empty())
            break;

        pipeline_.process(frame);
        sink_->handleFrame(frame);

        if (cv::waitKey(1) == 27) // ESC
            break;
    }
}
