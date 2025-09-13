#include "tween.h"

CallbackTween::CallbackTween(const std::function<void()>& func) {
    func_ = func;
}

void CallbackTween::update(float deltaTime) {
    func_();
}

void CallbackTween::start() {
    running_ = true;
    completed_ = false;
    onStart.invoke();
}

void CallbackTween::cancel() {
    canceled_ = true;
    onCancel.invoke();
}

void CallbackTween::complete() {
    completed_ = true;
    onComplete.invoke();
}