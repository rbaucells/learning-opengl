#pragma once
#include <map>
#include <memory>
#include <vector>

#include "nanoId.h"
#include "transform.h"
#include "systems/component.h"

class Scene;

class Object : public std::enable_shared_from_this<Object> {
public:
    const std::string name;
    const std::string id;
    const int tag;

    const std::shared_ptr<Transform> transform;

    void queueDestruction();

    JsonObject serialize() const;

    void setActive(bool state);
    [[nodiscard]] bool getActive() const;

    // fine to use raw ptr as object is "owned" by scene
    Scene* const scene;

    template<IsComponent T, typename... ARGS>
    std::weak_ptr<T> addComponent(ARGS&&... args) {
        static_assert(!std::is_same_v<T, Transform>, "Cannot add component of type 'Transform'. Objects always already have one");

        // make the component
        std::shared_ptr<T> newComponent = std::make_shared<T>({this, NanoId::nanoIdGen()}, std::forward<ARGS>(args)...);

        components_.push_back(newComponent);
        componentsToStart_.push_back(newComponent);

        return newComponent;
    }

    template<IsComponent T>
    std::weak_ptr<T> addComponent(std::shared_ptr<T> component) {
        static_assert(!std::is_same_v<T, Transform>, "Cannot add component of type 'Transform'. Objects always already have one");

        components_.push_back(component);
        componentsToStart_.push_back(component);

        return component;
    }

    std::weak_ptr<Component> getComponentBy(const std::function<bool(const std::shared_ptr<Component>&)>& predicate) const;

    std::vector<std::weak_ptr<Component>> getAllComponentsBy(const std::function<bool(const std::shared_ptr<Component>&)>& predicate) const;

    template<IsComponent T>
    std::weak_ptr<T> getComponent() const {
        for (const auto& component : components_) {
            if (auto castComponent = std::dynamic_pointer_cast<T>(component)) {
                return castComponent;
            }
        }

        return {};
    }

    template<IsComponent T = Component>
    std::weak_ptr<T> getComponentById(const std::string& componentId) const {
        for (const auto& component : components_) {
            if (component->id == componentId) {
                return component;
            }
        }

        return {};
    }

    template<IsComponent T>
    std::vector<std::weak_ptr<T>> getAllComponents() const {
        std::vector<std::weak_ptr<T>> foundComponents;

        for (const auto& component : components_) {
            if (auto castComponent = std::dynamic_pointer_cast<T>(component))
                foundComponents.push_back(castComponent);
        }

        return foundComponents;
    }

    void removeComponentBy(const std::function<bool(const std::shared_ptr<Component>&)>& predicate);

    void removeAllComponentsBy(const std::function<bool(const std::shared_ptr<Component>&)>& predicate);

    template<IsComponent T>
    void removeComponent() {
        static_assert(!std::is_same_v<T, Transform>, "Cannot remove component of type 'Transform'");

        removeComponentBy([](const std::shared_ptr<Component>& component) {
            if (std::dynamic_pointer_cast<T>(component))
                return true;

            return false;
        });
    }

    void removeComponent(const std::shared_ptr<Component>& component);

    template<IsComponent T>
    void removeAllComponents() {
        static_assert(!std::is_same_v<T, Transform>, "Cannot remove component of type 'Transform'");

        removeAllComponentsBy([](const std::shared_ptr<Component>& component) {
            if (std::dynamic_pointer_cast<T>(component))
                return true;

            return false;
        });
    }

    void removeComponentById(const std::string& componentId);

private:
    std::vector<std::shared_ptr<Component>> componentsToStart_;
    std::vector<std::shared_ptr<Component>> components_;
    std::vector<std::shared_ptr<Component>> componentsToDestroy_;

    void manageStarts();
    void manageDestructions();

    void update(float deltaTime);
    void fixedUpdate(float fixedDeltaTime) const;
    void lateUpdate(float deltaTime) const;

    bool enabled_ = true;

    Object(Scene* scene, std::string objectName, int objectTag = 0, const std::string& id = "");

    // for access to object constructor and private fields to clean up
    friend class Scene;
};
