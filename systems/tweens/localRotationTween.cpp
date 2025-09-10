#include "tween.h"

LocalRotationTween::LocalRotationTween(float* target, const float start, const float end, const double duration, const Curve& curve) {
    target_ = target;
    start_ = start;
    end_ = end;
    duration_ = duration;
    curve_ = curve;
}

void LocalRotationTween::start() {
    *target_ = start_;
    onStart.invoke();
}

void LocalRotationTween::update(const double deltaTime) {
    elapsed_ += deltaTime;

    const double t = curve_.evaluate(elapsed_ / duration_);
    *target_ = start_ + (end_ - start_) * t;

    if (elapsed_ > duration_) {
        complete();
    }
}

void LocalRotationTween::complete() {
    *target_ = end_;
    completed_ = true;
    onComplete.invoke();
}

void LocalRotationTween::cancel() {
    canceled_ = true;
    onCancel.invoke();
}

bool LocalRotationTween::shouldDelete() {
    return canceled_ || completed_;
}
