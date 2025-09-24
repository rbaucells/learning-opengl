#pragma once
#include <functional>

class Component;

class QueueEntry {
public:
    virtual ~QueueEntry() = default;

    virtual bool run(float deltaTime) {return true;};
};

class ConditionalQueueEntry final : public QueueEntry {
    using Action = std::function<void()>;
    using Condition = std::function<bool()>;

    Condition condition_;
    Action action_;

public:
    ConditionalQueueEntry(const Condition& condition, const Action& action);

    bool run(float deltaTime) override;
};

class TimedQueueEntry final : public QueueEntry {
public:
    TimedQueueEntry(const std::function<void()>& action, float time, int repetitions = 0);

    bool run(float deltaTime) override;
private:
    std::function<void()> action_;
    float duration_ = 0.f;
    float elapsed_ = 0.f;

    int timesToRun_ = 0;
};

class NextFrameQueueEntry final : public QueueEntry {
    using Action = std::function<void()>;
    std::function<void()> action_;

public:
    NextFrameQueueEntry(const Action& action); // NOLINT(google-explicit-constructor)
    bool run(float deltaTime) override;
};
