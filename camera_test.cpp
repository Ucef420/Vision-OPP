#include <opencv2/opencv.hpp>
#include <iostream>

static bool try_open(int idx, int api, const char* apiName) {
    cv::VideoCapture cap(idx, api);
    std::cerr << "Index " << idx << " / " << apiName
              << " opened=" << cap.isOpened() << "\n";
    if (!cap.isOpened()) return false;

    cv::Mat frame;
    bool ok = cap.read(frame);
    std::cerr << "   read=" << ok << " empty=" << frame.empty()
              << " size=" << frame.cols << "x" << frame.rows << "\n";

    if (!ok || frame.empty()) return false;

    cv::imshow("camera_test", frame);
    cv::waitKey(1000); // 1 sec
    cap.release();
    cv::destroyAllWindows();
    return true;
}

int main() {
    std::cerr << "CAM TEST START\n";

    for (int idx = 0; idx < 5; ++idx) {
        if (try_open(idx, cv::CAP_MSMF,  "MSMF"))  return 0;
        if (try_open(idx, cv::CAP_DSHOW, "DSHOW")) return 0;
    }

    std::cerr << "NO CAMERA OPENED (MSMF/DSHOW)\n";
    return 1;
}
