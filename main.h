#pragma once
#include "camera.h"
#include <vector>

class Component;

// variables
inline int SCREEN_WIDTH = 1920;
inline int SCREEN_HEIGHT = 1080;

inline GLFWwindow *mainWindow;
inline Camera *camera;

inline std::vector<Component*> callStartBeforeNextUpdate {};

constexpr float fixedUpdatePerSecond = 60;
constexpr int fps = 60;

constexpr float fixedUpdateIntervalInMilli = 1000.0f / fixedUpdatePerSecond;