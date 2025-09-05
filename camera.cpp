#include "camera.h"
#include "object.h"

void Camera::awake() {
    getViewMatrix();
}

void Camera::setMain() {
    mainCamera = this;
}

Matrix<4, 4> Camera::getViewMatrix() {
    auto localToWorldMatrix = calculateCameraLocalToWorld();

    if (lastLocalToWorldMatrix != localToWorldMatrix) {
        lastLocalToWorldMatrix = localToWorldMatrix;
        viewMatrix = localToWorldMatrix;
    }

    return viewMatrix;
}

Camera* Camera::mainCamera = nullptr;
Matrix<4, 4> Camera::calculateCameraLocalToWorld() const {
    auto localToWorldMatrix = Matrix<4,4>::identity<4>();

    localToWorldMatrix = localToWorldMatrix.translate(-object->transform->localPosition.x, -object->transform->localPosition.y, -1);
    localToWorldMatrix = localToWorldMatrix.rotateZ(object->transform->localRotation);
    localToWorldMatrix = localToWorldMatrix.scaleAnisotropic(object->transform->localScale.x, object->transform->localScale.y, 1);

    if (object->transform->getParent() != nullptr) {
        return object->transform->getParent()->localToWorldMatrix().multiply(localToWorldMatrix);
    }

    return localToWorldMatrix;
}
