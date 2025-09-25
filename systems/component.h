#pragma once
#include <vector>
#include "workQueue.h"
#include "tweens/tween.h"

template<typename TWEEN_TYPE>
concept derives_from_tween_base = std::is_base_of_v<TweenBase, TWEEN_TYPE>;

// forward declaration
class Object;

class Component {
protected:
    std::vector<std::unique_ptr<QueueEntry>> queue_;
    std::vector<std::shared_ptr<TweenBase>> tweens_;

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

    template<derives_from_tween_base TWEEN_TYPE>
    std::weak_ptr<TWEEN_TYPE> addTween(std::shared_ptr<TWEEN_TYPE>& tween) {
        tween->start();
        tweens_.push_back(tween);
        return tween;
    }

    // fine to use raw pointer since component will never be alive at a time where the object isnt
    // and because component doesnt own object
    Object* object = nullptr;
};

inline std::vector<std::weak_ptr<Component>> componentsToInitialize {};
