#include "workQueue.h"

ConditionalQueueEntry::ConditionalQueueEntry(const Condition& condition, const Action& action) {
    this->condition_ = condition;
    this->action_ = action;
}

bool ConditionalQueueEntry::run(float deltaTime) {
    if (condition_()) {
        action_();
        return true;
    }

    return false;
}

TimedQueueEntry::TimedQueueEntry(const std::function<void()>& action, const float time, const int repetitions) {
    this->action_ = action;
    this->duration_ = time;
    this->timesToRun_ = repetitions;
}

bool TimedQueueEntry::run(const float deltaTime) {
    elapsed_ += deltaTime;

    if (elapsed_ >= duration_) {
        action_();

        if (timesToRun_ > 0) {
            timesToRun_--;
            elapsed_ = 0;
            return false;
        }

        return true;
    }

    return false;
}

NextFrameQueueEntry::NextFrameQueueEntry(const Action& action) {
    this->action_ = action;
}

bool NextFrameQueueEntry::run(float deltaTime) {
    action_();
    return true;
}


