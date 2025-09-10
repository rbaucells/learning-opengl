#pragma once
#include "component.h"
#include "event.h"
#include "math/curve.h"

// transform.tweenLocalPosition(...) returns a tween that can be started() and that will add it to the update loop and let it run
// if used in sequence.append(transform.tweenLocalPosition(...)) it will instead add it to the sequence
// when the sequence is ran, it will start the tween on its corresponding object, then it will wait until that tween is done and then run the next.
// if there are multiple tweens in the same index, it will run all of them then wait for all of them to finish

class TweenBase {
public:
    virtual void start() = 0;

    virtual void update(const double deltaTime) = 0;

    virtual void complete() = 0;
    virtual void cancel() = 0;

    virtual bool isDone() = 0;

    virtual ~TweenBase() = default;
};

template<typename T>
class Tween : public TweenBase {
    T* ptr = nullptr;

    T startingValue;
    T endingValue;

    double duration = 0.f;
    double elapsed = 0.f;

    bool running = false;

    Curve curve;

public:
    Tween(T* ptr, const T& start, const T& end, const double duration, Curve curve) : ptr(ptr), startingValue(start), endingValue(end), duration(duration), curve(curve) {
        std::printf("Created tween \n");
    }

    void start() override {
        std::printf("Started tween \n");

        running = true;
        onStart.invoke();
    }

    void update(const double deltaTime) override {
        elapsed += deltaTime;
        *ptr = startingValue + (endingValue - startingValue) * curve.evaluate(elapsed / duration);

        running = elapsed <= duration;
    }

    void complete() override {
        std::printf("Completed tween \n");

        running = false;
        onComplete.invoke();
    }

    void cancel() override {
        std::printf("Cancelled tween \n");

        running = false;
        onCancel.invoke();
    }

    bool isDone() override {
        return !running;
    }

    // Events
    Event<> onStart;
    Event<> onComplete;
    Event<> onCancel;
};
