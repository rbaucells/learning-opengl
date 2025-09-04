#pragma once
#include <functional>
#include <vector>

class Object;

struct ConditionalQueueEntry {
    std::function<bool()> condition;
    std::function<void()> action;
    Object* owner;
};

struct TimedQueueEntry {
    std::function<void()> action;
    float time;
    Object* owner;
};

struct NextFrameQueueEntry {
    std::function<void()> action;
    Object* owner;
};

inline std::vector<ConditionalQueueEntry> conditionalQueue;
inline std::vector<TimedQueueEntry> timedQueue;
inline std::vector<NextFrameQueueEntry> nextFrameQueue;

void addConditionalQueueEntry(Object* owner, std::function<bool()> condition, std::function<void()> action);
void addTimedQueueEntry(Object* owner, float time, std::function<void()> action);
void addNextFrameEntry(Object* owner, std::function<void()> action);