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
    const int tag;

    Transform transform;

    void manageStarts();
    void manageDestructions();

    void update(float deltaTime);
    void fixedUpdate(float fixedDeltaTime) const;
    void lateUpdate(float deltaTime) const;

    void queueDestruction();

#pragma region Component stuff
    template<IsComponent T, typename... ARGS>
    std::weak_ptr<T> addComponent(ARGS&&... args) {
        // make the component
        std::shared_ptr<T> newComponent = std::make_shared<T>(this, std::forward<ARGS>(args)...);

        // get a weak ptr and add it to components
        std::weak_ptr<T> componentPtr(newComponent);

        components_.push_back(newComponent);
        componentsToStart_.push_back(newComponent);

        return componentPtr;
    }

    template<IsComponent T>
    std::weak_ptr<T> getComponent() {
        // loop until
        for (const auto& component : components_) {
            // the component raw pointer is of type T
            if (auto comp = std::dynamic_pointer_cast<T>(component)) {
                return std::weak_ptr<T>(comp);
            }
        }
        // nothing was found
        return std::weak_ptr<T>();
    }

    template<IsComponent T>
    std::vector<std::weak_ptr<T>> getComponents() {
        // same as getComponent only it adds it to a vector then returns it
        std::vector<std::weak_ptr<T>> foundComponents;

        for (const auto& component : components_) {
            if (auto comp = std::dynamic_pointer_cast<T>(component)) {
                foundComponents.push_back(std::weak_ptr<T>(component));
            }
        }

        return foundComponents;
    }

    template<IsComponent T>
    void removeComponent() {
        for (auto& component : components_) {
            if (std::dynamic_pointer_cast<T>(component)) {
                componentsToDestroy_.push_back(component);
                return;
            }
        }
    }

    void removeComponent(const std::shared_ptr<Component>& compPtr);

    void removeComponentBy(const std::function<bool(const Component&)>& predicate);

    template<IsComponent T>
    void removeAllComponents() {
        for (auto& component : components_) {
            if (std::dynamic_pointer_cast<T>(component)) {
                componentsToDestroy_.push_back(component);
            }
        }
    }
#pragma endregion
    void setActive(bool state);
    [[nodiscard]] bool getActive() const;

    // fine to use raw ptr as object is "owned" by scene
    Scene* scene = nullptr;

private:
    bool enabled_ = true;
    std::vector<std::shared_ptr<Component>> components_;

    std::vector<std::shared_ptr<Component>> componentsToDestroy_;
    std::vector<std::shared_ptr<Component>> componentsToStart_;

    Object(Scene* scene, std::string objectName, int objectTag, Vector2 pos, float rot, Vector2 scale);
    Object(Scene* scene, std::string objectName, int objectTag, Vector2 pos, float rot, Vector2 scale, Transform* parent);

    // for access to object constructor and private fields to clean up
    friend class Scene;
};
