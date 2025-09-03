#pragma once
#include <vector>

// forward declaration
class Object;

class Component {
protected:
    Object* object = nullptr;
public:
    explicit Component(Object* owner) {
        object = owner;
    };

    virtual ~Component() = default;

    virtual void awake() {}
    virtual void onEnable() {}
    virtual void start() {} // called once when the object is added
    virtual void update(double deltaTime) {} // once a frame
    virtual void lateUpdate(double deltaTime) {} // once a frame after update
    virtual void fixedUpdate(double fixedDeltaTime) {} // called a certain amount of times per second
    virtual void onDisable() {}
    virtual void onDestroy() {}
};

inline std::vector<Component*> callStartBeforeNextUpdate {};
