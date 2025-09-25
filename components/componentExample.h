#pragma once
#include "../systems/component.h"
#include "../systems/input.h"

class SpriteSheetRenderer;

class ComponentExample final : public Component {
public:
    using Component::Component;

    void start() override;
    void update(float deltaTime) override;

    void onMouseInput(bool state) const;

private:
    std::weak_ptr<SpriteSheetRenderer> spriteSheetRendererWeakPtr_;
    TweenBase* runningPositionTweenRawPtr_;
};
