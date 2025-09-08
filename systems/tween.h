#pragma once
#include <functional>
#include "math/curve.h"

// transform.tweenLocalPosition(...) returns a tween that can be started() and that will add it to the update loop and let it run
// if used in sequence.append(transform.tweenLocalPosition(...)) it will instead add it to the sequence
// when the sequence is ran, it will start the tween on its corresponding object, then it will wait until that tween is done and then run the next.
// if there are multiple tweens in the same index, it will run all of them then wait for all of them to finish


class TweenBase {
public:
    virtual ~TweenBase() = default;

    virtual bool update(double deltaTime) {
        return true;
    };
    virtual void kill() {};
};

template<typename T>
class Tween final : public TweenBase {
private:
    std::function<void(T)> targetFunc = nullptr;
    T* targetPtr = nullptr;
    T startValue;
    T endValue;
    float duration;
    float elapsed = 0;

    Curve curve;

public:
    Tween(std::function<void(T)> targetFunc, const T& start, const T& end, float duration, Curve curve) {
        this->targetFunc = targetFunc;
        this->startValue = start;
        this->endValue = end;
        this->duration = duration;
        this->curve = curve;

        functional = true;
    }
    Tween(T* targetPtr, const T& start, const T& end, float duration, Curve curve) {
        this->targetPtr = targetPtr;
        this->startValue = start;
        this->endValue = end;
        this->duration = duration;
        this->curve = curve;

        functional = false;
    }

    void start() {

    }

    /**
     * @note Dont call manually, will be automatically called
     * @param deltaTime How much time has elapsed from the last frame
     * @return Is the tween already done (finished)
     */
    bool update(const double deltaTime) override {
        if (finished)
            return true;

        elapsed += deltaTime;

        if (functional)
            targetFunc(lerp(startValue, endValue, curve.evaluate(elapsed / duration)));
        else
            (*targetPtr) = lerp(startValue, endValue, curve.evaluate(elapsed / duration));

        return elapsed >= duration;
    }

    void kill() override {
        finished = true;
    }

    bool getFinished() const {
        return finished;
    }

private:
    bool finished = false;
    bool functional = false;

    static T lerp(const T& start, const T& end, float t) {
        return start + (end - start) * t;
    }
};
