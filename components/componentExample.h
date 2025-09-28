#pragma once
#include "../systems/betterEvents.h"
#include "../systems/component.h"

class SpriteSheetRenderer;

class ComponentExample final : public Component {
public:
    using Component::Component;

    void start() override;
    void update(float deltaTime) override;

    void onMouseInput(bool state) const;

private:
    std::weak_ptr<SpriteSheetRenderer> spriteSheetRendererWeakPtr_;
    std::weak_ptr<TweenBase> runningPositionTweenWeakPtr_;

    Subscription<bool> leftClickEventSubscription_;
};
