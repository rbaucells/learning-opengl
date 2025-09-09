#pragma once
#include <functional>

class Component;

class QueueEntry {
public:
    virtual ~QueueEntry() = default;

private:
    virtual bool run() {return true;};
};

class ConditionalQueueEntry final : public QueueEntry {
    using Action = std::function<void()>;
    using Condition = std::function<bool()>;

    Condition condition;
    Action action;

public:
    ConditionalQueueEntry(const Condition& condition, const Action& action);

    bool run() override;
};

class TimedQueueEntry final : public QueueEntry {
    using Action = std::function<void()>;

    Action action;
    float time = 0.f;

public:
    TimedQueueEntry(const Action& action, float time);

    bool run() override;
    void decreaseTime(double deltaTime);
};

class NextFrameQueueEntry final : public QueueEntry {
    using Action = std::function<void()>;
    std::function<void()> action;

public:
    NextFrameQueueEntry(const Action& action); // NOLINT(google-explicit-constructor)
    bool run() override;
};
