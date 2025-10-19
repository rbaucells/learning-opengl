#include "tween.h"

CallbackTween::CallbackTween(const std::function<void()>& func) {
    func_ = func;
}

void CallbackTween::start() {
    onStart->invoke();
}

bool CallbackTween::update(float deltaTime) {
    func_();
    return true;
}

void CallbackTween::forceComplete() {
    onComplete->invoke();
}

void CallbackTween::forceCancel() {
    onCancel->invoke();
}

bool CallbackTween::running() const {
    return false;
}