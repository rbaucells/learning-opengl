#pragma once
#include <queue>
#include "../event.h"
#include "../../math/curve.h"

/**
 * @brief Virtual base class for all tweens. Derive from it to make custom tweens
 *
 * start() <- called to start the tween
 *
 * update(double deltaTime) <- called on every frame by owning component
 *
 * complete() <- forces the tween to complete. insta moving the interpolated value to end
 *
 * cancel() <- cancels the tween, leaving the interpolated value as-is
 *
 * Events <- called onStart, onComplete, and onCancel
 */
class TweenBase {
public:
    virtual ~TweenBase() = default;

    virtual void start() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void complete() = 0;
    virtual void cancel() = 0;

    virtual bool shouldDelete() = 0;

    Event<> onStart;
    Event<> onComplete;
    Event<> onCancel;

protected:
    bool completed_ = false;
    bool running_ = false;
    bool canceled_ = false;
};

/**
 * @brief Interpolates target by settings its value. Derives from TweenBase
 * @tparam T The type of thing to be interpolated
 * @note template param T must be able to do arithmatic operations (+, -, and * a float)
 */
template<typename T>
class Tween final : public TweenBase {
public:
    Tween(T* target, const T& start, const T& end, const double duration, const Curve& curve = Curve::linear) {
        target_ = target;
        start_ = start;
        end_ = end;
        duration_ = duration;
        curve_ = curve;
    }

    void start() override {
        *target_ = start_;
        onStart.invoke();
    }

    void update(const float deltaTime) override {
        elapsed_ += deltaTime;

        const double t = curve_.evaluate(elapsed_ / duration_);
        *target_ = start_ + (end_ - start_) * t;

        if (elapsed_ > duration_) {
            complete();
        }
    }

    void complete() override {
        *target_ = end_;
        completed_ = true;
        onComplete.invoke();
    }

    void cancel() override {
        canceled_ = true;
        onCancel.invoke();
    }

    bool shouldDelete() override {
        return canceled_ || completed_;
    }

private:
    T* target_;
    T start_;
    T end_;

    double duration_ = 0.f;
    double elapsed_ = 0.f;

    Curve curve_;
};

/**
 * @brief Interpolates target by calling targetFunc with the new value as a parameter. Derives from TweenBase
 * @tparam T The type of thing to be interpolated
 */
template<typename T>
class FunctionalTween final : public TweenBase {
public:
    FunctionalTween(const std::function<void(T)>& targetFunc, const T& start, const T& end, const double duration, const Curve& curve = Curve::linear) {
        targetFunc_ = targetFunc;
        start_ = start;
        end_ = end;
        duration_ = duration;
        curve_ = curve;
    }

    void start() override {
        targetFunc_(start_);
        onStart.invoke();
    }

    void update(const float deltaTime) override {
        elapsed_ += deltaTime;

        const double t = curve_.evaluate(elapsed_ / duration_);
        targetFunc_(start_ + (end_ - start_) * t);

        if (elapsed_ > duration_) {
            complete();
        }
    }

    void complete() override {
        targetFunc_(end_);
        completed_ = true;
        onComplete.invoke();
    }

    void cancel() override {
        canceled_ = true;
        onCancel.invoke();
    }

    bool shouldDelete() override {
        return canceled_ || completed_;
    }

private:
    std::function<void(T)> targetFunc_;
    T start_;
    T end_;

    double duration_ = 0.f;
    double elapsed_ = 0.f;

    Curve curve_;
};

/**
 * @brief An empty tween primarly used in sequences to add a delay
 *
 * implements from TweenBase
 */
class WaitTween : public TweenBase {
public:
    explicit WaitTween(double duration);

    void start() override;
    void update(float deltaTime) override;
    void complete() override;
    void cancel() override;

    bool shouldDelete() override;

private:
    double duration_ = 0.f;
    double elapsed_ = 0.f;
};

class CallbackTween final : public TweenBase {
public:
    explicit CallbackTween(const std::function<void()>& func);

    void start() override;
    void update(float deltaTime) override;
    void complete() override;
    void cancel() override;

    bool shouldDelete() override {
        return true;
    }
private:
    std::function<void()> func_;
};

/**
 * @brief Tween that manages other tweens in a sequence
 * To add it to a component you do addTween(std::make_unique<SequenceTween>())
 * then do operations on the returned pointer
 */
class SequenceTween final : public TweenBase {
private:
    std::queue<std::vector<std::unique_ptr<TweenBase>>> tweens_;

public:
    void add(std::unique_ptr<TweenBase> tween);
    void join(std::unique_ptr<TweenBase> tween);

    void start() override;
    void update(float deltaTime) override;
    void complete() override;
    void cancel() override;

    bool shouldDelete() override;
};
