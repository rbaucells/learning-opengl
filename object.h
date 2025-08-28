#pragma once
#include <map>
#include <memory>
#include "math/mathematics.h"
#include <vector>

#include "main.h"
#include "components/component.h"

template<typename T>
concept IsComponent = std::is_base_of_v<Component, T>;

class Object {
public:
    const std::string name;
    const int tag;
    bool activated = true;
    std::vector<std::unique_ptr<Component> > components;
    Transform transform;

    Object(const std::string &name, int objectTag, const Transform &transform);

    void update(double deltaTime) const;

    void fixedUpdate(double fixedDeltaTime) const;

    void lateUpdate(double deltaTime) const;

    void destroy();

    template<IsComponent T, typename... Args>
    T *AddComponent(Args &&... args) {
        auto newComponent = std::make_unique<T>(this, std::forward<Args>(args)...);

        T *componentPtr = newComponent.get();

        callStartBeforeNextUpdate.push_back(componentPtr);
        components.push_back(std::move(newComponent));

        return componentPtr;
    }

    template<IsComponent T>
    T *GetComponent() {
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
    std::vector<T *> GetComponents() {
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
    const T *GetComponent() const {
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
    std::vector<const T *> GetComponents() const {
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
    void RemoveComponent() {
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
    void RemoveAllComponents() {
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

private:
    Buffers buffers = {};
    std::vector<Vertex> vertices = {};
    std::vector<unsigned int> indices = {};
    unsigned int shaders = 0;
    unsigned int vertexArrayObject = 0;
    unsigned int texture = 0;
};

// inline std::vector<Object*> allObjects;

Buffers definePrimitive(std::vector<Vertex> vertices, std::vector<unsigned int> indices, unsigned int usage);

void drawPrimitive(unsigned int indexBuffer, int indicesCount, unsigned int mode, unsigned int vao, unsigned int texture);

inline std::vector<Object *> allObjects;
