#include "component.h"
#include "tweens/tween.h"

Component::Component(Object* owner) {
    this->object = owner;
};

void Component::manageQueue(const double deltaTime) {
    for (auto it = queue.begin(); it != queue.end(); ) {
        if (const auto conditionalQueueEntry = dynamic_cast<ConditionalQueueEntry*>(it->get())) {
            if (conditionalQueueEntry->run()) {
                it = queue.erase(it);
            }
            else {
                ++it;
            }
        }
        else {
            if (const auto timedQueueEntry = dynamic_cast<TimedQueueEntry*>(it->get())) {
                timedQueueEntry->decreaseTime(deltaTime);
                if (timedQueueEntry->run()) {
                    it = queue.erase(it);
                }
                else {
                    ++it;
                }
            }
            else if (const auto nextFrameQueueEntry = dynamic_cast<NextFrameQueueEntry*>(it->get())) {
                nextFrameQueueEntry->run();
                it = queue.erase(it);
            }
        }
    }
}

void Component::manageTweens(const double deltaTime) {
    for (auto it = tweens.begin(); it != tweens.end(); ) {
        (*it)->update(deltaTime);

        if ((*it)->shouldDelete()) {
            std::printf("Deleting tween \n");
            it = tweens.erase(it);
        }
        else {
            ++it;
        }
    }
}