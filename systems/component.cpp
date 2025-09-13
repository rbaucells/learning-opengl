#include "component.h"
#include "tweens/tween.h"

Component::Component(Object* owner) {
    this->object = owner;
};

void Component::manageQueue(float deltaTime) {
    for (auto it = queue.begin(); it != queue.end(); ) {
        if ((*it)->run(deltaTime))
            it = queue.erase(it);
        else
            ++it;
    }
}

void Component::manageTweens(float deltaTime) {
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