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

void addConditionalQueueEntry(Object* owner, const std::function<bool()> &condition, const std::function<void()> &action);
void addTimedQueueEntry(Object* owner, float time, const std::function<void()> &action);
void addNextFrameEntry(Object* owner, const std::function<void()> &action);

void removeConditionalQueueEntry(const ConditionalQueueEntry &entry);
void removeTimedQueueEntry(const TimedQueueEntry &entry);
void removeNextFrameQueueEntry(const NextFrameQueueEntry &entry);

void removeAllQueueEntries(const Object *owner);