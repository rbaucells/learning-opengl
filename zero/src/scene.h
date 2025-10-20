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

    JsonObject serialize() const;

    void manageStarts() const;
    void manageDestructions();

#pragma region Object things
    std::shared_ptr<Object> addObject(const std::string& objectName, int objectTag);

    void removeObject(const std::shared_ptr<Object>& objectPtr);
    void removeObjectByTag(int objectTag);
    void removeObjectByName(const std::string& objectName);
    void removeObjectById(const std::string& objectId);
    void removeObjectBy(const std::function<bool(const Object&)>& predicate);

#pragma region Component things
    void removeComponentBy(const std::function<bool(const Component& component)>& predicate);
    std::weak_ptr<Component> getComponentById(const std::string& componentId) const;

    template<IsComponent T>
    std::vector<std::weak_ptr<T>> getComponents() const {
        std::vector<std::weak_ptr<T>> foundComponents;

        for (const auto& object : objects_) {
            foundComponents.push_back(object->getComponents<T>());
        }

        return foundComponents;
    }

    template<IsComponent T>
    // ReSharper disable once CppMemberFunctionMayBeConst
    void removeAllComponents() {
        for (const auto& object : objects_) {
            object->removeAllComponents<T>();
        }
    }
#pragma endregion
#pragma endregion

private:
    std::vector<std::shared_ptr<Object>> objects_;
    std::vector<std::shared_ptr<Object>> objectsToDestroy_;
};
