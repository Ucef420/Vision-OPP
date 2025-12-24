#pragma once
#include <memory>
#include "source/IFrameSource.hpp"
#include "pipeline/Pipeline.hpp"
#include "output/IOutputSink.hpp"

class VisionApp {
public:
    VisionApp(std::unique_ptr<IFrameSource> source,
              Pipeline&& pipeline,
              std::unique_ptr<IOutputSink> sink);

    void run();

private:
    std::unique_ptr<IFrameSource> source_;
    Pipeline pipeline_;
    std::unique_ptr<IOutputSink> sink_;
};
