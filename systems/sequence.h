#pragma once
#include <map>
#include "tween.h"


class Sequence {
    std::map<int, std::vector<std::unique_ptr<TweenBase>>> actions;
    int curIndex = 0;

public:
    void start();
    void add(const TweenBase& tween);
    void join(const TweenBase& tween);
    void stop();
};
