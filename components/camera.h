#pragma once
#include "../systems/component.h"
#include "../math/matrix.h"

class Camera final : public Component {
public:
    using Component::Component;
    void awake() override;
    void setMain();
    Matrix<4, 4> getViewMatrix();

    static Camera* mainCamera;

private:
    Matrix<4, 4> lastLocalToWorldMatrix;

    Matrix<4, 4> viewMatrix;

    [[nodiscard]] Matrix<4, 4> calculateCameraLocalToWorld() const;
};
