#include "component.h"
#include "tweens/tween.h"

Component::Component(Object* owner) {
    this->object = owner;
};

void Component::manageQueue(const float deltaTime) {
    for (auto it = queue_.begin(); it != queue_.end();) {
        if ((*it)->run(deltaTime))
            it = queue_.erase(it);
        else
            ++it;
    }
}

void Component::manageTweens(const float deltaTime) {
    for (auto it = tweens_.begin(); it != tweens_.end();) {
        if ((*it)->update(deltaTime)) {
            it = tweens_.erase(it);
            printf("Erased Tween\n");
        }
        else {
            ++it;
            printf("Incrementing Tween It\n");
        }
    }
}
