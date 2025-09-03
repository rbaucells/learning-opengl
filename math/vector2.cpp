#include <cmath>
#include "mathematics.h"

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

bool Vector2::operator==(const Vector2& other) const {
    return x == other.x && y == other.y;
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

float Vector2::Magnitude() const {
    return std::sqrt(x * x + y * y);
}
Vector2 Vector2::Normalized() const {
    float mag = Magnitude();

    if (mag > 0) {
        return Vector2{x / mag, y / mag};
    }

    return *this;
}
Vector2 Vector2::hadamard(const Vector2 &other) const {
    return {x * other.x, y * other.y};
}

float Vector2::Dot(const Vector2 &other) const {
    return (x * other.x + y * other.y);
}
