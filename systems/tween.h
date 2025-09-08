#pragma once
#include <functional>

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

    Curve curve;

    Tween(T *targetPtr, const T &start, const T &end, float duration, Curve curve) {
        this->targetPtr = targetPtr;
        this->start = start;
        this->end = end;
        this->duration = duration;
        this->curve = curve;

        functional = false;
    }

    Tween(std::function<void(T)> targetFunc, const T &start, const T &end, float duration, Curve curve) {
        this->targetFunc = targetFunc;
        this->start = start;
        this->end = end;
        this->duration = duration;
        this->curve = curve;

        functional = true;
    }

    bool update(const float deltaTime) override {
        if (finished)
            return true;

        elapsed += deltaTime;

        if (functional)
            targetFunc(lerp(start, end, curve.evaluate(elapsed / duration)));
        else
            (*targetPtr) = lerp(start, end, curve.evaluate(elapsed / duration));

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
