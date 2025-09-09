#pragma once
#include <vector>

#include "workQueue.h"
// forward declaration
class Object;
class TweenBase;

class Component {
protected:
    std::vector<std::unique_ptr<TweenBase>> tweens;
    std::vector<std::unique_ptr<QueueEntry>> queue;
public:
    explicit Component(Object *owner);
    virtual ~Component() = default;

    // initialization
    virtual void awake() {}
    virtual void onEnable() {}
    virtual void start() {}

    // update loop
    virtual void update(double deltaTime) {}
    virtual void lateUpdate(double deltaTime) {}
    virtual void fixedUpdate(double fixedDeltaTime) {}

    void manageTweens(double deltaTime);
    void manageQueue(double deltaTime);

    // de-initialization
    virtual void onDisable() {};
    virtual void onDestroy() {};

    void addTween(std::unique_ptr<TweenBase> tween);

    // fine to use raw pointer since component will never be alive at a time where the object isnt
    // and because component doesnt own object
    Object *object = nullptr;
};
