#pragma once
#include <float.h>
#include <functional>

#include "object.h"

class Component;

namespace WorkQueue {
    struct TimedQueueEntry {
        Component* owner;
        float time;
        std::function<void()> action;

        bool operator==(const TimedQueueEntry &other) const {
            return (other.time - time) < FLT_EPSILON && other.action.target<void()>() == action.target<void()>() && other.owner == owner;
        }
    };

    struct ConditionalQueueEntry {
        Component* owner;
        std::function<bool()> condition;
        std::function<void()> action;

        bool operator==(const ConditionalQueueEntry &other) const {
            return other.condition.target<bool()>() == condition.target<bool()>() && other.action.target<void()>() == action.target<void()>() && other.owner == owner;
        }
    };

    struct NextFrameQueueEntry {
        Component* owner;
        std::function<void()> action;

        bool operator==(const NextFrameQueueEntry &other) const {
            return other.action.target<void()>() == action.target<void()>() && other.owner == owner;
        }
    };

    inline std::vector<TimedQueueEntry> timedQueue; // things to do in some time
    void addToTimedQueue(Component* owner, float delay, const std::function<void()>& action); // adds action to queue after delay in seconds

    inline std::vector<ConditionalQueueEntry> conditionalQueue; // things to do if the matching condition is true
    void addToConditionalQueue(Component* owner, const std::function<bool()> &condition, const std::function<void()> &action); // constantly checks if condition is true, if so adds acition to queue

    inline std::vector<NextFrameQueueEntry> nextFrameQueue; // things to do rn
    void addToNextFrameQueue(Component* owner, const std::function<void()> &action); // adds to queue in next frame

    bool removeFromTimedQueue(const TimedQueueEntry& entry);
    bool removeFromConditionalQueue(const ConditionalQueueEntry& entry);
    bool removeFromNextFrameQueue(const NextFrameQueueEntry& entry);

    int removeAllTimedQueue(const Component* owner);
    int removeAllConditionalQueue(const Component* owner);
    int removeAllNextFrameQueue(const Component* owner);

}
