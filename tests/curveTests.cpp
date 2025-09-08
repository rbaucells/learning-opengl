#include <iostream>

#include "../math//curve.h"

int main() {
    Curve quadraticBeizier;

    // Quadratic Bézier (3 points)
    quadraticBeizier.addPoint({0, 1});
    quadraticBeizier.addPoint({0, 0});
    quadraticBeizier.addPoint({1, 0});

    std::cout << "3-point curve value at t=0.5: " << quadraticBeizier.evaluate(0.5) << '\n';

    Curve cubicBezier;

    // Cubic Bézier (4 points)
    cubicBezier.addPoint({0, 1});
    cubicBezier.addPoint({0, 0});
    cubicBezier.addPoint({1, 0});
    cubicBezier.addPoint({1, 1});

    std::cout << "4-point curve value at t=0.5: " << cubicBezier.evaluate(0.5) << '\n';

    Curve quarticBezier;

    // Quartic Bézier (5 points)
    quarticBezier.addPoint({0, 1});
    quarticBezier.addPoint({0, 0});
    quarticBezier.addPoint({1, 0});
    quarticBezier.addPoint({1, 1});
    quarticBezier.addPoint({2, 1});

    std::cout << "5-point curve value at t=0.5: " << quarticBezier.evaluate(0.5) << '\n';
    return 0;
}