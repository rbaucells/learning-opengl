#include "sequence.h"

void Sequence::handleNextSequenceLayer(const int i) {
    if (i > curIndex) {
        isDone = true;
        return;
    }
    for (const auto& tween : tweens[i]) {
        tween->start();
        runningTweens++;
        tween->onFinished.subscribe([this, i](){countFinishedTweens(i + 1);});
    }
}

void Sequence::countFinishedTweens(const int i) {
    runningTweens--;

    if (runningTweens == 0)
        handleNextSequenceLayer(i);
}


void Sequence::start() {
    handleNextSequenceLayer(0);
}
void Sequence::add(std::unique_ptr<TweenBase> tween) {
    curIndex++;
    tweens[curIndex].push_back(std::move(tween));
}
void Sequence::join(std::unique_ptr<TweenBase> tween) {
    tweens[curIndex].push_back(std::move(tween));
}
void Sequence::stop() {
    for (int i = 0; i < curIndex; i++) {
        for (const auto& tween : tweens[i]) {
            tween->kill();
        }
    }
}
