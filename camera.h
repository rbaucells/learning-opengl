#pragma once
#include "math/mathematics.h"
#include "systems/component.h"

class Camera : public Component {
public:
    using Component::Component;
    void awake() override;
    void setMain();
    Matrix<4, 4> getViewMatrix();

    static Camera* mainCamera;

private:
    Matrix<4, 4> lastLocalToWorldMatrix;

    Matrix<4, 4> viewMatrix;

    Matrix<4, 4> calculateCameraLocalToWorld() const;
};
