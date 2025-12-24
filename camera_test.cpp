#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    cv::VideoCapture cap(0, cv::CAP_DSHOW);
    if (!cap.isOpened()) {
        std::cerr << "Cannot open camera\n";
        return 1;
    }

    cv::Mat frame;
    while (true) {
        if (!cap.read(frame) || frame.empty()) {
            std::cerr << "Read failed\n";
            break;
        }

        cv::imshow("cam_test", frame);

        int k = cv::waitKey(1);          // <= 1ms : temps réel
        if (k == 27) break;              // ESC pour quitter
    }
    return 0;
}
