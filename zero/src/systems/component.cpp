#include "component.h"
#include "json++/json.h"

#include "../src/nanoId.h"
#include "../object.h"
#include "tweens/tween.h"

Component::Component(Object* const owner) : object(owner), id(NanoId::nanoIdGen()) {}

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
        }
        else {
            ++it;
        }
    }
}

JsonObject Component::serialize() const {
    return {};
}

void Component::queueDestruction() {
    object->removeComponent(shared_from_this());
}
