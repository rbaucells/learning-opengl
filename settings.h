#pragma once
#include "systems/event.h"

// variables
constexpr float FIXED_UPDATE_PER_SECOND = 60;
constexpr int FPS = 60;

constexpr float FIXED_UPDATE_INTERVAL_IN_SECONDS = 1.0f / FIXED_UPDATE_PER_SECOND;

inline Event<float> updateEvent;
inline Event<float> lateUpdateEvent;
inline Event<float> fixedUpdateEvent;