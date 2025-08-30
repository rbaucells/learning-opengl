#pragma once
#include <vector>

#include "math/mathematics.h"

class Transform {
private:
    Transform *parent = nullptr;
    std::vector<Transform*> children;

public:
    Transform(vector2 pos, float rot, vector2 scale, Transform *parent);

    Transform(vector2 pos, float rot, vector2 scale);

    [[nodiscard]] ColumnMatrix4x4 localToWorldMatrix() const;
    [[nodiscard]] ColumnMatrix4x4 worldToLocalMatrix() const;

    void setGlobalPosition(vector2 pos);

    void setGlobalRotation(float rot);

    void setGlobalScale(vector2 scale);

    [[nodiscard]] vector2 getGlobalPosition() const;

    [[nodiscard]] float getGlobalRotation() const;

    [[nodiscard]] vector2 getGlobalScale() const;

    void setParent(Transform* parent);
    [[nodiscard]] const Transform* getParent() const;

    void addChild(Transform* child);
    std::vector<Transform*> getChildren();
    void removeChild(Transform* child);
    void removeAllChildren();

    vector2 localPosition{};
    float localRotation{};
    vector2 localScale{};
};
