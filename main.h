#pragma once
#include "components/camera.h"
#include <vector>

#include "systems/event.h"

struct GLFWwindow;
class Component;

// variables
inline int screenWidth = 1920;
inline int screenHeight = 1080;

inline GLFWwindow *mainWindow;

inline std::vector<std::weak_ptr<Component>> componentsToInitialize {};

constexpr float FIXED_UPDATE_PER_SECOND = 60;
constexpr int FPS = 60;

// constexpr float fixedUpdateIntervalInMilli = 1000.0f / fixedUpdatePerSecond;
constexpr float FIXED_UPDATE_INTERVAL_IN_SECONDS = 1.0f / FIXED_UPDATE_PER_SECOND;

inline Event<double> updateEvent;
inline Event<double> lateUpdateEvent;
inline Event<double> fixedUpdateEvent;