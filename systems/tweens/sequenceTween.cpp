#include "tween.h"

void SequenceTween::add(std::unique_ptr<TweenBase> tween) {
    tweens_.emplace();
    tweens_.back().push_back(std::move(tween));
}

void SequenceTween::join(std::unique_ptr<TweenBase> tween) {
    if (tweens_.empty())
        tweens_.emplace();

    tweens_.back().push_back(std::move(tween));
}

void SequenceTween::start() {
    onStart->invoke();
    for (const auto& tween : tweens_.front()) {
        tween->start();
    }

    done_ = false;
}

bool SequenceTween::update(const float deltaTime) {
    // natural completion
    if (tweens_.empty() && !naturallyCompleted_) {
        naturallyCompleted_ = true;
        onComplete->invoke();
    }

    // means we either (were force completed, or were force cancelled)
    if (done_)
        return true;


    // if we finished naturally and AUTO_KILL is on we will kill ourself else if we finished naturally and if AUTO_KILL
    // is off then we wont kill ourself until manually completed
    if (naturallyCompleted_)
        return AUTO_KILL;

    for (auto it = tweens_.front().begin(); it != tweens_.front().end();) {
        if ((*it)->update(deltaTime))
            it = tweens_.front().erase(it);
        else
            ++it;
    }

    // no more tweens in step
    if (tweens_.front().empty()) {
        tweens_.pop();
    }

    return false;
}

void SequenceTween::forceComplete() {
    onComplete->invoke();
    // force complete all the tweens inside
    while (!tweens_.empty()) {
        for (const auto& tween : tweens_.front()) {
            tween->forceComplete();
            tweens_.pop();
        }
    }

    done_ = true;
}

void SequenceTween::forceCancel() {
    onCancel->invoke();

    // force cancel all the tweens inside
    while (!tweens_.empty()) {
        for (const auto& tween : tweens_.front()) {
            tween->forceCancel();
            tweens_.pop();
        }
    }

    done_ = true;
}
