#pragma once
#include <functional>
#include "math/curve.h"
#include "component.h"
#include "event.h"

// transform.tweenLocalPosition(...) returns a tween that can be started() and that will add it to the update loop and let it run
// if used in sequence.append(transform.tweenLocalPosition(...)) it will instead add it to the sequence
// when the sequence is ran, it will start the tween on its corresponding object, then it will wait until that tween is done and then run the next.
// if there are multiple tweens in the same index, it will run all of them then wait for all of them to finish


class TweenBase {
public:
    virtual ~TweenBase() = default;

    virtual void start();

    virtual bool update(double deltaTime) {
        return true;
    };
    virtual void kill() {};

    virtual bool getFinished() const {
        return true;
    };

    // will run when tween is started
    Event<> onStart;
    // will run in the update loop after the value is changed
    Event<> onUpdate;
    // will run when tween finishes naturally
    Event<> onFinished;
    // will run when the tween is killed/cancelled
    Event<> onKilled;
};

template<typename T>
class Tween final : public TweenBase {
private:
    bool functional = false;
    std::function<void(T)> targetFunc = nullptr;
    T* targetPtr = nullptr;

    T startValue;
    T endValue;

    float duration;
    float elapsed = 0;
    bool finished = false;

    Curve curve;

    Component* owner;

    static T lerp(const T& start, const T& end, float t) {
        return start + (end - start) * t;
    }

public:
    Tween(Component* owner, std::function<void(T)> targetFunc, const T& start, const T& end, float duration, Curve curve) {
        this->targetFunc = targetFunc;
        this->startValue = start;
        this->endValue = end;
        this->duration = duration;
        this->curve = curve;
        this->owner = owner;

        functional = true;
    }
    Tween(Component* owner, T* targetPtr, const T& start, const T& end, float duration, Curve curve) {
        this->targetPtr = targetPtr;
        this->startValue = start;
        this->endValue = end;
        this->duration = duration;
        this->curve = curve;
        this->owner = owner;

        functional = false;
    }

    void start() override {
        owner->addTween(this);
        onStart.invoke();
    }

    bool update(const double deltaTime) override {
        if (finished) // we dont call onFinished here, because if finished == true but we are still alive, it means we were cancelled, onCancel was already run
            return true;

        elapsed += deltaTime;

        if (functional)
            targetFunc(lerp(startValue, endValue, curve.evaluate(elapsed / duration)));
        else
            (*targetPtr) = lerp(startValue, endValue, curve.evaluate(elapsed / duration));

        onUpdate.invoke();

        if (elapsed >= duration) {
            finished = true;
            onFinished.invoke();
            return true;
        }

        return false;
    }

    void kill() override {
        finished = true;
        onKilled.invoke();
    }

    bool getFinished() const override {
        return finished;
    }
};
