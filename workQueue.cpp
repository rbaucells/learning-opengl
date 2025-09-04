#include "workQueue.h"

void addConditionalQueueEntry(Object *owner, std::function<bool()> condition, std::function<void()> action) {
    conditionalQueue.push_back(ConditionalQueueEntry(condition, action, owner));
}

void addTimedQueueEntry(Object *owner, float time, std::function<void()> action) {
    timedQueue.push_back(TimedQueueEntry(action, time, owner));
}

void addNextFrameEntry(Object *owner, std::function<void()> action) {
    nextFrameQueue.push_back(NextFrameQueueEntry(action, owner));
}
