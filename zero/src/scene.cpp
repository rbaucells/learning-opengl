#include "scene.h"

#include <vector>
#include "nanoId.h"
#include "object.h"
#include "json++/json.h"
#include "serialization/componentRegistry.h"

Scene::Scene(const std::string& id) : id(id.empty() ? NanoId::nanoIdGen() : id) {}

void Scene::update(const float deltaTime) const {
    for (const auto& objectPtr : objects_)
        objectPtr->update(deltaTime);
}

void Scene::fixedUpdate(const float fixedDeltaTime) const {
    for (const auto& objectPtr : objects_)
        objectPtr->fixedUpdate(fixedDeltaTime);
}

void Scene::lateUpdate(const float lateDeltaTime) const {
    for (const auto& objectPtr : objects_)
        objectPtr->lateUpdate(lateDeltaTime);
}

void Scene::manageStarts() const {
    for (const auto& objectPtr : objects_)
        objectPtr->manageStarts();
}

void Scene::manageDestructions() {
    for (const auto& objectPtr : objects_) {
        objectPtr->manageDestructions();
    }

    for (const auto& objectPtr : objectsToDestroy_) {
        // we need to deactivate before destroying
        const bool enabled = objectPtr->enabled_;

        for (const auto& comp : objectPtr->components_) {
            if (enabled) {
                comp->onDisable();
            }

            comp->onDestroy();
        }

        objectPtr->components_.clear();
        objectPtr->componentsToDestroy_.clear();
        objectPtr->componentsToStart_.clear();
    }

    std::erase_if(objects_,
                  [this](const std::shared_ptr<Object>& obj) {
                      return std::ranges::find(objectsToDestroy_, obj) != objectsToDestroy_.end();
                  });

    objectsToDestroy_.clear();
}

std::weak_ptr<Object> Scene::addObject(const std::string& objectName, const int objectTag, const std::string& id) {
    return objects_.emplace_back(new Object(this, objectName, objectTag, id));
}

std::weak_ptr<Object> Scene::getObjectBy(const std::function<bool(const std::shared_ptr<Object>&)>& predicate) const {
    for (const auto& objectPtr : objects_) {
        if (predicate(objectPtr))
            return objectPtr;
    }

    return {};
}

std::weak_ptr<Object> Scene::getObjectByName(const std::string& objectName) const {
    return getObjectBy([objectName](const std::shared_ptr<Object>& object) {
        return object->name == objectName;
    });
}

std::weak_ptr<Object> Scene::getObjectById(const std::string& objectId) const {
    return getObjectBy([objectId](const std::shared_ptr<Object>& object) {
        return object->id == objectId;
    });
}

std::weak_ptr<Object> Scene::getObjectByTag(int objectTag) const {
    return getObjectBy([objectTag](const std::shared_ptr<Object>& object) {
        return object->tag == objectTag;
    });
}

void Scene::removeObjectBy(const std::function<bool(const std::shared_ptr<Object>&)>& predicate) {
    for (const auto& objectPtr : objects_) {
        if (predicate(objectPtr))
            objectsToDestroy_.push_back(objectPtr);
    }
}

void Scene::removeObjectByName(const std::string& objectName) {
    removeObjectBy([objectName](const std::shared_ptr<Object>& obj) {
        return obj->name == objectName;
    });
}

void Scene::removeObjectById(const std::string& objectId) {
    removeObjectBy([objectId](const std::shared_ptr<Object>& obj) {
        return obj->id == objectId;
    });
}

void Scene::removeObjectByTag(int objectTag) {
    removeObjectBy([objectTag](const std::shared_ptr<Object>& obj) {
        return obj->tag == objectTag;
    });
}

void Scene::removeObject(const std::shared_ptr<Object>& object) {
    objectsToDestroy_.push_back(object);
}

std::weak_ptr<Component> Scene::getComponentBy(const std::function<bool(const std::shared_ptr<Component>&)>& predicate) const {
    for (const auto& object : objects_) {
        for (const auto& component : object->components_) {
            if (predicate(component)) {
                return component;
            }
        }
    }

    return {};
}

