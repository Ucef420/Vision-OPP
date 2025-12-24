#include "app/VisionApp.hpp"
#include "source/CameraSource.hpp"
#include "pipeline/Pipeline.hpp"
#include "filter/DummyFilter.hpp"
#include "output/WindowSink.hpp"

int main() {
    auto source = std::make_unique<CameraSource>(0);
    auto sink = std::make_unique<WindowSink>("Vision OOP");

    Pipeline pipeline;
    pipeline.addFilter(std::make_unique<DummyFilter>());

    VisionApp app(std::move(source), pipeline, std::move(sink));
    app.run();

    return 0;
}
