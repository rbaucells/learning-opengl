#include "Common.h"

Camera::Camera(Vector2 startPos, float startRot) {
    mat4x4_identity(viewMatrix);
    position = Vector3(startPos.x, startPos.y, 1);
    rotation = startRot;
    UpdateViewMatrix();
}

void Camera::SetCameraPosition(Vector2 newPos) {
    position = Vector3(newPos.x, newPos.y, position.z);
    UpdateViewMatrix();
}

void Camera::SetCameraRotation(float newRot) {
    rotation = newRot;
    UpdateViewMatrix();
}


void Camera::MoveCamera(Vector2 change) {
    position += Vector3(change.x, change.y, 0);
    UpdateViewMatrix();
}

void Camera::RotateCamera(float change) {
    rotation += change;
    UpdateViewMatrix();
}


void Camera::UpdateViewMatrix() {
    mat4x4 translationMatrix;
    mat4x4_translate(translationMatrix, -position.x, -position.y, -position.z);

    mat4x4 rotationMatrix;
    mat4x4_identity(rotationMatrix);
    mat4x4_rotate_Z(rotationMatrix, rotationMatrix, rotation);

    mat4x4_mul(viewMatrix, rotationMatrix, translationMatrix);
}



