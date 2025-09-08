#pragma once
#include <vector>
#include "vector2.h"

class Curve {
    std::vector<Vector2> points;

    static Vector2 lerpPoint(const Vector2 &start, const Vector2 &end, float t);

public:
    Curve() = default;
    Curve(const Curve &other);
    Curve(std::initializer_list<Vector2> l);
    [[nodiscard]] float evaluate(float t) const;
    std::vector<Vector2> getPoints();
    void addPoint(const Vector2 &point);

    // static curves
    static Curve linear;

    static Curve sineIn;
    static Curve sineOut;
    static Curve sineInOut;

    static Curve circIn;
    static Curve circOut;
    static Curve circInOut;

    static Curve cubicIn;
    static Curve cubicOut;
    static Curve cubicInOut;

    static Curve quartIn;
    static Curve quartOut;
    static Curve quartInOut;

    static Curve expoIn;
    static Curve expoOut;
    static Curve expoInOut;

    static Curve backIn;
    static Curve backOut;
    static Curve backInOut;

    static Curve elasticIn;
    static Curve elasticOut;
    static Curve elasticInOut;

    static Curve bounceIn;
    static Curve bounceOut;
    static Curve bounceInOut;

    static Curve smoothstep;
    static Curve smootherstep;
};