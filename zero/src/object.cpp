#include "object.h"
#include "systems/component.h"

#include <utility>
#include <vector>

#include "nanoId.h"
#include "scene.h"
#include "json++/json.h"
#include "serialization/componentRegistry.h"

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

void Object::replaceTrasnform(const std::shared_ptr<Transform>& newTransform) {
    // remove the old one
    std::erase(components_, transform);
    std::erase(componentsToStart_, transform);
    std::erase(componentsToDestroy_, transform);

    // set the new one
    transform = newTransform;

    // setup the new one
    components_.push_back(newTransform);
    componentsToStart_.push_back(newTransform);
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

void Object::fixedUpdate(const float fixedDeltaTime) {
    if (!enabled_)
        return;

    for (const auto& component : components_) {
        component->fixedUpdate(fixedDeltaTime);
    }
}

void Object::lateUpdate(const float deltaTime) {
    if (!enabled_)
        return;

    for (const auto& component : components_) {
        component->lateUpdate(deltaTime);
    }
}

void Object::deserialize(Scene* owner, const JsonObject& jsonObject) {
    struct DoLaterComponent {
        Object* owner;
        JsonObject object;

        std::vector<std::string> dependenciesToWaitFor;
    };

    static std::string sceneId;
    static std::vector<DoLaterComponent> doLaterComponents;
    static std::vector<std::string> ids;

    static auto check_is_in_id_registry = [&](const std::string& id) -> bool {
        for (const auto& i : ids) {
            if (i == id)
                return true;
        }

        return false;
    };

    // ReSharper disable once CppInconsistentNaming
    static std::function<void(Object* owner, const JsonObject& component)> make_component_from_json;

    static auto add_to_id_registry = [&](const std::string& id) -> void {
        ids.push_back(id);

        for (auto it = doLaterComponents.begin(); it != doLaterComponents.end();) {
            auto dependenciesToWaitFor = it->dependenciesToWaitFor;
            for (auto dependencyIt = it->dependenciesToWaitFor.begin(); dependencyIt != it->dependenciesToWaitFor.end();) {
                if (*dependencyIt == id)
                    dependencyIt = it->dependenciesToWaitFor.erase(dependencyIt);
                else
                    ++dependencyIt;
            }

            if (it->dependenciesToWaitFor.empty()) {
                Object* componentOwner = it->owner;
                JsonObject object = it->object;

                it = doLaterComponents.erase(it);

                make_component_from_json(componentOwner, object);
            }
            else {
                ++it;
            }
        }
    };

    make_component_from_json = [&](Object* componentOwner, const JsonObject& component) -> void {
        if (JsonArray dependencies = component.getArrayField("dependencies"); !dependencies.empty()) {
            std::vector<std::string> dependenciesToWaitFor;

            for (std::string dependency : dependencies) {
                if (!check_is_in_id_registry(dependency)) {
                    dependenciesToWaitFor.push_back(dependency);
                }
            }

            if (!dependenciesToWaitFor.empty()) {
                doLaterComponents.emplace_back(componentOwner, component, dependenciesToWaitFor);
                return;
            }
        }

        const std::string type = component.getStringField("type");
        const std::string id = component.getStringField("id");

        const std::shared_ptr<Component> comp = ComponentRegistry::create(type, componentOwner, component);

        if (type == "Transform") {
            componentOwner->replaceTrasnform(std::dynamic_pointer_cast<Transform>(comp));
        }
        else {
            componentOwner->addComponent(comp);
        }

        add_to_id_registry(id);
    };

    // reset the sceneId and doLaterComponents when we are deserializing a new scene
    if (sceneId != owner->id) {
        sceneId = owner->id;
        doLaterComponents.clear();
        ids.clear();
        ids.push_back(owner->id);
    }

    const std::string name = jsonObject.getStringField("name");
    const int tag = static_cast<int>(jsonObject.getNumberField("tag"));
    const std::string objectId = jsonObject.getStringField("id");
    const bool enabled = jsonObject.getBoolField("enabled");

    std::weak_ptr<Object> weakObject = owner->addObject(name, tag, objectId);

    add_to_id_registry(objectId);

    JsonArray components = jsonObject.getArrayField("components");

    if (auto sharedObject = weakObject.lock()) {
        sharedObject->setActive(enabled);

        for (JsonObject component : components) {
            make_component_from_json(sharedObject.get(), component);
        }

        sharedObject->manageDestructions();
    }
}

Object::Object(Scene* scene, std::string objectName, const int objectTag, const std::string& id) : name(std::move(objectName)), id(id.empty() ? NanoId::nanoIdGen() : id), tag(objectTag), transform(std::make_shared<Transform>(ComponentParams(this, NanoId::nanoIdGen()))), scene(scene) {
    components_.push_back(transform);
    componentsToStart_.push_back(transform);
}
