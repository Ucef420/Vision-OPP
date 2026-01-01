#include "DnnMatteEstimator.hpp"
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <algorithm>

DnnMatteEstimator::DnnMatteEstimator(const std::string& onnxPath) {
    try {
        std::cerr << "[DnnMatteEstimator] Loading ONNX: " << onnxPath << "\n";
        net_ = cv::dnn::readNetFromONNX(onnxPath);

        net_.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
        net_.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);

        valid_ = true;
        lastError_.clear();
    } catch (const cv::Exception& e) {
        valid_ = false;
        lastError_ = e.what();
        std::cerr << "[DnnMatteEstimator] ERROR loading ONNX: " << lastError_ << "\n";
    } catch (const std::exception& e) {
        valid_ = false;
        lastError_ = e.what();
        std::cerr << "[DnnMatteEstimator] ERROR loading ONNX: " << lastError_ << "\n";
    }
}

cv::Mat DnnMatteEstimator::squeezeToHW(const cv::Mat& out) {
    if (out.dims == 2) {
        return out;
    }
    if (out.dims == 4) {
        const int N = out.size[0];
        const int C = out.size[1];
        const int H = out.size[2];
        const int W = out.size[3];
        if (N != 1 || C != 1) {
            throw std::runtime_error("MODNet output shape unexpected (expected 1x1xHxW).");
        }
        cv::Mat hw(H, W, CV_32F, (void*)out.ptr<float>());
        return hw;
    }
    throw std::runtime_error("MODNet output dims unsupported.");
}
static cv::Mat padToSquare(const cv::Mat& src, int& top, int& left, int& size) {
    CV_Assert(!src.empty());
    int h = src.rows, w = src.cols;
    size = std::max(h, w);

    top  = (size - h) / 2;
    int bottom = size - h - top;

    left = (size - w) / 2;
    int right  = size - w - left;

    cv::Mat dst;
    cv::copyMakeBorder(src, dst, top, bottom, left, right, cv::BORDER_CONSTANT, cv::Scalar(0,0,0));
    return dst;
}

static cv::Mat cropFromSquare(const cv::Mat& square, int top, int left, int h, int w) {
    cv::Rect roi(left, top, w, h);
    roi &= cv::Rect(0, 0, square.cols, square.rows);
    return square(roi).clone();
}

cv::Mat DnnMatteEstimator::computeAlpha(const cv::Mat& bgr) {
    if (!valid_ || bgr.empty()) return {};

    try {
        const int S = 512; // MODNet: fixe, évite les mismatch (Mul/Concat)

        // --- 1) Letterbox vers SxS (garde ratio + padding)
        int w = bgr.cols, h = bgr.rows;
        float scale = std::min(S / (float)w, S / (float)h);
        int nw = std::max(1, (int)std::round(w * scale));
        int nh = std::max(1, (int)std::round(h * scale));

        cv::Mat resized;
        cv::resize(bgr, resized, cv::Size(nw, nh), 0, 0, cv::INTER_LINEAR);

        cv::Mat canvas(S, S, bgr.type(), cv::Scalar(0, 0, 0));
        int x0 = (S - nw) / 2;
        int y0 = (S - nh) / 2;
        resized.copyTo(canvas(cv::Rect(x0, y0, nw, nh)));

        // --- 2) Blob (MODNet souvent attend [-1,1])
        // (x - 127.5) / 127.5  => scalefactor=1/127.5, mean=127.5
        cv::Mat blob = cv::dnn::blobFromImage(
            canvas,
            1.0 / 127.5,
            cv::Size(S, S),
            cv::Scalar(127.5, 127.5, 127.5),
            true,   // swapRB BGR->RGB
            false
        );

        net_.setInput(blob);

        // --- 3) Forward
        cv::Mat out = net_.forward();         // shape typique: 1x1xSxS
        cv::Mat alphaS = squeezeToHW(out);    // -> SxS float

        // clamp
        cv::max(alphaS, 0.0, alphaS);
        cv::min(alphaS, 1.0, alphaS);

        // --- 4) Dé-letterbox: crop la zone utile puis resize à l'image originale
        cv::Mat alphaCrop = alphaS(cv::Rect(x0, y0, nw, nh)).clone();

        cv::Mat alpha;
        cv::resize(alphaCrop, alpha, cv::Size(w, h), 0, 0, cv::INTER_LINEAR);

        lastError_.clear();
        return alpha;

    } catch (const cv::Exception& e) {
        lastError_ = e.what();
        std::cerr << "[DnnMatteEstimator] forward ERROR: " << lastError_ << "\n";
        return {};
    } catch (const std::exception& e) {
        lastError_ = e.what();
        std::cerr << "[DnnMatteEstimator] forward ERROR: " << lastError_ << "\n";
        return {};
    }
}

