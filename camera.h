#pragma once
#include "math/mathematics.h"

class Camera {
public:
    Matrix<4, 4> viewMatrix;
    Camera(const Vector2 &startPos, float startRot);
    void setCameraPosition(const Vector2 &newPos);
    void setCameraRotation(float newRot);
    void moveCamera(const Vector2 &change);
    void rotateCamera(float change);

    void setZoom(float value);
    void zoom(float change);

private:
    Vector3 position;
    float rotation;
    float zooooom = 1.0f;
    void updateViewMatrix();
};
