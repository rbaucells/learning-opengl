#pragma once
#include <map>
#include <memory>
#include <vector>

#include "main.h"
#include "components/component.h"
#include "transform.h"

template<typename T>
concept IsComponent = std::is_base_of_v<Component, T>;

class Object {
public:
    const std::string name;
    const int tag;
    bool activated = true;

    std::vector<std::unique_ptr<Component> > components;

    // all objects must have a transform
    Transform transform;

    Object(const std::string &name, int objectTag, const Transform &transform);

    void update(double deltaTime) const;
    void fixedUpdate(double fixedDeltaTime) const;
    void lateUpdate(double deltaTime) const;
    void destroy();

    template<IsComponent T, typename... Args>
    T *addComponent(Args &&... args) {
        auto newComponent = std::make_unique<T>(this, std::forward<Args>(args)...);

        T *componentPtr = newComponent.get();

        callStartBeforeNextUpdate.push_back(componentPtr);
        components.push_back(std::move(newComponent));

        return componentPtr;
    }

    template<IsComponent T>
    T *getComponent() {
        // loop until
        for (const auto &component: components) {
            // one of them is the correct type
            if (T *specificComp = dynamic_cast<T *>(component.get())) {
                return specificComp;
            }
        }
        // nothing was found
        return nullptr;
    }

    template<IsComponent T>
    std::vector<T *> getComponents() {
        // same as GetComponent only it adds it to a vector then returns it
        std::vector<T *> foundComponents;
        for (const auto &component: components) {
            if (T *specificComp = dynamic_cast<T *>(component.get())) {
                foundComponents.push_back(specificComp);
            }
        }
        return foundComponents;
    }

    template<IsComponent T>
    const T *getComponentConst() const {
        // loop until
        for (const auto &component: components) {
            // one of them is the correct type
            if (const T *specificComp = dynamic_cast<const T *>(component.get())) {
                return specificComp;
            }
        }
        // nothing was found
        return nullptr;
    }

    template<IsComponent T>
    std::vector<const T *> getComponentsConst() const {
        // same as GetComponent only it adds it to a vector then returns it
        std::vector<const T *> foundComponents;
        for (const auto &component: components) {
            if (const T *specificComp = dynamic_cast<const T *>(component.get())) {
                foundComponents.push_back(specificComp);
            }
        }
        return foundComponents;
    }

    template<IsComponent T>
    void removeComponent() {
        for (int i = 0; i < components.size(); i++) {
            auto &component = components.at(i);

            if (T *specificComp = dynamic_cast<T *>(component)) {
                component.release();
                components.erase(components.begin() + i);
                break;
            }
        }
    }

    template<IsComponent T>
    void removeAllComponents() {
        std::vector<int> componentsToRemove;

        for (int i = 0; i < components.size(); i++) {
            auto &component = components.at(i);

            if (T *specificComp = dynamic_cast<T *>(component)) {
                component.release();
                componentsToRemove.push_back(i);
            }
        }

        for (int i = 0; i < componentsToRemove.size(); i++) {
            int index = componentsToRemove.back();

            components.erase(components.begin() + index);

            componentsToRemove.pop_back();
        }
    }

    static Object *findObjectByName(const std::string &name);
    static Object *findObjectByTag(int tag);
    static std::vector<Object *> findObjectsByName(const std::string &name);
    static std::vector<Object *> findObjectsByTag(int tag);
};

inline std::vector<Object *> allObjects;
