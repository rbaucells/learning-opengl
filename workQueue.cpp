#include "workQueue.h"
#include <thread>

void WorkQueue::addToTimedQueue(Component* owner, const float delay, const std::function<void()> &action) {
    timedQueue.push_back(TimedQueueEntry(owner, delay, action));
}

void WorkQueue::addToNextFrameQueue(Component* owner, const std::function<void()> &action) {
    nextFrameQueue.push_back(NextFrameQueueEntry(owner, action));
}

void WorkQueue::addToConditionalQueue(Component* owner, const std::function<bool()> &condition, const std::function<void()> &action) {
    conditionalQueue.push_back(ConditionalQueueEntry(owner, condition, action));
}

bool WorkQueue::removeFromTimedQueue(const TimedQueueEntry &entry) {
    for (const TimedQueueEntry& tqe : timedQueue) {
        if (tqe == entry) {
            std::erase(timedQueue, tqe);
            return true;
        }
    }

    return false;
}

bool WorkQueue::removeFromConditionalQueue(const ConditionalQueueEntry &entry) {
    for (const ConditionalQueueEntry& cqe : conditionalQueue) {
        if (cqe == entry) {
            std::erase(conditionalQueue, cqe);
            return true;
        }
    }

    return false;
}

bool WorkQueue::removeFromNextFrameQueue(const NextFrameQueueEntry &entry) {
    for (const NextFrameQueueEntry& nfqe : nextFrameQueue) {
        if (nfqe == entry) {
            std::erase(nextFrameQueue, nfqe);
            return true;
        }
    }

    return false;
}

int WorkQueue::removeAllTimedQueue(const Component *owner) {
    int removed = 0;

    for (auto it = timedQueue.rbegin(); it != timedQueue.rend(); ++it) {
        if (it->owner == owner) {
            std::erase(timedQueue, *it);
            removed++;
        }
        else {
            ++it;
        }
    }

    return removed;
}

int WorkQueue::removeAllConditionalQueue(const Component *owner) {
    int removed = 0;

    for (auto it = conditionalQueue.rbegin(); it != conditionalQueue.rend(); ++it) {
        if (it->owner == owner) {
            std::erase(conditionalQueue, *it);
            removed++;
        }
        else {
            ++it;
        }
    }

    return removed;
}

int WorkQueue::removeAllNextFrameQueue(const Component *owner) {
    int removed = 0;

    for (auto it = nextFrameQueue.rbegin(); it != nextFrameQueue.rend(); ++it) {
        if (it->owner == owner) {
            std::erase(nextFrameQueue, *it);
            removed++;
        }
        else {
            ++it;
        }
    }

    return removed;
}