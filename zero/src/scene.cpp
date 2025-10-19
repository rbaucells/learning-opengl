#include "scene.h"

#include "nanoId.h"
#include "object.h"
#include "json++/json.h"

Scene::Scene(const bool main) : id(NanoId::nanoIdGen()) {
    if (main)
        setMain();
}

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

    for (auto& objectPtr : objectsToDestroy_) {
        // we need to deactivate before destroying
        if (objectPtr->enabled_) {
            objectPtr->transform.onDisable();
        }

        for (const auto& comp : objectPtr->components_) {
            if (objectPtr->enabled_) {
                comp->onDisable();
            }

            comp->onDestroy();
        }

        objectPtr->transform.onDestroy();

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

std::shared_ptr<Object> Scene::addObject(const std::string& objectName, const int objectTag, const Vector2 pos, const float rot, const Vector2 scale) {
    return objects_.emplace_back(new Object(this, objectName, objectTag, pos, rot, scale));
}

std::shared_ptr<Object> Scene::addObject(const std::string& objectName, const int objectTag, const Vector2 pos, const float rot, const Vector2 scale, Transform* parent) {
    return objects_.emplace_back(new Object(this, objectName, objectTag, pos, rot, scale, parent));
}

void Scene::removeObject(const std::shared_ptr<Object>& objectPtr) {
    // if its not alr in there
    if (std::ranges::find(objectsToDestroy_, objectPtr) != objectsToDestroy_.end()) {
        return;
    }

    objectsToDestroy_.push_back(objectPtr);
}

void Scene::removeObjectByTag(int objectTag) {
    removeObjectBy([objectTag](const Object& obj) {
        return obj.tag == objectTag;
    });
}

void Scene::removeObjectByName(const std::string& objectName) {
    removeObjectBy([objectName](const Object& obj) {
        return obj.name == objectName;
    });
}

void Scene::removeObjectById(const std::string& objectId) {
    removeObjectBy([objectId](const Object& obj) {
        return obj.id == objectId;
    });
}

void Scene::removeObjectBy(const std::function<bool(const Object&)>& predicate) {
    for (auto& obj : objects_) {
        if (predicate(*obj)) {
            removeObject(obj);
        }
    }
}

void Scene::setMain() {
    mainScene = this;
}

std::weak_ptr<Component> Scene::getComponentById(const std::string& componentid) const {
    for (const auto& object : objects_) {
        for (const auto& component : object->components_) {
            if (component->id == componentid) {
                return component;
            }
        }
    }

    return {};
}

void Scene::removeComponentBy(const std::function<bool(const Component&)>& predicate) {
    for (const auto& object : objects_) {
        for (const auto& component : object->components_) {
            if (predicate(*component)) {
                object->componentsToDestroy_.push_back(component);
            }
        }
    }
}

JsonObject Scene::serialize() const {
    JsonObject jsonResult;

    jsonResult.putStringField("id", id);

    JsonArray jsonObjects;

    for (const auto& object: objects_) {
        jsonObjects.putObject(object->serialize());
    }

    jsonResult.putArrayField("objects", jsonObjects);

    return jsonResult;
}


Scene* Scene::mainScene;
