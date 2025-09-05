#pragma once
#include "../math/mathematics.h"
#include <vector>

#include "../systems/component.h"

class Object;

class Transform final : public Component{
private:
    Transform *parent = nullptr;
    std::vector<Transform *> children;

public:
    Transform(Object* owner, Vector2 pos, float rot, Vector2 scale, Transform *parent);
    Transform(Object* owner, Vector2 pos, float rot, Vector2 scale);

    ~Transform() override;

    [[nodiscard]] Matrix<4,4> localToWorldMatrix() const;
    [[nodiscard]] Matrix<4,4> worldToLocalMatrix() const;

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

    void setParent(Transform* parent);
    Transform* getParent();

    Vector2 localPosition = {0, 0};
    float localRotation = 0;
    Vector2 localScale = {0, 0};
};
