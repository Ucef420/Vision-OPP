cmake_minimum_required(VERSION 3.16)
project(vision_oop LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Link statically
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

find_package(OpenCV REQUIRED)

add_executable(vision_oop
    src/main.cpp
    src/app/VisionApp.cpp
    src/source/CameraSource.cpp
    src/pipeline/Pipeline.cpp
    src/filter/DummyFilter.cpp
    src/output/WindowSink.cpp
    src/filter/EdgeFilter.cpp
    src/filter/PixelFilter.cpp
    src/filter/PseudoColorFilter.cpp
    src/filter/NegativeFilter.cpp
    src/filter/CartoonFilter.cpp
)
add_executable(camera_test camera_test.cpp)
target_link_libraries(camera_test PRIVATE ${OpenCV_LIBS})

target_include_directories(vision_oop PRIVATE src)
target_link_libraries(vision_oop PRIVATE ${OpenCV_LIBS})