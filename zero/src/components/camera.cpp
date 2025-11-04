#include "camera.h"
#include "../src/object.h"

Camera::AspectRatio Camera::AspectRatio::sixteenByNine = {16, 9};
Camera::AspectRatio Camera::AspectRatio::fourByThree = {4, 3};
Camera::AspectRatio Camera::AspectRatio::twentyOneByNine = {21, 9};
Camera::AspectRatio Camera::AspectRatio::oneByOne = {1, 1};

Camera::Camera(const ComponentParams& params, const bool main) : Component(params) {
    if (main)
        setMain();
}

void Camera::awake() {
    getViewMatrix();
}

void Camera::setMain() {
    mainCamera = this;
}

Matrix<4, 4> Camera::getViewMatrix() {
    if (const auto localToWorldMatrix = calculateCameraLocalToWorld(); lastLocalToWorldMatrix_ != localToWorldMatrix) {
        lastLocalToWorldMatrix_ = localToWorldMatrix;
        viewMatrix_ = localToWorldMatrix;
    }

    return lastLocalToWorldMatrix_;
}

Camera* Camera::mainCamera = nullptr;

Camera::~Camera() {
    if (mainCamera == this) {
        mainCamera = nullptr;
    }
}

Matrix<4, 4> Camera::calculateCameraLocalToWorld() const {
    auto localToWorldMatrix = Matrix<4, 4>::identity();

    localToWorldMatrix = localToWorldMatrix.translate(-object->transform->localPosition[0], -object->transform->localPosition[1], -1);
    localToWorldMatrix = localToWorldMatrix.rotateZ(object->transform->localRotation);
    localToWorldMatrix = localToWorldMatrix.scaleAnisotropic(object->transform->localScale[0], object->transform->localScale[1], 1);

    Transform* parent = object->transform->getParent();

    if (parent != nullptr) {
        return parent->localToWorldMatrix().multiply(localToWorldMatrix);
    }

    return localToWorldMatrix;
}
