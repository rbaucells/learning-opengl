#include "object.h"
#include "systems/component.h"

#include <utility>
#include <vector>

#include "nanoId.h"
#include "scene.h"

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
    for (const auto& comp : componentsToDestroy_) {
        if (enabled_) {
            comp->onDisable();
        }

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

void Object::queueDestruction() {
    scene->removeObject(shared_from_this());
}

std::weak_ptr<Component> Object::getComponentById(const std::string& id) const {
    for (const auto& component : components_) {
        if (component->id == id) {
            return std::weak_ptr(component);
        }
    }

    // nothing was found
    return std::weak_ptr<Component>();
}

void Object::removeComponent(const std::shared_ptr<Component>& compPtr) {
    // if its not alr in there
    if (std::ranges::find(componentsToDestroy_, compPtr) != componentsToDestroy_.end()) {
        return;
    }

    componentsToDestroy_.push_back(compPtr);
}

void Object::removeComponentBy(const std::function<bool(const Component&)>& predicate) {
    for (auto& comp : components_) {
        if (predicate(*comp)) {
            removeComponent(comp);
        }
    }
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

Object::Object(Scene* scene, std::string objectName, const int objectTag, const Vector2 pos, const float rot, const Vector2 scale) : name(std::move(objectName)), tag(objectTag), transform(this, pos, rot, scale), scene(scene), id(NanoId::nanoIdGen()) {}

Object::Object(Scene* scene, std::string objectName, const int objectTag, Vector2 pos, float rot, Vector2 scale, Transform* parent) : name(std::move(objectName)), tag(objectTag), transform(this, pos, rot, scale, parent), scene(scene), id(NanoId::nanoIdGen()) {}
