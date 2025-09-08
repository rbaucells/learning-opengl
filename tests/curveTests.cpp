#include "../math/mathematics.h"

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

    Curve QuarticBezier;

    // Quartic Bézier (5 points)
    QuarticBezier.addPoint({0, 1});
    QuarticBezier.addPoint({0, 0});
    QuarticBezier.addPoint({1, 0});
    QuarticBezier.addPoint({1, 1});
    QuarticBezier.addPoint({2, 1});

    std::cout << "5-point curve value at t=0.5: " << QuarticBezier.evaluate(0.5) << '\n';
    return 0;
}