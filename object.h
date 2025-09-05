#pragma once
#include <map>
#include <memory>
#include <vector>

#include "main.h"
#include "systems/component.h"
#include "transform.h"

template<typename T>
concept IsComponent = std::is_base_of_v<Component, T>;

class Object {
private:
    bool activated = true;
public:
    const std::string name;
    const int tag;
    bool markedForDeath;

    std::vector<Component*> components;

    // all objects must have a transform component
    Transform* transform;

    Object(const std::string &objectName, int objectTag, Vector2 pos, float rot, Vector2 scale);
    Object(const std::string &objectName, int objectTag, Vector2 pos, float rot, Vector2 scale, Transform* parent);

    void update(double deltaTime) const;
    void fixedUpdate(double fixedDeltaTime) const;
    void lateUpdate(double deltaTime) const;
    void destroy();
    void destroyImmediately();

    template<IsComponent T, typename... Args>
    T *addComponent(Args &&... args) {
        T* newComponent = new T(this, std::forward<Args>(args)...);

        componentsToInitialize.push_back(newComponent);
        components.push_back(std::move(newComponent));

        return newComponent;
    }

    template<IsComponent T>
    T *getComponent() {
        // loop until
        for (const auto component: components) {
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
        for (const auto component: components) {
            if (T *specificComp = dynamic_cast<T *>(component)) {
                foundComponents.push_back(specificComp);
            }
        }
        return foundComponents;
    }

    template<IsComponent T>
    const T *getComponentConst() const {
        // loop until
        for (const auto component: components) {
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
        std::vector<const T*> foundComponents;
        for (const auto component: components) {
            if (const T *specificComp = dynamic_cast<const T *>(component)) {
                foundComponents.push_back(specificComp);
            }
        }
        return foundComponents;
    }

    template<IsComponent T>
    void removeComponent() {
        for (int i = 0; i < components.size(); i++) {
            auto component = components.at(i);

            if (const T *specificComp = dynamic_cast<T *>(component)) {
                delete specificComp;
                components.erase(components.begin() + i);
                break;
            }
        }
    }

    template<IsComponent T>
    void removeAllComponents() {
        // std::vector<int> componentsToRemove;
        //
        // for (int i = 0; i < components.size(); i++) {
        //     auto &component = components.at(i);
        //
        //     if (T *specificComp = dynamic_cast<T *>(component)) {
        //         delete specificComp;
        //         componentsToRemove.push_back(i);
        //     }
        // }
        //
        // for (int i = 0; i < componentsToRemove.size(); i++) {
        //     const int index = componentsToRemove.back();
        //
        //     components.erase(components.begin() + index);
        //
        //     componentsToRemove.pop_back();
        // }

        for (auto it = components.begin(); it != components.end(); ) {
            if (const T *specificComp = dynamic_cast<T *>(*it)) {
                delete specificComp;
                it = components.erase(it);
            }
        }
    }

    static Object *findObjectByName(const std::string &name);
    static Object *findObjectByTag(int tag);
    static std::vector<Object *> findObjectsByName(const std::string &name);
    static std::vector<Object *> findObjectsByTag(int tag);

    void setActive(bool state);
    bool getActive() const;
};

inline std::vector<Object *> allObjects;
inline std::vector<Object*> waitingLineOfDeath;
