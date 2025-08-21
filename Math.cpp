#include "Math.h"

// Vector 2
Vector2 Vector2::operator+(const Vector2 &other) const {
    return Vector2{x + other.x, y + other.y};
}
Vector2 Vector2::operator-(const Vector2 &other) const {
    return Vector2{x - other.x, y - other.y};
}
Vector2 Vector2::operator*(float scalar) const {
    return Vector2{x * scalar, y * scalar};
}
Vector2 Vector2::operator/(float scalar) const {
    return Vector2{x / scalar, y / scalar};
}


Vector2& Vector2::operator+=(const Vector2 &other) {
    x += other.x;
    y += other.y;
    return *this;
}
Vector2& Vector2::operator-=(const Vector2 &other) {
    x -= other.x;
    y -= other.y;
    return *this;
}
Vector2& Vector2::operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
}
Vector2& Vector2::operator/=(float scalar) {
    x /= scalar;
    y /= scalar;
    return *this;
}

// Vector 3
Vector3 Vector3::operator+(const Vector3 &other) const {
    return Vector3{x + other.x, y + other.y, z + other.z};
}
Vector3 Vector3::operator-(const Vector3 &other) const {
    return Vector3{x - other.x, y - other.y, z - other.z};
}
Vector3 Vector3::operator*(float scalar) const {
    return Vector3{x * scalar, y * scalar, z * scalar};
}
Vector3 Vector3::operator/(float scalar) const {
    return Vector3{x / scalar, y / scalar, z / scalar};
}

Vector3& Vector3::operator+=(const Vector3 &other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}
Vector3& Vector3::operator-=(const Vector3 &other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
}
Vector3& Vector3::operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}
Vector3& Vector3::operator/=(float scalar) {
    x /= scalar;
    y /= scalar;
    y /= scalar;
    return *this;
}