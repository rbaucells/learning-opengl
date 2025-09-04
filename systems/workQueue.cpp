#include "workQueue.h"

/**
 * @brief In main loop, condition will be ran, if true, will run action and remove it from the queue
 * @param owner The owner of the task
 * @param condition What to check every frame
 * @param action What is run if condition is true
 */
void addConditionalQueueEntry(Object *owner, const std::function<bool()> &condition, const std::function<void()> &action) {
    conditionalQueue.push_back(ConditionalQueueEntry(condition, action, owner));
}

/**
 * @brief In main loop, will subtract deltaTime from time, if time < 0, times up, run the action
 * @param owner The owner of the task
 * @param time How many seconds to wait before running the action
 * @param action What to run after time is up
 */
void addTimedQueueEntry(Object *owner, float time, const std::function<void()> &action) {
    timedQueue.push_back(TimedQueueEntry(action, time, owner));
}

/**
 * @brief Will simply run the action at the end of the next frame
 * @param owner The owner of the task
 * @param action What to run at the end of the next frame
 */
void addNextFrameEntry(Object *owner, const std::function<void()> &action) {
    nextFrameQueue.push_back(NextFrameQueueEntry(action, owner));
}

/**
 * @brief Will remove entry from queue
 * @param entry The ConditionalQueueEntry to remove
 */
void removeConditionalQueueEntry(const ConditionalQueueEntry &entry) {
    for (auto it = conditionalQueue.begin(); it != conditionalQueue.end();) {
        if (it->owner == entry.owner && it->condition.target<bool()>() == entry.condition.target<bool()>() && it->action.target<void()>() == entry.action.target<void()>()) {
            conditionalQueue.erase(it);
            break;
        }

        ++it;
    }
}

/**
 * @brief Will remove entry from queue
 * @param entry The TimedQueueEntry to remove
 */
void removeTimedQueueEntry(const TimedQueueEntry &entry) {
    for (auto it = timedQueue.begin(); it != timedQueue.end();) {
        if (it->owner == entry.owner && it->time == entry.time && it->action.target<void()>() == entry.action.target<void()>()) {
            timedQueue.erase(it);
            break;
        }

        ++it;
    }
}

/**
 * @brief Will remove entry from queue
 * @param entry The NextFrameQueueEntry to remove
 */
void removeNextFrameQueueEntry(const NextFrameQueueEntry &entry) {
    for (auto it = nextFrameQueue.begin(); it != nextFrameQueue.end();) {
        if (it->owner == entry.owner && it->action.target<void()>() == entry.action.target<void()>()) {
            nextFrameQueue.erase(it);
            break;
        }

        ++it;
    }
}

/**
 * @brief Removes all queue entries (from all queues) that are owned by owner
 * @param owner The object that owns the entries to remove
 */
void removeAllQueueEntries(const Object *owner) {
    for (auto it = nextFrameQueue.begin(); it != nextFrameQueue.end();) {
        if (it->owner == owner)
            it = nextFrameQueue.erase(it);
        else
            ++it;
    }

    for (auto it = conditionalQueue.begin(); it != conditionalQueue.end();) {
        if (it->owner == owner)
            it = conditionalQueue.erase(it);
        else
            ++it;
    }

    for (auto it = timedQueue.begin(); it != timedQueue.end();) {
        if (it->owner == owner)
            it = timedQueue.erase(it);
        else
            ++it;
    }
}
