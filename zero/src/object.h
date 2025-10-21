#pragma once
#include <map>
#include <memory>
#include <vector>
#include "transform.h"
#include "systems/component.h"

class Scene;

class Object : public std::enable_shared_from_this<Object> {
public:
    const std::string name;
    const std::string id;
    const int tag;

    std::shared_ptr<Transform> transform;

    void manageStarts();
    void manageDestructions();

    void update(float deltaTime);
    void fixedUpdate(float fixedDeltaTime) const;
    void lateUpdate(float deltaTime) const;

    void queueDestruction();

    JsonObject serialize() const;

#pragma region Component stuff
    template<IsComponent T, typename... ARGS>
    std::weak_ptr<T> addComponent(ARGS&&... args) {
        // make the component
        std::shared_ptr<T> newComponent = std::make_shared<T>(this, std::forward<ARGS>(args)...);

        components_.push_back(newComponent);
        componentsToStart_.push_back(newComponent);

        return newComponent;
    }

    template<IsComponent T>
    std::weak_ptr<T> addComponent(std::shared_ptr<T> component) {
        components_.push_back(component);
        componentsToStart_.push_back(component);

        return component;
    }

    void removeComponentsBy(const std::function<bool(const std::shared_ptr<Component>&)>& predicate);

    template<IsComponent T>
    void removeAllComponents() {
        removeComponentsBy([](const std::shared_ptr<Component>& component) {
            return std::dynamic_pointer_cast<T>(component);
        });
    }

    template<IsComponent T>
    void removeComponent() {
        for (const auto& component: components_) {
            if (std::dynamic_pointer_cast<T>(component)) {
                componentsToDestroy_.push_back(component);
            }
        }
    }

    template<IsComponent T>
    std::vector<std::weak_ptr<T>>> getAllComponents() {

    }

#pragma endregion

    void setActive(bool state);
    [[nodiscard]] bool getActive() const;

    // fine to use raw ptr as object is "owned" by scene
    Scene* const scene;

private:
    std::vector<std::shared_ptr<Component>> componentsToStart_;
    std::vector<std::shared_ptr<Component>> components_;
    std::vector<std::shared_ptr<Component>> componentsToDestroy_;

    bool enabled_ = true;

    Object(Scene* scene, std::string objectName, int objectTag);

    // for access to object constructor and private fields to clean up
    friend class Scene;
};
