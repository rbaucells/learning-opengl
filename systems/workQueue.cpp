#include "workQueue.h"

ConditionalQueueEntry::ConditionalQueueEntry(const Condition& condition, const Action& action) {
    this->condition = condition;
    this->action = action;
}

bool ConditionalQueueEntry::run() {
    if (condition()) {
        action();
        return true;
    }

    return false;
}

TimedQueueEntry::TimedQueueEntry(const Action& action, float time) {
    this->action = action;
    this->time = time;
}

bool TimedQueueEntry::run() {
    if (time > 0)
        return false;

    action();
    return true;
}

void TimedQueueEntry::decreaseTime(const double deltaTime) {
    this->time -= deltaTime;
}

NextFrameQueueEntry::NextFrameQueueEntry(const Action& action) {
    this->action = action;
}

bool NextFrameQueueEntry::run() {
    action();
    return true;
}


