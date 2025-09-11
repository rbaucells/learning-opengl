#include "workQueue.h"

ConditionalQueueEntry::ConditionalQueueEntry(const Condition& condition, const Action& action) {
    this->condition_ = condition;
    this->action_ = action;
}

bool ConditionalQueueEntry::run() {
    if (condition_()) {
        action_();
        return true;
    }

    return false;
}

TimedQueueEntry::TimedQueueEntry(const Action& action, float time) {
    this->action_ = action;
    this->time_ = time;
}

bool TimedQueueEntry::run() {
    if (time_ > 0)
        return false;

    action_();
    return true;
}

void TimedQueueEntry::decreaseTime(const double deltaTime) {
    this->time_ -= deltaTime;
}

NextFrameQueueEntry::NextFrameQueueEntry(const Action& action) {
    this->action_ = action;
}

bool NextFrameQueueEntry::run() {
    action_();
    return true;
}


