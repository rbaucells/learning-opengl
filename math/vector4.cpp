#include "mathematics.h"
#include <cmath>

vector4 vector4::operator+(const vector4 &other) const {
    return vector4{x + other.x, y + other.y, z + other.z, w + other.w};
}

vector4 vector4::operator-(const vector4 &other) const {
    return vector4{x - other.x, y - other.y, z - other.z, w - other.w};
}

vector4 vector4::operator*(float scalar) const {
    return vector4{x * scalar, y * scalar, z * scalar, w * scalar};
}

vector4 vector4::operator/(float scalar) const {
    if (scalar == 0) {
        // Handle division by zero to prevent crashes
        return vector4{0, 0, 0, 0};
    }
    return vector4{x / scalar, y / scalar, z / scalar, w / scalar};
}

vector4 &vector4::operator+=(const vector4 &other) {
    x += other.x;
    y += other.y;
    z += other.z;
    w += other.w;
    return *this;
}

vector4 &vector4::operator-=(const vector4 &other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    w -= other.w;
    return *this;
}

vector4 &vector4::operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    w *= scalar;
    return *this;
}

vector4 &vector4::operator/=(float scalar) {
    if (scalar != 0) {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        w /= scalar;
    }
    return *this;
}

float vector4::magnitude() const {
    return std::sqrt((x * x) + (y * y) + (z * z) + (w * w));
}

vector4 vector4::normalized() const {
    float mag = magnitude();
    if (mag != 0) {
        return vector4{x / mag, y / mag, z / mag, w / mag};
    }
    return vector4{0, 0, 0, 0};
}

float vector4::dot(const vector4 &other) const {
    return x * other.x + y * other.y + z * other.z + w * other.w;
}
