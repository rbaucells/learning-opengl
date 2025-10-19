#pragma once
#include <vector>

#include "object.h"
#include "transform.h"
#include "math++/math.h"

class Object;

class Scene {
public:
    const std::string id;

    explicit Scene(bool main = true);

    void update(float deltaTime) const;
    void fixedUpdate(float fixedDeltaTime) const;
    void lateUpdate(float lateDeltaTime) const;

    JsonObject serialize() const;

    void manageStarts() const;
    void manageDestructions();

#pragma region Object things
    std::shared_ptr<Object> addObject(const std::string& objectName, int objectTag, Vector2 pos, float rot, Vector2 scale);
    std::shared_ptr<Object> addObject(const std::string& objectName, int objectTag, Vector2 pos, float rot, Vector2 scale, Transform* parent);

    void removeObject(const std::shared_ptr<Object>& objectPtr);
    void removeObjectByTag(int objectTag);
    void removeObjectByName(const std::string& objectName);
    void removeObjectById(const std::string& objectId);
    void removeObjectBy(const std::function<bool(const Object&)>& predicate);

#pragma region Component things

    std::weak_ptr<Component> getComponentById(const std::string& componentid) const;

    template<IsComponent T>
    std::vector<std::weak_ptr<T>> getComponents() const {
        // same as getComponent only it adds it to a vector then returns it
        std::vector<std::weak_ptr<T>> foundComponents;

        for (const auto& object : objects_) {
            for (const auto& component : object->components_) {
                if (auto comp = std::dynamic_pointer_cast<T>(component)) {
                    foundComponents.push_back(std::weak_ptr<T>(component));
                }
            }
        }

        return foundComponents;
    }

    template<IsComponent T>
    // ReSharper disable once CppMemberFunctionMayBeConst
    void removeAllComponents() {
        for (const auto& object : objects_) {
            for (auto& component : object->components_) {
                if (std::dynamic_pointer_cast<T>(component)) {
                    object->componentsToDestroy_.push_back(component);
                }
            }
        }
    }

    void removeComponentBy(const std::function<bool(const Component&)>& predicate);
#pragma endregion
#pragma endregion

    void setMain();

    static Scene* mainScene;

private:
    std::vector<std::shared_ptr<Object>> objects_;
    std::vector<std::shared_ptr<Object>> objectsToDestroy_;
};
