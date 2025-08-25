#pragma once
#include "GLFW/glfw3.h"
// variables
inline int SCREEN_WIDTH = 1920;
inline int SCREEN_HEIGHT = 1080;

inline GLFWwindow* window;

constexpr float fixedUpdatePerSecond = 60;
constexpr int fps = 60;

constexpr float fixedUpdateIntervalInMilli = 1000.0f / fixedUpdatePerSecond;