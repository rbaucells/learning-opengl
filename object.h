#pragma once
#include <map>
#include <memory>
#include <vector>
#include "main.h"
#include "transform.h"
#include "systems/component.h"

template<typename T>
concept is_component = std::is_base_of_v<Component, T>;

class Object {
    bool activated = true;
    std::vector<std::shared_ptr<Component>> components;

public:
    const std::string name;
    const int tag;
    bool markedForDeath = false;

    // all objects must have a transform
    Transform transform;

    Object(const std::string& objectName, int objectTag, Vector2 pos, float rot, Vector2 scale);
    Object(const std::string& objectName, int objectTag, Vector2 pos, float rot, Vector2 scale, Transform* parent);

    void update(double deltaTime);
    void fixedUpdate(double fixedDeltaTime) const;
    void lateUpdate(double deltaTime) const;
    void destroy();
    void destroyImmediately();

    template<is_component T, typename... ARGS>
    std::weak_ptr<T> addComponent(ARGS&&... args) {
        // make the component
        std::shared_ptr<T> newComponent = std::make_shared<T>(this, std::forward<ARGS>(args)...);

        // get a weak ptr and add it to components
        std::weak_ptr<T> componentPtr(newComponent);
        componentsToInitialize.push_back(componentPtr);

        components.push_back(newComponent);
        return componentPtr;
    }

    template<is_component T>
    std::weak_ptr<T> getComponent() {
        // loop until
        for (const auto& component : components) {
            // the component raw pointer is of type T
            if (auto comp = std::dynamic_pointer_cast<T>(component)) {
                return std::weak_ptr<T>(comp);
            }
        }
        // nothing was found
        return std::weak_ptr<T> {};
    }

    template<is_component T>
    std::vector<std::weak_ptr<T>> getComponents() {
        // same as getComponent only it adds it to a vector then returns it
        std::vector<std::weak_ptr<T>> foundComponents(components.size());

        for (const auto& component : components) {
            if (auto comp = std::dynamic_pointer_cast<T>(component)) {
                foundComponents.push_back(std::weak_ptr<T>(component));
            }
        }

        return foundComponents;
    }

    template<is_component T>
    void removeComponent() {
        for (auto it = components.begin(); it != components.end(); ++it) {
            if (std::dynamic_pointer_cast<T>(*it)) {
                components.erase(it);
                break;
            }
        }
    }

    template<is_component T>
    void removeAllComponents() {
        for (auto it = components.begin(); it != components.end();) {
            if (std::dynamic_pointer_cast<T>(*it)) {
                it = components.erase(it);
            }
            else {
                ++it;
            }
        }
    }

    static Object* findObjectByName(const std::string& name);
    static Object* findObjectByTag(int tag);
    static std::vector<Object*> findObjectsByName(const std::string& name);
    static std::vector<Object*> findObjectsByTag(int tag);

    void setActive(bool state);
    [[nodiscard]] bool getActive() const;
};

inline std::vector<Object*> allObjects;
inline std::vector<Object*> waitingLineOfDeath;
