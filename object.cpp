#include "components/component.h"
#include "object.h"
#include "main.h"

#include <vector>

Object::Object(const Transform &transform) {
    this->transform = transform;

    // when its update/lateUpdate/fixedUpdate time, lmk
    updateEvent.subscribe(this, &Object::update);
    lateUpdateEvent.subscribe(this, &Object::lateUpdate);
    fixedUpdateEvent.subscribe(this, &Object::fixedUpdate);
}

void Object::update(double deltaTime) const {
    for (const auto &component: components) {
        component->update(deltaTime);
    }
}

void Object::fixedUpdate(double fixedDeltaTime) const {
    for (const auto &component: components) {
        component->fixedUpdate(fixedDeltaTime);
    }
}

void Object::lateUpdate(double deltaTime) const {
    for (const auto &component: components) {
        component->lateUpdate(deltaTime);
    }
}
