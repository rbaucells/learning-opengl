#pragma once
#include <vector>

#include "math/mathematics.h"

class Transform {
private:
    Transform *parent = nullptr;
    std::vector<Transform*> children;

public:
    Transform(Vector2 pos, float rot, Vector2 scale, Transform *parent);

    Transform(Vector2 pos, float rot, Vector2 scale);

    [[nodiscard]] ColumnMatrix4X4 localToWorldMatrix() const;
    [[nodiscard]] ColumnMatrix4X4 worldToLocalMatrix() const;

    void setGlobalPosition(Vector2 pos);

    void setGlobalRotation(float rot);

    void setGlobalScale(Vector2 scale);

    [[nodiscard]] Vector2 getGlobalPosition() const;

    [[nodiscard]] float getGlobalRotation() const;

    [[nodiscard]] Vector2 getGlobalScale() const;

    void setParent(Transform* parent);
    [[nodiscard]] const Transform* getParent() const;
    void unParent();

    void addChild(Transform* child);
    std::vector<Transform*> getChildren();
    void removeChild(Transform* child);
    void removeAllChildren();

    Vector2 localPosition{};
    float localRotation{};
    Vector2 localScale{};
};
