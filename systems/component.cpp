#include "component.h"
#include "tweens/tween.h"

Component::Component(Object* owner) {
    this->object = owner;
};

void Component::manageQueue(float deltaTime) {
    for (auto it = queue_.begin(); it != queue_.end(); ) {
        if ((*it)->run(deltaTime))
            it = queue_.erase(it);
        else
            ++it;
    }
}

void Component::manageTweens(float deltaTime) {
    for (auto it = tweens_.begin(); it != tweens_.end(); ) {
        (*it)->update(deltaTime);

        if ((*it)->shouldDelete()) {
            std::printf("Deleting tween \n");
            it = tweens_.erase(it);
        }
        else {
            ++it;
        }
    }
}