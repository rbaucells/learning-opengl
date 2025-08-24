#pragma once
#include "GLFW/glfw3.h"
// variables
inline int SCREEN_WIDTH = 600;
inline int SCREEN_HEIGHT = 400;

inline GLFWwindow* window;

constexpr float fixedUpdatePerSecond = 60;
constexpr int fps = 120;

constexpr float fixedUpdateIntervalInMilli = 1000.0f / fixedUpdatePerSecond;