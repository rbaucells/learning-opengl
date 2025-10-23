#pragma once
#include <vector>

#include "json++/json.h"
#include "math++/math.h"
#include "systems/component.h"
#include "systems/tweens/tween.h"

class Object;

class Transform final : public Component {
public:
    explicit Transform(const ComponentParams& params, Vector2 pos = {0, 0}, float rot = 0, Vector2 scale = {0, 0}, Transform* parent = nullptr);

    [[nodiscard]] Matrix<4, 4> localToWorldMatrix() const;
    [[nodiscard]] Matrix<4, 4> worldToLocalMatrix() const;

    void setGlobalPosition(Vector2 pos);
    void setGlobalRotation(float rot);
    void setGlobalScale(Vector2 scale);

    [[nodiscard]] Vector2 getGlobalPosition() const;
    [[nodiscard]] float getGlobalRotation() const;
    [[nodiscard]] Vector2 getGlobalScale() const;

    void addChild(Transform* child);

    void removeChild(Transform* child);
    void removeAllChildren();
    void deleteAllChildren();

    std::vector<Transform*> getChildren();

    void setParent(Transform* newParent);
    [[nodiscard]] Transform* getParent() const;

#pragma region tweens
    std::weak_ptr<Tween<Vector2>> localPosTween(Vector2 target, float duration, const Curve& curve);
    std::weak_ptr<Tween<Vector2>> localPosTween(Vector2 start, Vector2 end, float duration, const Curve& curve);

    std::weak_ptr<Tween<float>> localRotationTween(float target, float duration, const Curve& curve);
    std::weak_ptr<Tween<float>> localRotationTween(float start, float end, float duration, const Curve& curve);

    std::weak_ptr<Tween<Vector2>> localScaleTween(Vector2 target, float duration, const Curve& curve);
    std::weak_ptr<Tween<Vector2>> localScaleTween(Vector2 start, Vector2 end, float duration, const Curve& curve);

    std::weak_ptr<FunctionalTween<Vector2>> globalPosTween(Vector2 target, float duration, const Curve& curve);
    std::weak_ptr<FunctionalTween<Vector2>> globalPosTween(Vector2 start, Vector2 end, float duration, const Curve& curve);

    std::weak_ptr<FunctionalTween<float>> globalRotationTween(float target, float duration, const Curve& curve);
    std::weak_ptr<FunctionalTween<float>> globalRotationTween(float start, float end, float duration, const Curve& curve);

    std::weak_ptr<FunctionalTween<Vector2>> globalScaleTween(Vector2 target, float duration, const Curve& curve);
    std::weak_ptr<FunctionalTween<Vector2>> globalScaleTween(Vector2 start, Vector2 end, float duration, const Curve& curve);
#pragma endregion
    JsonObject serialize() const override;

    ~Transform() override;

    Vector2 localPosition = {0, 0};
    float localRotation = 0;
    Vector2 localScale = {0, 0};

    static std::shared_ptr<Component> deserialize(Object* owner, const JsonObject& jsonTransform);

private:
    // both raw pointers are fine because non owning
    Transform* parent_ = nullptr;
    std::vector<Transform*> children_;
};
