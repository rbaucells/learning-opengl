#include "Math.h"
#include <cmath>

vector3 vector3::operator+(const vector3 &other) const {
    return vector3{x + other.x, y + other.y, z + other.z};
}
vector3 vector3::operator-(const vector3 &other) const {
    return vector3{x - other.x, y - other.y, z - other.z};
}
vector3 vector3::operator*(float scalar) const {
    return vector3{x * scalar, y * scalar, z * scalar};
}
vector3 vector3::operator/(float scalar) const {
    return vector3{x / scalar, y / scalar, z / scalar};
}

vector3& vector3::operator+=(const vector3 &other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}
vector3& vector3::operator-=(const vector3 &other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
}
vector3& vector3::operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}
vector3& vector3::operator/=(float scalar) {
    x /= scalar;
    y /= scalar;
    y /= scalar;
    return *this;
}

float vector3::magnitude() const {
    return std::sqrt((x * x) + (y * y) + (z * z));
}
vector3 vector3::normalized() const {
    float mag = magnitude();

    if (mag > 0) {
        return vector3{x / mag, y / mag, z / mag};
    }

    return *this;
}
float vector3::dot(const vector3 &other) const {
    return (x * other.x + y * other.y + z * other.z);
}
vector3 vector3::cross(const vector3 &other) const {
    return {
        y * other.z - z * other.y,
        z * other.x - x * other.z,
        x * other.y - y * other.x
    };
}
