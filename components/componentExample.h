#pragma once
#include "../serializationAndDeserialization/deserializer.h"
#include "../systems/events.h"
#include "../systems/component.h"

class JsonObject;
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

    void onLeftClick(bool state) const;

    ~ComponentExample() override;

    JsonObject serialize() const override;

    static std::shared_ptr<Component> deserialize(Object* owner, const JsonObject& data);
private:
    Subscription<bool> leftClickSubscription_;

    float f;
};
