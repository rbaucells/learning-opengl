#include "object.h"
#include "systems/component.h"

#include <utility>
#include <vector>

#include "scene.h"

Object::Object(std::string objectName, const int objectTag, const Vector2 pos, const float rot, const Vector2 scale) : name(std::move(objectName)), tag(objectTag), transform(this, pos, rot, scale) {}

Object::Object(std::string objectName, const int objectTag, Vector2 pos, float rot, Vector2 scale, Transform* parent) : name(std::move(objectName)), tag(objectTag), transform(this, pos, rot, scale, parent) {}

void Object::manageStarts() {
    for (const auto& componentPtr : componentsToStart_) {
        componentPtr->awake();
    }

    for (const auto& componentPtr : componentsToStart_) {
        componentPtr->start();
    }

    if (enabled_) {
        for (const auto& componentPtr : componentsToStart_) {
            componentPtr->onEnable();
        }
    }

    componentsToStart_.clear();
}

void Object::manageDestructions() {
    if (enabled_) {
        for (const auto& componentPtr : componentsToDestroy_) {
            componentPtr->onDisable();
        }
    }

    for (const auto& comp : componentsToDestroy_) {
        comp->onDestroy();
    }

    // erase the component from components_ if the component is also in componentsToDestroy
    std::erase_if(components_,
                  [this](const std::shared_ptr<Component>& comp) {
                      return std::ranges::find(componentsToDestroy_, comp) != componentsToDestroy_.end();
                  });

    componentsToDestroy_.clear();
}

void Object::update(const float deltaTime) {
    if (!enabled_)
        return;

    for (const auto& component : components_) {
        component->update(deltaTime);
    }

    transform.update(deltaTime);

    for (const auto& component : components_) {
        component->manageTweens(deltaTime);
    }

    transform.manageTweens(deltaTime);

    for (const auto& component : components_) {
        component->manageQueue(deltaTime);
    }

    transform.manageQueue(deltaTime);
}

void Object::fixedUpdate(const float fixedDeltaTime) const {
    if (!enabled_)
        return;

    for (const auto& component : components_) {
        component->fixedUpdate(fixedDeltaTime);
    }
}

void Object::lateUpdate(const float deltaTime) const {
    if (!enabled_)
        return;

    for (const auto& component : components_) {
        component->lateUpdate(deltaTime);
    }
}

void Object::destroy() {
    scene->objectsToDestroy_.push_back(shared_from_this());
}

void Object::destroyImmediately() {
    // we need to deactivate before destroying
    if (enabled_) {
        for (const auto& comp : components_) {
            comp->onDisable();
        }

        transform.onDisable();
    }

    for (const auto& comp : components_) {
        comp->onDestroy();
    }

    transform.onDestroy();

    components_.clear();
    componentsToDestroy_.clear();
    componentsToStart_.clear();
}

void Object::setActive(const bool state) {
    // if activated but not anymore
    if (enabled_ && !state) {
        for (const auto& component : components_) {
            component->onDisable();
        }
    }
    // if not activated but are now
    else if (!enabled_ && state) {
        for (const auto& component : components_) {
            component->onEnable();
        }
    }

    enabled_ = state;
}

bool Object::getActive() const {
    return enabled_;
}
