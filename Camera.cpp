#include "camera.h"

Camera::Camera(vector2 startPos, float startRot) {
    viewMatrix = viewMatrix.identity();
    position = vector3(startPos.x, startPos.y, 1);
    rotation = startRot;
    UpdateViewMatrix();
}

void Camera::SetCameraPosition(vector2 newPos) {
    position = vector3(newPos.x, newPos.y, position.z);
    UpdateViewMatrix();
}

void Camera::SetCameraRotation(float newRot) {
    rotation = newRot;
    UpdateViewMatrix();
}

void Camera::MoveCamera(vector2 change) {
    position += vector3(change.x, change.y, 0);
    UpdateViewMatrix();
}

void Camera::RotateCamera(float change) {
    rotation += change;
    UpdateViewMatrix();
}

void Camera::UpdateViewMatrix() {
    ColumnMatrix4x4 translationMatrix;
    translationMatrix = translationMatrix.translate(-position.x, -position.y, -position.z);

    ColumnMatrix4x4 rotationMatrix = ColumnMatrix4x4::identity();
    rotationMatrix = rotationMatrix.rotate_z(rotation);

    viewMatrix = rotationMatrix * translationMatrix;
}



