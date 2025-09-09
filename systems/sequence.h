#pragma once
#include <map>
#include "tween.h"


class Sequence {
    std::map<int, std::vector<std::unique_ptr<TweenBase>>> tweens;
    int curIndex = 0;
    int runningTweens = 0;

    void handleNextSequenceLayer(int i);
    void countFinishedTweens(int i);

    bool isDone = false;

public:
    void start();
    void add(std::unique_ptr<TweenBase> tween);
    void join(std::unique_ptr<TweenBase> tween);
    void stop();

    bool getDone() const {
        return isDone;
    }
};
