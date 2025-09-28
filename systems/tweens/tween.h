#pragma once
#include <queue>

#include "../betterEvents.h"
#include "../event.h"
#include "../../math/curve.h"

// if AUTO_KILL is true, then when a tween has completed naturally its time it starts to return true to shouldDelete()
// if AUTO_KILL is false, then when a tween has completed naturally it just chills, it does call complete but doesnt return true to shouldDelete() until it is manually complete() or cancel().
constexpr bool AUTO_KILL = true;

class TweenBase {
public:
    virtual void start() = 0;
    virtual bool update(float deltaTime) = 0;
    virtual void forceComplete() = 0;
    virtual void forceCancel() = 0;

    virtual ~TweenBase() = default;

    Publisher<> onStart;
    Publisher<> onCancel;
    Publisher<> onComplete;
};

template<typename T>
class Tween final : public TweenBase {
public:
    Tween(T* target, const T& start, const T& end, const float duration, const Curve& curve = Curve::linear) {
        this->target_ = target;
        this->start_ = start;
        this->end_ = end;
        this->duration_ = duration;
        this->curve_ = curve;
    }

    void start() override {
        onStart.invoke();
        *target_ = start_;
        done_ = false;
        elapsed_ = 0;
    }

    bool update(const float deltaTime) override {
        // natural completion
        if (elapsed_ >= duration_ && !naturallyCompleted_) {
            naturallyCompleted_ = true;
            onComplete.invoke();
        }

        // means we either (were force completed, or were force cancelled)
        if (done_)
            return true;


        // if we finished naturally and AUTO_KILL is on we will kill ourself else if we finished naturally and if AUTO_KILL
        // is off then we wont kill ourself until manually completed
        if (naturallyCompleted_)
            return AUTO_KILL;

        elapsed_ += deltaTime;

        const float t = curve_.evaluate(elapsed_ / duration_);
        *target_ = start_ + (end_ - start_) * t;

        return false;
    }

    void forceComplete() override {
        done_ = true;
        *target_ = end_;
        onComplete.invoke();
    }

    void forceCancel() override {
        done_ = true;
        onCancel.invoke();
    }

private:
    T* target_;
    T start_;
    T end_;

    float duration_ = 0;
    float elapsed_ = 0;

    Curve curve_;

    bool done_ = false;
    bool naturallyCompleted_ = false;
};

template<typename T>
class FunctionalTween final : public TweenBase {
public:
    FunctionalTween(const std::function<void(T)>& targetFunc, const T& start, const T& end, const float duration, const Curve& curve = Curve::linear) {
        this->targetFunc_ = targetFunc;
        this->start_ = start;
        this->end_ = end;
        this->duration_ = duration;
        this->curve_ = curve;
    }

    void start() override {
        onStart.invoke();

        targetFunc_(start_);
        done_ = false;
        elapsed_ = 0;
    }

    bool update(const float deltaTime) override {
        // natural completion
        if (elapsed_ >= duration_ && !naturallyCompleted_) {
            naturallyCompleted_ = true;
            onComplete.invoke();
        }

        // means we either (were force completed, or were force cancelled)
        if (done_)
            return true;


        // if we finished naturally and AUTO_KILL is on we will kill ourself else if we finished naturally and if AUTO_KILL
        // is off then we wont kill ourself until manually completed
        if (naturallyCompleted_)
            return AUTO_KILL;

        elapsed_ += deltaTime;

        const float t = curve_.evaluate(elapsed_ / duration_);
        targetFunc_(start_ + (end_ - start_) * t);

        return false;
    }

    void forceComplete() override {
        onComplete.invoke();
        targetFunc_(end_);
        done_ = true;
    }

    void forceCancel() override {
        onCancel.invoke();
        done_ = true;
    }

private:
    std::function<void(T)> targetFunc_;
    T start_;
    T end_;

    float duration_ = 0;
    float elapsed_ = 0;

    Curve curve_;

    bool done_ = false;
    bool naturallyCompleted_ = false;
};

class WaitTween final : public TweenBase {
public:
    explicit WaitTween(float duration);

    void start() override;
    bool update(float deltaTime) override;
    void forceComplete() override;
    void forceCancel() override;

private:
    float duration_ = 0;
    float elapsed_ = 0;

    bool done_ = false;
    bool naturallyCompleted_ = false;
};

class CallbackTween final : public TweenBase {
public:
    explicit CallbackTween(const std::function<void()>& func);

    void start() override;
    bool update(float deltaTime) override;
    void forceComplete() override;
    void forceCancel() override;

private:
    std::function<void()> func_;
};

class SequenceTween final : public TweenBase {
public:
    void add(std::unique_ptr<TweenBase> tween);
    void join(std::unique_ptr<TweenBase> tween);

    void start() override;
    bool update(float deltaTime) override;
    void forceComplete() override;
    void forceCancel() override;

private:
    std::queue<std::vector<std::unique_ptr<TweenBase>>> tweens_;

    bool done_ = false;
    bool naturallyCompleted_ = false;
};
