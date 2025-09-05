#pragma once
// forward declaration
class Object;

class Component {
public:
    explicit Component(Object* owner) {
        object = owner;
    };

    virtual ~Component() = default;

    // initialization
    virtual void awake() {}
    virtual void onEnable() {}
    virtual void start() {}

    // update loop
    virtual void update(double deltaTime) {}
    virtual void lateUpdate(double deltaTime) {}
    virtual void fixedUpdate(double fixedDeltaTime) {}

    // de-initialization
    virtual void onDisable() {};
    virtual void onDestroy() {};

    // fine to use raw pointer since component will never be alive at a time where the object isnt
    // and because component doesnt own object
    Object* object = nullptr;
};
