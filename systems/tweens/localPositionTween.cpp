#include "tween.h"

LocalPositionTween::LocalPositionTween(Vector2* target, const Vector2& start, const Vector2& end, double duration, Curve curve) {
    target_ = target;
    start_ = start;
    end_ = end;
    duration_ = duration;
    curve_ = curve;
}

void LocalPositionTween::start() {
    *target_ = start_;
    onStart.invoke();
}

void LocalPositionTween::update(const double deltaTime) {
    elapsed_ += deltaTime;

    const double t = curve_.evaluate(elapsed_ / duration_);
    *target_ = start_ + (end_ - start_) * t;

    if (elapsed_ > duration_) {
        complete();
    }
}

void LocalPositionTween::complete() {
    *target_ = end_;
    completed_ = true;
    onComplete.invoke();
}

void LocalPositionTween::cancel() {
    canceled_ = true;
    onCancel.invoke();
}

bool LocalPositionTween::shouldDelete() {
    return canceled_ || completed_;
}
