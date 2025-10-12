#pragma once
#include <vector>
#include "workQueue.h"
#include "tweens/tween.h"

template<typename T>
concept DerivesFromTweenBase = std::is_base_of_v<TweenBase, T>;

template<typename T>
concept IsComponent = std::is_base_of_v<Component, T>;

// forward declaration
class Object;

class Component : public std::enable_shared_from_this<Component> {
public:
    explicit Component(Object* owner);
    // initialization
    virtual void awake() {}
    virtual void onEnable() {}
    virtual void start() {}


    // update loop
    virtual void update(float deltaTime) {}
    virtual void lateUpdate(float deltaTime) {}
    virtual void fixedUpdate(float fixedDeltaTime) {}

    void manageQueue(float deltaTime);
    void manageTweens(float deltaTime);

    // de-initialization
    virtual void onDisable() {}
    virtual void onDestroy() {}

    virtual ~Component() = default;

    template<DerivesFromTweenBase T>
    std::weak_ptr<T> addTween(std::shared_ptr<T> tween) {
        tweens_.push_back(tween);
        return tween;
    }

    void queueDestruction();

    // fine to use raw ptr as component is "owned" by object
    Object* object = nullptr;

protected:
    std::vector<std::unique_ptr<QueueEntry>> queue_;
    std::vector<std::shared_ptr<TweenBase>> tweens_;
};
