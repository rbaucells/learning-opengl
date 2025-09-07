#pragma once
#include <functional>

enum Easer : int {
    linear,
    sine_in, sine_out, sine_in_out,
    cubic_in, cubic_out, cubic_in_out,
    expo_in, expo_out, expo_in_out,
};

class TweenBase {
public:
    virtual ~TweenBase() = default;
    virtual bool update(float deltaTime) {return true;};
    virtual void kill() {};
};

template<typename T>
class Tween : public TweenBase {
public:
    std::function<void(T)> targetFunc = nullptr;
    T *targetPtr = nullptr;
    T start;
    T end;
    float duration;
    float elapsed = 0;

    Easer easer;

    Tween(T *targetPtr, const T &start, const T &end, float duration, Easer easer) {
        this->targetPtr = targetPtr;
        this->start = start;
        this->end = end;
        this->duration = duration;
        this->easer = easer;

        functional = false;
    }

    Tween(std::function<void(T)> targetFunc, const T &start, const T &end, float duration, Easer easer) {
        this->targetFunc = targetFunc;
        this->start = start;
        this->end = end;
        this->duration = duration;
        this->easer = easer;

        functional = true;
    }

    bool update(const float deltaTime) override {
        if (finished)
            return true;

        elapsed += deltaTime;

        if (functional)
            targetFunc(lerp(start, end, elapsed / duration));
        else
            (*targetPtr) = lerp(start, end, elapsed / duration);

        return elapsed >= duration;
    }

    void kill() override {
        finished = true;
    }

private:
    bool finished = false;
    bool functional = false;

    static T lerp(const T &start, const T &end, float t) {
        return start + (end - start) * t;
    }
};
