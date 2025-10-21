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

    [[nodiscard]] std::weak_ptr<Object> addObject(const std::string& objectName, int objectTag);

    [[nodiscard]] std::weak_ptr<Object> getObjectBy(const std::function<bool(const std::shared_ptr<Object>&)>& predicate) const;

    [[nodiscard]] std::weak_ptr<Object> getObjectByName(const std::string& objectName) const;
    [[nodiscard]] std::weak_ptr<Object> getObjectById(const std::string& objectId) const;
    [[nodiscard]] std::weak_ptr<Object> getObjectByTag(int objectTag) const;

    void removeObjectBy(const std::function<bool(const std::shared_ptr<Object>&)>& predicate);

    void removeObjectByName(const std::string& objectName);
    void removeObjectById(const std::string& objectId);
    void removeObjectByTag(int objectTag);

    void removeObject(const std::shared_ptr<Object>& object);

    std::weak_ptr<Component> getComponentBy(const std::function<bool(const std::shared_ptr<Component>&)>& predicate) const;
    std::vector<std::weak_ptr<Component>> getAllComponentsBy(const std::function<bool(const std::shared_ptr<Component>&)>& predicate) const;

    template<IsComponent T>
    std::weak_ptr<T> getComponent() const {
        for (const auto& object : objects_) {
            for (const auto& component : object->components_) {
                if (auto castComponent = std::dynamic_pointer_cast<T>(component)) {
                    return castComponent;
                }
            }
        }

        return {};
    }

    template<IsComponent T = Component>
    std::weak_ptr<T> getComponentById(const std::string& componentId) const {
        for (const auto& object : objects_) {
            for (const auto& component : object->components_) {
                if (component->id == componentId) {
                    return component;
                }
            }
        }

        return {};
    }

    template<IsComponent T>
    std::vector<std::weak_ptr<T>> getAllComponents() const {
        std::vector<std::weak_ptr<T>> foundComponents;

        for (const auto& object : objects_) {
            for (const auto& component : object->components_) {
                if (auto castComponent = std::dynamic_pointer_cast<T>(component))
                    foundComponents.push_back(castComponent);
            }
        }

        return foundComponents;
    }

    void removeComponentBy(const std::function<bool(const std::shared_ptr<Component>&)>& predicate);
    void removeAllComponentsBy(const std::function<bool(const std::shared_ptr<Component>&)>& predicate);

    template<IsComponent T>
    void removeComponent() {
        removeComponentBy([](const std::shared_ptr<T>& component) {
            if (std::dynamic_pointer_cast<T>(component))
                return true;

            return false;
        });
    }

    template<IsComponent T>
    void removeAllComponents() {
        removeAllComponentsBy([](const std::shared_ptr<T>& component) {
            if (std::dynamic_pointer_cast<T>(component))
                return true;

            return false;
        });
    }

    void removeComponentById(const std::string& componentId);

    [[nodiscard]] JsonObject serialize() const;

private:
    std::vector<std::shared_ptr<Object>> objects_;
    std::vector<std::shared_ptr<Object>> objectsToDestroy_;
};
