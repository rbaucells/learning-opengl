#pragma once
#include <vector>
#include "workQueue.h"

// forward declaration
class TweenBase;
class Object;

class Component {
protected:
    std::vector<std::unique_ptr<QueueEntry>> queue;
    std::vector<std::unique_ptr<TweenBase>> tweens;

public:
    explicit Component(Object* owner);
    virtual ~Component() = default;

    // initialization
    virtual void awake() {}
    virtual void onEnable() {}
    virtual void start() {}

    // update loop
    virtual void update(double deltaTime) {}
    virtual void lateUpdate(double deltaTime) {}
    virtual void fixedUpdate(double fixedDeltaTime) {}

    void manageQueue(double deltaTime);
    void manageTweens(double deltaTime);

    // de-initialization
    virtual void onDisable() {}
    virtual void onDestroy() {}

    void registerTween(std::unique_ptr<TweenBase> tween);

    // fine to use raw pointer since component will never be alive at a time where the object isnt
    // and because component doesnt own object
    Object* object = nullptr;
};
