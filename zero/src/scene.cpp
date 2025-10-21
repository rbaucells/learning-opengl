#include "scene.h"

#include "nanoId.h"
#include "object.h"
#include "json++/json.h"

Scene::Scene() : id(NanoId::nanoIdGen()) {}

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

std::weak_ptr<Object> Scene::addObject(const std::string& objectName, const int objectTag) {
    return objects_.emplace_back(new Object(this, objectName, objectTag));
}

std::weak_ptr<Object> Scene::getObjectBy(const std::function<bool(const std::shared_ptr<Object>&)>& predicate) const {
    for (const auto& objectPtr : objects_) {
        if (predicate(objectPtr))
            return objectPtr;
    }

    return {};
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

std::weak_ptr<Object> Scene::getObjectByName(const std::string& objectName) const {
    return getObjectBy([objectName](const std::shared_ptr<Object>& object) {
        return object->name == objectName;
    });
}

void Scene::removeObjectBy(const std::function<bool(const std::shared_ptr<Object>&)>& predicate) {
    for (const auto& objectPtr : objects_) {
        if (predicate(objectPtr))
            objectsToDestroy_.push_back(objectPtr);
    }
}

void Scene::removeObjectByTag(int objectTag) {
    removeObjectBy([objectTag](const std::shared_ptr<Object>& obj) {
        return obj->tag == objectTag;
    });
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

void Scene::removeComponentById(const std::string& componentId) {
    removeComponentBy([componentId](const std::shared_ptr<Component>& component) {
        return component->id == componentId;
    });
}