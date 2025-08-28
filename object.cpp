#include "components/component.h"
#include "object.h"
#include "main.h"

#include <vector>

Object::Object(const std::string &objectName, const int objectTag, const Transform &transform) : name(objectName), tag(objectTag) {
    this->transform = transform;

    allObjects.push_back(this);

    // when its update/lateUpdate/fixedUpdate time, lmk
    updateEvent.subscribe(this, &Object::update);
    lateUpdateEvent.subscribe(this, &Object::lateUpdate);
    fixedUpdateEvent.subscribe(this, &Object::fixedUpdate);
}

void Object::update(double deltaTime) const {
    if (!activated)
        return;
    for (const auto &component: components) {
        component->update(deltaTime);
    }
}

void Object::fixedUpdate(double fixedDeltaTime) const {
    if (!activated)
        return;
    for (const auto &component: components) {
        component->fixedUpdate(fixedDeltaTime);
    }
}

void Object::lateUpdate(double deltaTime) const {
    if (!activated)
        return;
    for (const auto &component: components) {
        component->lateUpdate(deltaTime);
    }
}

void Object::destroy() {
    components.clear(); // kill al the components
    delete this; // kys
}

// static methods

Object *Object::findObjectByName(const std::string &name) {
    for (Object *obj: allObjects) {
        if (obj->name == name)
            return obj;
    }

    return nullptr;
}

Object *Object::findObjectByTag(const int tag) {
    for (Object *obj: allObjects) {
        if (obj->tag == tag)
            return obj;
    }

    return nullptr;
}
