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
    Transform *parent = nullptr;
    std::vector<Transform *> children;

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

    std::unique_ptr<TweenBase> localPosTween(Vector2 target, double duration, const Curve& curve);
    std::unique_ptr<TweenBase> localPosTween(Vector2 start, Vector2 end, double duration, const Curve& curve);

    std::unique_ptr<TweenBase> localRotationTween(float target, double duration, const Curve& curve);
    std::unique_ptr<TweenBase> localRotationTween(float start, float end, double duration, const Curve& curve);

    std::unique_ptr<TweenBase> localScaleTween(Vector2 target, double duration, const Curve& curve);
    std::unique_ptr<TweenBase> localScaleTween(Vector2 start, Vector2 end, double duration, const Curve& curve);

    Vector2 localPosition = {0, 0};
    float localRotation = 0;
    Vector2 localScale = {0, 0};
};
