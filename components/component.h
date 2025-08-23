#pragma once

// forward declaration of Object
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
    virtual void update() {}
    virtual void lateUpdate() {}
    virtual void fixedUpdate() {}
};
