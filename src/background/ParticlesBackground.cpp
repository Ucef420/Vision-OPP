#include "ParticlesBackground.hpp"
#include <opencv2/imgproc.hpp>
#include <random>
#include <algorithm>

static float frand(float a, float b) {
    static std::mt19937 rng{std::random_device{}()};
    std::uniform_real_distribution<float> d(a,b);
    return d(rng);
}

ParticlesBackground::ParticlesBackground(int count) : count_(count) {
    ps_.resize(count_);
}

void ParticlesBackground::setCount(int n) {
    count_ = std::max(10, n);
    ps_.assign(count_, P{});
}

void ParticlesBackground::setSpeed(float s) {
    speed_ = std::max(10.0f, s);
}

void ParticlesBackground::setRadius(int r) {
    radius_ = std::clamp(r, 1, 25);
}

void ParticlesBackground::resetParticle(P& p, int w, int h) {
    p.x = frand(0, (float)w);
    p.y = frand(0, (float)h);
    float ang = frand(0.0f, 6.2831853f);
    p.vx = std::cos(ang);
    p.vy = std::sin(ang);
}

cv::Mat ParticlesBackground::next(int w, int h, double t) {
    cv::Mat bg(h, w, CV_8UC3);

    // gradient “pro” en base (évite fond noir)
    for (int y=0; y<h; ++y) {
        float k = (float)y / std::max(1, h-1);
        cv::Vec3b c(
            (uchar)(30 + 40*k),
            (uchar)(35 + 35*k),
            (uchar)(40 + 30*k)
        );
        for (int x=0; x<w; ++x) bg.at<cv::Vec3b>(y,x) = c;
    }

    // init lazy
    if ((int)ps_.size() != count_) ps_.assign(count_, P{});
    static bool inited = false;
    if (!inited) {
        for (auto& p : ps_) resetParticle(p, w, h);
        inited = true;
    }

    // animation: on utilise t pour bouger (delta approx)
    static double lastT = t;
    float dt = (float)std::clamp(t - lastT, 0.0, 0.05);
    lastT = t;

    for (auto& p : ps_) {
        p.x += p.vx * speed_ * dt;
        p.y += p.vy * speed_ * dt;

        if (p.x < -50 || p.x > w+50 || p.y < -50 || p.y > h+50) {
            resetParticle(p, w, h);
        }

        cv::Point pt((int)p.x, (int)p.y);
        cv::circle(bg, pt, radius_, cv::Scalar(220,220,230), -1, cv::LINE_AA);
    }

    // petit blur léger pour un look “soft”
    cv::GaussianBlur(bg, bg, cv::Size(7,7), 0);
    return bg;
}