std::vector<std::weak_ptr<Component>> Scene::getAllComponentsBy(const std::function<bool(const std::shared_ptr<Component>&)>& predicate) const {
    std::vector<std::weak_ptr<Component>> foundComponents;

    for (const auto& object : objects_) {
        for (const auto& component : object->components_) {
            if (predicate(component))
                foundComponents.push_back(component);
        }
    }

    return foundComponents;
}

void Scene::removeComponentBy(const std::function<bool(const std::shared_ptr<Component>&)>& predicate) {
    for (const auto& object : objects_) {
        for (const auto& component : object->components_) {
            if (predicate(component)) {
                object->componentsToDestroy_.push_back(component);
                return;
            }
        }
    }
}

void Scene::removeAllComponentsBy(const std::function<bool(const std::shared_ptr<Component>&)>& predicate) {
    for (const auto& object : objects_) {
        for (const auto& component : object->components_) {
            if (predicate(component)) {
                object->componentsToDestroy_.push_back(component);
            }
        }
    }
}

void Scene::removeComponentById(const std::string& componentId) {
    removeComponentBy([componentId](const std::shared_ptr<Component>& component) {
        return component->id == componentId;
    });
}

JsonObject Scene::serialize() const {
    JsonObject jsonResult;

    jsonResult.putStringField("id", id);

    JsonArray jsonObjects;

    for (const auto& object : objects_) {
        jsonObjects.putObject(object->serialize());
    }

    jsonResult.putArrayField("objects", jsonObjects);

    return jsonResult;
}

std::unique_ptr<Scene> Scene::deserialize(const std::string& filePath) {
    struct DoLaterComponent {
        Object* owner;
        JsonObject object;

        std::vector<std::string> dependenciesToWaitFor;
    };

    std::ifstream ifstream(filePath);
    JsonLexer lexer(ifstream);
    JsonParser parser(lexer);
    JsonObject jsonScene = parser.parseValue();

    std::string sceneId = jsonScene.getStringField("id");
    std::unique_ptr<Scene> scene = std::make_unique<Scene>(sceneId);

    std::vector<std::string> ids;
    std::vector<DoLaterComponent> doLaterComponents;

    auto is_in_ids = [&](const std::string& id) {
        for (const auto& i : ids) {
            if (i == id)
                return true;
        }

        return false;
    };

    std::function<void(Object* owner, const JsonObject& component)> componentFrom;

    auto add_to_id_registry = [&](const std::string& id) {
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
                Object* owner = it->owner;
                JsonObject object = it->object;

                it = doLaterComponents.erase(it);

                componentFrom(owner, object);
            }
            else {
                ++it;
            }
        }
    };

    componentFrom = [&](Object* owner, const JsonObject& component) {
        if (JsonArray dependencies = component.getArrayField("dependencies"); !dependencies.empty()) {
            std::vector<std::string> dependenciesToWaitFor;

            for (std::string dependency : dependencies) {
                if (!is_in_ids(dependency)) {
                    dependenciesToWaitFor.push_back(dependency);
                }
            }

            if (!dependenciesToWaitFor.empty()) {
                doLaterComponents.emplace_back(owner, component, dependenciesToWaitFor);
                return;
            }
        }

        const std::string type = component.getStringField("type");
        const std::string id = component.getStringField("id");
        const std::shared_ptr<Component> comp = ComponentRegistry::create(type, owner, component);
        owner->addComponent(comp);
        add_to_id_registry(id);
    };

    for (JsonArray objects = jsonScene.getArrayField("objects"); JsonObject object : objects) {
        const std::string name = object.getStringField("name");
        const int tag = object.getNumberField("tag");
        const std::string objectId = object.getStringField("id");

        auto weakObject = scene->addObject(name, tag, objectId);

        add_to_id_registry(objectId);

        JsonArray components = object.getArrayField("components");

        if (auto sharedObject = weakObject.lock()) {
            for (JsonObject component : components) {
                componentFrom(sharedObject.get(), component);
            }
        }
    }

    return scene;
}
