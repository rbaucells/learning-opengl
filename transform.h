#pragma once
#include <vector>
#include "math/mathematics.h"

#include "systems/component.h"

enum Easer : int;
class Object;
class TweenBase;

class Transform {
private:
    // both raw pointers are fine because non owning
    Transform *parent = nullptr;
    std::vector<Transform *> children;
    std::vector<std::unique_ptr<TweenBase>> tweens;

public:
    Transform(Object *owner, Vector2 pos, float rot, Vector2 scale, Transform *parent);
    Transform(Object *owner, Vector2 pos, float rot, Vector2 scale);

    ~Transform();

    void doTweens(float deltaTime);

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

    Vector2 localPosition = {0, 0};
    float localRotation = 0;
    Vector2 localScale = {0, 0};

    // animations
    void tweenLocalPosition(const Vector2 &target, float duration, Easer easeFunction);
    void tweenLocalPosition(const Vector2 &start, const Vector2 &end, float duration, Easer easeFunction);

    void tweenGlobalPosition(const Vector2 &target, float duration, Easer easeFunction);
    void tweenGlobalPosition(const Vector2 &start, const Vector2 &end, float duration, Easer easeFunction);

    void tweenLocalRotation(float target, float duration, Easer easeFunction);
    void tweenLocalRotation(float start, float end, float duration, Easer easeFunction);

    void tweenGlobalRotation(float target, float duration, Easer easeFunction);
    void tweenGlobalRotation(float start, float end, float duration, Easer easeFunction);

    void tweenLocalScale(const Vector2 &target, float duration, Easer easeFunction);
    void tweenLocalScale(const Vector2 &start, const Vector2 &end, float duration, Easer easeFunction);

    void tweenGlobalScale(const Vector2 &target, float duration, Easer easeFunction);
    void tweenGlobalScale(const Vector2 &start, const Vector2 &end, float duration, Easer easeFunction);

    Object *object;
};
