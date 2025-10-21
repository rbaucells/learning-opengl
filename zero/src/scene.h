#pragma once
#include <vector>

#include "object.h"
#include "math++/math.h"

class Object;

class Scene {
public:
    const std::string id;

    Scene();

    void update(float deltaTime) const;
    void fixedUpdate(float fixedDeltaTime) const;
    void lateUpdate(float lateDeltaTime) const;

    void manageStarts() const;
    void manageDestructions();

#pragma region Object Stuff
    [[nodiscard]] std::weak_ptr<Object> addObject(const std::string& objectName, int objectTag);

    [[nodiscard]] std::weak_ptr<Object> getObjectBy(const std::function<bool(const std::shared_ptr<Object>&)>& predicate) const;
    [[nodiscard]] std::weak_ptr<Object> getObjectByName(const std::string& objectName) const;
    [[nodiscard]] std::weak_ptr<Object> getObjectById(const std::string& objectId) const;
    [[nodiscard]] std::weak_ptr<Object> getObjectByTag(int objectTag) const;

    void removeObjectBy(const std::function<bool(const std::shared_ptr<Object>&)>& predicate);

    void removeObjectByName(const std::string& objectName);
    void removeObjectById(const std::string& objectId);
    void removeObjectByTag(int objectTag);
#pragma endregion
#pragma region Component Stuff
    template<IsComponent T>
    std::vector<std::weak_ptr<T>> getAllComponents() {
        std::vector<std::weak_ptr<T>> foundComponents;

        for (const auto& object : objects_) {
            foundComponents.push_back(object->getAllComponents<T>());
        }

        return foundComponents;
    }

    [[nodiscard]] std::weak_ptr<Component> getComponentBy(const std::function<bool(const std::shared_ptr<Component>&)>& predicate) const;

    template<IsComponent T = Component>
    std::weak_ptr<T> getComponentById(const std::string& componentId) {
        return std::dynamic_pointer_cast<T>(getComponentBy([componentId](const std::shared_ptr<Component>& component) {
            return component->id == componentId;
        }));
    }

    template<IsComponent T>
    std::weak_ptr<T> removeAllComponents() {
        for (const auto& object : objects_) {
            object->removeAllComponents<T>();
        }
    }

    void removeComponentBy(const std::function<bool(const std::shared_ptr<Component>&)>& predicate) const;

    void removeComponentById(const std::string& componentId);
#pragma endregion

    template<IsComponent T>
    void removeAllComponents() {
        for (const auto& object : objects_) {
            object->removeAllComponents<T>();
        }
    }

    [[nodiscard]] JsonObject serialize() const;

private:
    std::vector<std::shared_ptr<Object>> objects_;
    std::vector<std::shared_ptr<Object>> objectsToDestroy_;
};
