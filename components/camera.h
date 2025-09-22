#pragma once
#include "../systems/component.h"
#include "../math/matrix.h"

class Camera final : public Component {
public:
    struct AspectRatio {
        float width, height;
    };

    using Component::Component;
    void awake() override;
    void setMain();
    Matrix<4, 4> getViewMatrix();

    float getVerticalWidth() const {
        return verticalWidth_;
    }

    float verticalSize = 5;
    AspectRatio aspectRatio;

    static Camera* mainCamera;

private:
    Matrix<4, 4> lastLocalToWorldMatrix_;

    Matrix<4, 4> viewMatrix_;

    [[nodiscard]] Matrix<4, 4> calculateCameraLocalToWorld() const;

    float verticalWidth_;
};
