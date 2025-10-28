#pragma once
#include "systems/component.h"

class Transform final : public Component {
public:
    enum class RotationType {
        radians,
        degrees
    };

    explicit Transform(const ComponentParams& params);

    [[nodiscard]] bool hasParent() const;

    void setGlobalPosition(const Vector2& position);
    void setGlobalRotation(float rotation);
    void setGlobalScale(const Vector2& scale);

    [[nodiscard]] Vector2 getGlobalPosition() const;
    [[nodiscard]] float getGlobalRotation(RotationType type = RotationType::degrees) const;
    [[nodiscard]] Vector2 getGlobalScale() const;

    [[nodiscard]] Matrix<4, 4> localToWorldMatrix() const;
    [[nodiscard]] Matrix<4, 4> worldToLocalMatrix() const;

    Vector2 localPosition = {0, 0};

    // in radians
    float localRotation = 0;
    Vector2 localScale = {0, 0};

private:
    std::weak_ptr<Transform> parent_;
    std::vector<std::weak_ptr<Transform>> children_;
};
