#pragma once
#include "math/mathematics.h"

class Camera {
public:
    ColumnMatrix4x4 viewMatrix;
    Camera(vector2 startPos, float startRot);
    void SetCameraPosition(vector2 newPos);
    void SetCameraRotation(float newRot);
    void MoveCamera(vector2 change);
    void RotateCamera(float change);
private:
    vector3 position;
    float rotation;

    void UpdateViewMatrix();
};
