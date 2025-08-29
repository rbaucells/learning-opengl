#include <cmath>
#include "mathematics.h"

vector2 vector2::operator+(const vector2 &other) const {
    return vector2{x + other.x, y + other.y};
}
vector2 vector2::operator-(const vector2 &other) const {
    return vector2{x - other.x, y - other.y};
}
vector2 vector2::operator*(float scalar) const {
    return vector2{x * scalar, y * scalar};
}
vector2 vector2::operator/(float scalar) const {
    return vector2{x / scalar, y / scalar};
}

bool vector2::operator==(const vector2& other) const {
    return x == other.x && y == other.y;
}



vector2& vector2::operator+=(const vector2 &other) {
    x += other.x;
    y += other.y;
    return *this;
}
vector2& vector2::operator-=(const vector2 &other) {
    x -= other.x;
    y -= other.y;
    return *this;
}
vector2& vector2::operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
}
vector2& vector2::operator/=(float scalar) {
    x /= scalar;
    y /= scalar;
    return *this;
}

float vector2::Magnitude() const {
    return std::sqrt(x * x + y * y);
}
vector2 vector2::Normalized() const {
    float mag = Magnitude();

    if (mag > 0) {
        return vector2{x / mag, y / mag};
    }

    return *this;
}
vector2 vector2::hadamard(const vector2 &other) const {
    return {x * other.x, y * other.y};
}

float vector2::Dot(const vector2 &other) const {
    return (x * other.x + y * other.y);
}
