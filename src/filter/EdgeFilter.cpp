#include "EdgeFilter.hpp"
#include <opencv2/imgproc.hpp>

void EdgeFilter::apply(cv::Mat& frame) {
    // frame arrive souvent en BGR (webcam)
    cv::Mat gray, edges;

    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(gray, gray, cv::Size(5,5), 1.5);
    cv::Canny(gray, edges, 80, 160);

    // Pour afficher dans une fenêtre couleur, on reconvertit en BGR
    cv::cvtColor(edges, frame, cv::COLOR_GRAY2BGR);
}