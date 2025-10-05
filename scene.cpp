#include "scene.h"

#include <utility>

#include "object.h"

Scene::Scene(const bool main) {
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
        objectPtr->destroyImmediately();
        std::erase(objects_, objectPtr);
    }

    objectsToDestroy_.clear();
}

std::shared_ptr<Object> Scene::addObject(const std::string& objectName, const int objectTag, const Vector2 pos, const float rot, const Vector2 scale) {
    return objects_.emplace_back(new Object(this, objectName, objectTag, pos, rot, scale));
}

std::shared_ptr<Object> Scene::addObject(const std::string& objectName, const int objectTag, const Vector2 pos, const float rot, const Vector2 scale, Transform* parent) {
    return objects_.emplace_back(new Object(this, objectName, objectTag, pos, rot, scale, parent));
}

void Scene::setMain() {
    mainScene = this;
}

Scene* Scene::mainScene = nullptr;