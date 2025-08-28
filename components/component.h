#pragma once
#include <vector>
#include "../input.h"
#include "../event.h"

// forward declaration
class Object;
class Component {
protected:
    Object* object = nullptr;
public:
    Component(Object* owner) {
        object = owner;
    };

    virtual ~Component() = default;

    virtual void start() {}
    virtual void update(double deltaTime) {}
    virtual void lateUpdate(double deltaTime) {}
    virtual void fixedUpdate(double fixedDeltaTime) {}
};
