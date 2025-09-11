#pragma once
#include <functional>

class Component;

class QueueEntry {
public:
    virtual ~QueueEntry() = default;

    virtual bool run() {return true;};
};

class ConditionalQueueEntry final : public QueueEntry {
    using Action = std::function<void()>;
    using Condition = std::function<bool()>;

    Condition condition_;
    Action action_;

public:
    ConditionalQueueEntry(const Condition& condition, const Action& action);

    bool run() override;
};

class TimedQueueEntry final : public QueueEntry {
    using Action = std::function<void()>;

    Action action_;
    float time_ = 0.f;

public:
    TimedQueueEntry(const Action& action, float time);

    bool run() override;
    void decreaseTime(double deltaTime);
};

class NextFrameQueueEntry final : public QueueEntry {
    using Action = std::function<void()>;
    std::function<void()> action_;

public:
    NextFrameQueueEntry(const Action& action); // NOLINT(google-explicit-constructor)
    bool run() override;
};
