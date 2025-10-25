#pragma once
#include "systems/component.h"

class Transform final : public Component {
public:
    explicit Transform(const ComponentParams& params);

    [[nodiscard]] bool hasParent() const;

    void setGlobalPosition(const Vector2& position);
    void setGlobalRotation(float rotation);
    void setGlobalScale(const Vector2& scale);

    [[nodiscard]] Vector2 getGlobalPosition() const;

    /**
     * @brief calculates the global rotation of the transform
     * @return global rotation in radians
     */
    [[nodiscard]] float getGlobalRotation() const;
    [[nodiscard]] Vector2 getGlobalScale() const;

    [[nodiscard]] Matrix<4, 4> localToWorldMatrix() const;
    [[nodiscard]] Matrix<4, 4> worldToLocalMatrix() const;

    Vector2 localPosition = {0, 0};
    float localRotation = 0;
    Vector2 localScale = {0, 0};

private:
    std::weak_ptr<Transform> parent_;
    std::vector<std::weak_ptr<Transform>> children_;
};
