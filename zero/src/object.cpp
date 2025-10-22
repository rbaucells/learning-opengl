#include "object.h"
#include "systems/component.h"

#include <utility>
#include <vector>

#include "nanoId.h"
#include "scene.h"
#include "json++/json.h"

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

    for (const auto& component : components_) {
        component->manageTweens(deltaTime);
    }

    for (const auto& component : components_) {
        component->manageQueue(deltaTime);
    }
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

JsonObject Object::serialize() const {
    JsonObject jsonObject;

    jsonObject.putStringField("id", id);
    jsonObject.putStringField("name", name);
    jsonObject.putNumberField("tag", tag);

    JsonArray jsonComponents;

    for (const auto& component : components_) {
        jsonComponents.putObject(component->serialize());
    }

    jsonObject.putArrayField("components", jsonComponents);

    return jsonObject;
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

std::weak_ptr<Component> Object::getComponentBy(const std::function<bool(const std::shared_ptr<Component>&)>& predicate) const {
    for (const auto& component : components_) {
        if (predicate(component)) {
            return component;
        }
    }

    return {};
}

std::vector<std::weak_ptr<Component>> Object::getAllComponentsBy(const std::function<bool(const std::shared_ptr<Component>&)>& predicate) const {
    std::vector<std::weak_ptr<Component>> foundComponents;

    for (const auto& component : components_) {
        if (predicate(component))
            foundComponents.push_back(component);
    }

    return foundComponents;
}

void Object::removeComponentBy(const std::function<bool(const std::shared_ptr<Component>&)>& predicate) {
    for (const auto& component : components_) {
        if (predicate(component)) {
            componentsToDestroy_.push_back(component);
            return;
        }
    }
}

void Object::removeAllComponentsBy(const std::function<bool(const std::shared_ptr<Component>&)>& predicate) {
    for (const auto& component : components_) {
        if (predicate(component)) {
            componentsToDestroy_.push_back(component);
        }
    }
}

void Object::removeComponent(const std::shared_ptr<Component>& component) {
    componentsToDestroy_.push_back(component);
}

void Object::removeComponentById(const std::string& componentId) {
    removeComponentBy([componentId](const std::shared_ptr<Component>& component) {
        return component->id == componentId;
    });
}

Object::Object(Scene* scene, std::string objectName, const int objectTag, const std::string& id) : name(std::move(objectName)), id(id.empty() ? NanoId::nanoIdGen() : id), tag(objectTag), scene(scene) {
    transform = std::make_shared<Transform>(ComponentParams(this, NanoId::nanoIdGen()));

    components_.push_back(transform);
    componentsToStart_.push_back(transform);
}
