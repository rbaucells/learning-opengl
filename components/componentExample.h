#pragma once
#include "../systems/component.h"

class ComponentExample final : public Component {
private:
    float speed_;
public:
    explicit ComponentExample(Object* owner, float speed) : Component(owner), speed_(speed) {};

    void awake() override;
    void start() override;
    void onEnable() override;

    void update(float deltaTime) override;
    void lateUpdate(float deltaTime) override;
    void fixedUpdate(float fixedDeltaTime) override;

    void onDisable() override;
    void onDestroy() override;
};
