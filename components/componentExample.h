#pragma once
#include "../systems/events.h"
#include "../systems/component.h"

class SpriteSheetRenderer;

class ComponentExample final : public Component {
public:
    explicit ComponentExample(Object* owner);

    void awake() override;
    void start() override;
    void onEnable() override;

    void update(float deltaTime) override;
    void lateUpdate(float deltaTime) override;
    void fixedUpdate(float fixedDeltaTime) override;

    void onDisable() override;
    void onDestroy() override;

    ~ComponentExample() override;
};
