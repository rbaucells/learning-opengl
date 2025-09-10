#pragma once
#include <queue>

#include "../event.h"
#include "math/curve.h"

class TweenBase {
public:
    virtual ~TweenBase() = default;

    virtual void start() = 0;
    virtual void update(double deltaTime) = 0;
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

class LocalPositionTween final : public TweenBase {
public:
    LocalPositionTween(Vector2* target, const Vector2& start, const Vector2& end, double duration, Curve curve);

    void start() override;
    void update(double deltaTime) override;
    void complete() override;
    void cancel() override;

    bool shouldDelete() override;

private:
    Vector2* target_;
    Vector2 start_;
    Vector2 end_;

    Curve curve_;

    double duration_ = 0.f;
    double elapsed_ = 0.f;
};

class LocalRotationTween final : public TweenBase {
public:
    LocalRotationTween(float* target, float start, float end, double duration, const Curve& curve);

    void start() override;
    void update(double deltaTime) override;
    void complete() override;
    void cancel() override;

    bool shouldDelete() override;

private:
    float* target_;
    float start_;
    float end_;

    Curve curve_;

    double duration_ = 0.f;
    double elapsed_ = 0.f;
};

class LocalScaleTween final : public TweenBase {
public:
    LocalScaleTween(Vector2* target, const Vector2& start, const Vector2& end, double duration, const Curve& curve);

    void start() override;
    void update(double deltaTime) override;
    void complete() override;
    void cancel() override;

    bool shouldDelete() override;

private:
    Vector2* target_;
    Vector2 start_;
    Vector2 end_;

    Curve curve_;

    double duration_ = 0.f;
    double elapsed_ = 0.f;
};

class SequenceTween : public TweenBase {
private:
    std::queue<std::unique_ptr<TweenBase>> tweens_;
    TweenBase* currentTween_ = nullptr;
public:
    void add(std::unique_ptr<TweenBase> tween);

    void start() override;
    void update(double deltaTime) override;
    void complete() override;
    void cancel() override;

    bool shouldDelete() override;
};
