#include "mathematics.h"
#include <cmath>

Transform::Transform(const vector2 pos, const float rot, const vector2 scale) {
    setGlobalPosition(pos);
    setGlobalRotation(rot);
    setGlobalScale(scale);
}

Transform::Transform(vector2 pos, float rot, vector2 scale, const Transform *parent) {
    this->parent = parent;
    this->localPosition = pos;
    this->localRotation = rot;
    this->localScale = scale;
}

ColumnMatrix4x4 Transform::localTransformationMatrix() const {
    ColumnMatrix4x4 transformationMatrix = ColumnMatrix4x4::identity();

    transformationMatrix.data[0][0] = localScale.x * std::cos(localRotation);
    transformationMatrix.data[1][0] = -localScale.y * std::sin(localRotation);
    transformationMatrix.data[3][0] = localPosition.x;

    transformationMatrix.data[0][1] = localScale.x * std::sin(localRotation);
    transformationMatrix.data[1][1] = localScale.y * std::cos(localRotation);
    transformationMatrix.data[3][1] = localPosition.y;

    return transformationMatrix;
}

ColumnMatrix4x4 Transform::globalTransformationMatrix() const {
    if (parent != nullptr) {
        return parent->globalTransformationMatrix() * localTransformationMatrix();
    }

    return localTransformationMatrix();
}

vector2 Transform::getGlobalPosition() const {
    if (parent != nullptr) {
        ColumnMatrix4x4 matrix = globalTransformationMatrix();

        float x = matrix[3][0];
        float y = matrix[3][1];

        return vector2{x, y};
    }

    return localPosition;
}

vector2 Transform::getGlobalScale() const {
    if (parent != nullptr) {
        ColumnMatrix4x4 matrix = globalTransformationMatrix();

        vector2 xColumn = {matrix[0][0], matrix[0][1]};
        vector2 yColumn = {matrix[1][0], matrix[1][1]};

        const vector2 scale = {xColumn.Magnitude(), yColumn.Magnitude()};

        return scale;
    }

    return localScale;
}

float Transform::getGlobalRotation() const {
    if (parent != nullptr) {
        ColumnMatrix4x4 matrix = globalTransformationMatrix();
        const float rotAngle = std::atan2(matrix[1][0], matrix[1][1]);
        return rotAngle;
    }

    return localRotation;
}

void Transform::setGlobalPosition(const vector2 pos) {
    if (parent) {
        const vector2 parentGlobalPosition;
        const vector2 relativePos = pos - parentGlobalPosition;

        const float parentGlobalRotation = parent->getGlobalRotation();

        vector2 unrotatedPos;
        unrotatedPos.x = relativePos.x * std::cos(-parentGlobalRotation) - relativePos.y * std::sin(parentGlobalRotation);
        unrotatedPos.y = relativePos.x * std::sin(-parentGlobalRotation) + relativePos.y * cos(-parentGlobalRotation);

        vector2 parentGlobalScale = parent->getGlobalScale();
        vector2 newLocalPosition;

        newLocalPosition.x = unrotatedPos.x / parentGlobalScale.x;
        newLocalPosition.y = unrotatedPos.y / parentGlobalScale.y;

        localPosition = newLocalPosition;
    }
    else {
        localPosition = pos;
    }
}

void Transform::setGlobalRotation(const float rot) {
    if (parent) {
        const float parentGlobalRotation = parent->getGlobalRotation();
        const float newGlobalRotation = rot - parentGlobalRotation;
        localRotation = newGlobalRotation;
    }
    else {
        localRotation = rot;
    }
}

void Transform::setGlobalScale(const vector2 scale) {
    if (parent) {
        const vector2 parentGlobalScale = parent->getGlobalScale();

        const float newGlobalScaleX = scale.x / parentGlobalScale.x;
        const float newGlobalScaleY = scale.y / parentGlobalScale.y;

        localScale = {newGlobalScaleX, newGlobalScaleY};
    }
    else {
        localScale = scale;
    }
}
