#pragma once

struct Vector2 {
    float x = {};
    float y = {};

    Vector2 operator+(const Vector2& other) const;
    Vector2 operator-(const Vector2& other) const;
    Vector2 operator*(float scalar) const;
    Vector2 operator/(float scalar) const;
    Vector2& operator+=(const Vector2& other);
    Vector2& operator-=(const Vector2& other);
    Vector2& operator*=(float scalar);
    Vector2& operator/=(float scalar);
};

struct Color {
    float r, g, b, a;
};

struct Vertex {
    Vector2 position;
    Color color;
    Vector2 uv;
};