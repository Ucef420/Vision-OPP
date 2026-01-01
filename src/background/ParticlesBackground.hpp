#pragma once
#include "IBackgroundProvider.hpp"
#include <vector>

class ParticlesBackground : public IBackgroundProvider {
public:
    explicit ParticlesBackground(int count = 250);

    cv::Mat next(int w, int h, double t) override;

    void setCount(int n);
    void setSpeed(float s);
    void setRadius(int r);

private:
    struct P { float x,y,vx,vy; };
    std::vector<P> ps_;

    int count_ = 250;
    float speed_ = 120.0f; // pixels/sec
    int radius_ = 3;

    void resetParticle(P& p, int w, int h);
};
