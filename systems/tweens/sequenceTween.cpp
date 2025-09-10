#include "tween.h"
void SequenceTween::add(std::unique_ptr<TweenBase> tween) {
    tweens_.push(std::move(tween));
}

void SequenceTween::start() {
    running_ = true;
    completed_ = false;
    if (!tweens_.empty()) {
        currentTween_ = tweens_.front().get();
        currentTween_->start();
    }
}

void SequenceTween::update(const double deltaTime) {
    if (!currentTween_)
        return;

    currentTween_->update(deltaTime);

    if (currentTween_->shouldDelete()) {
        // aka pop_front
        tweens_.pop();
        currentTween_ = nullptr;

        // start the next tween if it isnt empty
        if (!tweens_.empty()) {
            currentTween_ = tweens_.front().get();
            currentTween_->start();
        }
        else {
            // else we are done
            completed_ = true;
            running_ = false;
        }
    }
}

void SequenceTween::complete() {
    // force complete all the tweens inside
    while (!tweens_.empty()) {
        tweens_.front()->complete();
        tweens_.pop();
    }

    completed_ = true;
    running_ = false;
}

void SequenceTween::cancel() {
    // cancel all the tweens inside
    while (!tweens_.empty()) {
        tweens_.front()->cancel();
        tweens_.pop();
    }

    canceled_ = true;
    running_ = false;
}

bool SequenceTween::shouldDelete() {
    return canceled_ || completed_ || !running_;
}
