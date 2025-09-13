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
    virtual void update(float deltaTime) {}
    virtual void lateUpdate(float deltaTime) {}
    virtual void fixedUpdate(float fixedDeltaTime) {}

    void manageQueue(float deltaTime);
    void manageTweens(float deltaTime);

    // de-initialization
    virtual void onDisable() {}
    virtual void onDestroy() {}

    template<typename TWEEN_TYPE>
    TWEEN_TYPE* addTween(std::unique_ptr<TWEEN_TYPE> tween) {
        TWEEN_TYPE* rawPtr = tween.get();
        tweens.push_back(std::move(tween));
        return rawPtr;
    }

    // fine to use raw pointer since component will never be alive at a time where the object isnt
    // and because component doesnt own object
    Object* object = nullptr;
};
