#include "tween.h"

WaitTween::WaitTween(const double duration) {
    duration_ = duration;
}

void WaitTween::start() {
    running_ = true;
    completed_ = false;
    onStart.invoke();

    elapsed_ = 0;
}

void WaitTween::update(const double deltaTime) {
    elapsed_ += deltaTime;

    if (elapsed_ >= duration_)
        complete();
}

void WaitTween::complete() {
    completed_ = true;
    onComplete.invoke();
}

void WaitTween::cancel() {
    canceled_ = true;
    onCancel.invoke();
}

bool WaitTween::shouldDelete() {
    return elapsed_ >= duration_;
}

