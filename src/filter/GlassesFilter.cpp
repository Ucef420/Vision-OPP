#include "GlassesFilter.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <stdexcept>

static void overlayRGBA(cv::Mat& bgr, const cv::Mat& rgba, cv::Rect roi) {
    if (roi.width <= 0 || roi.height <= 0) return;

    // Clamp ROI inside bgr
    roi &= cv::Rect(0, 0, bgr.cols, bgr.rows);
    if (roi.width <= 0 || roi.height <= 0) return;

    cv::Mat resized;
    cv::resize(rgba, resized, roi.size(), 0, 0, cv::INTER_AREA);

    CV_Assert(resized.channels() == 4);

    std::vector<cv::Mat> ch;
    cv::split(resized, ch); // B,G,R,A
    cv::Mat a = ch[3];
    cv::threshold(a, a, 20, 255, cv::THRESH_BINARY); // 20..50 à tester

    cv::Mat alpha;
    ch[3].convertTo(alpha, CV_32F, 1.0 / 255.0); // 0..1

    cv::Mat fg;
    cv::merge(std::vector<cv::Mat>{ch[0], ch[1], ch[2]}, fg);

    cv::Mat dst = bgr(roi);

    cv::Mat fgF, dstF;
    fg.convertTo(fgF, CV_32F);
    dst.convertTo(dstF, CV_32F);

    // expand alpha to 3 channels
    cv::Mat alpha3;
    cv::merge(std::vector<cv::Mat>{alpha, alpha, alpha}, alpha3);

    cv::Mat outF = fgF.mul(alpha3) + dstF.mul(1.0f - alpha3);
    outF.convertTo(dst, CV_8U);
}
GlassesFilter::GlassesFilter(const std::string& faceCascadePath,
                             const std::string& eyeCascadePath,
                             const std::string& glassesPngPath) {
    if (!faceCascade_.load(faceCascadePath)) throw std::runtime_error("Face cascade load failed");
    if (!eyeCascade_.load(eyeCascadePath))   throw std::runtime_error("Eye cascade load failed");

    glassesRGBA_ = cv::imread(glassesPngPath, cv::IMREAD_UNCHANGED); // keep alpha
    if (glassesRGBA_.empty() || glassesRGBA_.channels() != 4) {
        throw std::runtime_error("glasses.png must be RGBA (transparent PNG)");
    }
}

void GlassesFilter::apply(cv::Mat& frame) {
    cv::Mat gray;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
    cv::equalizeHist(gray, gray);

    std::vector<cv::Rect> faces;
    faceCascade_.detectMultiScale(gray, faces, 1.1, 3, 0, cv::Size(80,80));
    if (faces.empty()) return;

    // prends le plus grand visage
    cv::Rect face = *std::max_element(faces.begin(), faces.end(),
        [](const cv::Rect& a, const cv::Rect& b){ return a.area() < b.area(); });

    cv::Mat faceROI = gray(face);
    std::vector<cv::Rect> eyes;
    eyeCascade_.detectMultiScale(faceROI, eyes, 1.1, 3, 0, cv::Size(20,20));
    if (eyes.size() < 2) return;

    // prends 2 yeux (les plus "hauts" + grands)
    std::sort(eyes.begin(), eyes.end(), [](const cv::Rect& a, const cv::Rect& b){
        if (a.y != b.y) return a.y < b.y;
        return a.area() > b.area();
    });
    cv::Rect e1 = eyes[0], e2 = eyes[1];
    if (e2.x < e1.x) std::swap(e1, e2);

    // coords globales
    cv::Point c1(face.x + e1.x + e1.width/2, face.y + e1.y + e1.height/2);
    cv::Point c2(face.x + e2.x + e2.width/2, face.y + e2.y + e2.height/2);

    int eyeDist = std::max(1, c2.x - c1.x);
    if (eyeDist < 10) return;           // too small / unstable -> skip this frame
    // ROI lunettes : largeur ~ 2.2 * distance yeux, hauteur ~ 0.9 * distance
   int w = int(2.2 * eyeDist);
int h = int(0.9 * eyeDist);

w = std::max(w, 1);
h = std::max(h, 1);

int x = (c1.x + c2.x)/2 - w/2;
int y = (c1.y + c2.y)/2 - h/2;

// clamp ROI inside image
x = std::max(0, x);
y = std::max(0, y);
if (x + w > frame.cols) w = frame.cols - x;
if (y + h > frame.rows) h = frame.rows - y;

// if ROI became invalid -> skip
if (w <= 0 || h <= 0) return;

overlayRGBA(frame, glassesRGBA_, cv::Rect(x, y, w, h));
}
