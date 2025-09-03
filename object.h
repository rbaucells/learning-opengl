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
private:
    bool activated = true;

public:
    const std::string name;
    const int tag;
    bool markedForDestruction = false;

    std::vector<Component *> components;

    // all objects must have a transform
    Transform transform;

    Object(const std::string &name, int objectTag, const Transform &transform);

    void update(double deltaTime) const;

    void fixedUpdate(double fixedDeltaTime) const;

    void lateUpdate(double deltaTime) const;

    void onEnable() const;
    void onDisable() const;

    void onDestroy();

    void markForDestroy();

    void setActive(bool state);
    [[nodiscard]] bool getActive() const;

    template<IsComponent T, typename... Args>
    T *addComponent(Args &&... args) {
        T *newComponent = new T(this, std::forward<Args>(args)...);

        callStartBeforeNextUpdate.push_back(newComponent);
        components.push_back(newComponent);

        return newComponent;
    }

    template<IsComponent T>
    T *getComponent() {
        // loop until
        for (const auto &component: components) {
            // one of them is the correct type
            if (T *specificComp = dynamic_cast<T *>(component)) {
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
            if (T *specificComp = dynamic_cast<T *>(component)) {
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
            if (const T *specificComp = dynamic_cast<const T *>(component)) {
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
            if (const T *specificComp = dynamic_cast<const T *>(component)) {
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
                delete specificComp;
                components.erase(components.begin() + i);
                break;
            }
        }
    }

    template<IsComponent T>
    void removeComponents() {
        std::vector<int> componentsToRemove;

        for (int i = 0; i < components.size(); i++) {
            auto &component = components.at(i);

            if (T *specificComp = dynamic_cast<T *>(component)) {
                delete specificComp;
                componentsToRemove.push_back(i);
            }
        }

        for (int i = 0; i < componentsToRemove.size(); i++) {
            int index = componentsToRemove.back();

            components.erase(components.begin() + index);

            componentsToRemove.pop_back();
        }
    }

    void removeAllComponents();

    static Object *findObjectByName(const std::string &name);

    static Object *findObjectByTag(int tag);

    static std::vector<Object *> findObjectsByName(const std::string &name);

    static std::vector<Object *> findObjectsByTag(int tag);
};

inline std::vector<Object *> markedForDestructionObjects;
inline std::vector<Object *> allObjects;
