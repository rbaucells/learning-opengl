#pragma once
#include <vector>

#include "math/curve.h"
#include "systems/component.h"
#include "math/matrix.h"
#include "math/vector2.h"
#include "systems/tweens/tween.h"

class Object;

class Transform final : public Component {
    // both raw pointers are fine because non owning
    Transform *parent_ = nullptr;
    std::vector<Transform *> children_;

public:
    Transform(Object *owner, Vector2 pos, float rot, Vector2 scale, Transform *parent);
    Transform(Object *owner, Vector2 pos, float rot, Vector2 scale);

    ~Transform() override;

    [[nodiscard]] Matrix<4, 4> localToWorldMatrix() const;
    [[nodiscard]] Matrix<4, 4> worldToLocalMatrix() const;

    void setGlobalPosition(Vector2 pos);
    void setGlobalRotation(float rot);
    void setGlobalScale(Vector2 scale);

    [[nodiscard]] Vector2 getGlobalPosition() const;
    [[nodiscard]] float getGlobalRotation() const;
    [[nodiscard]] Vector2 getGlobalScale() const;

    void addChild(Transform *child);
    void removeChild(Transform *child);
    void removeAllChildren();
    std::vector<Transform *> getChildren();
    void deleteAllChildren();

    void setParent(Transform *newParent);
    [[nodiscard]] Transform *getParent() const;

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

    JsonObject serialize() const override;

    Vector2 localPosition = {0, 0};
    float localRotation = 0;
    Vector2 localScale = {0, 0};
};
