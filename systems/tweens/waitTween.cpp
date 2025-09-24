#include "tween.h"

WaitTween::WaitTween(const float duration) {
    this->duration_ = duration;
}

void WaitTween::start() {
    onStart.invoke();

    done_ = false;
    elapsed_ = 0;
}

bool WaitTween::update(const float deltaTime) {
    // natural completion
    if (elapsed_ >= duration_ && !naturallyCompleted_) {
        naturallyCompleted_ = true;
        onComplete.invoke();
    }

    // means we either (were force completed, or were force cancelled)
    if (done_)
        return true;


    // if we finished naturally and AUTO_KILL is on we will kill ourself else if we finished naturally and if AUTO_KILL
    // is off then we wont kill ourself until manually completed
    if (naturallyCompleted_)
        return AUTO_KILL;

    elapsed_ += deltaTime;

    return false;
}

void WaitTween::forceComplete() {
    onComplete.invoke();
    done_ = true;
}

void WaitTween::forceCancel() {
    onCancel.invoke();
    done_ = true;
}