#pragma once
#include "systems/betterEvents.h"
#include "systems/event.h"

// variables
constexpr float FIXED_UPDATE_PER_SECOND = 60;
constexpr int FPS = 60;

constexpr float FIXED_UPDATE_INTERVAL_IN_SECONDS = 1.0f / FIXED_UPDATE_PER_SECOND;

inline Publisher<float> updateEventDispatcher;
inline Publisher<float> lateUpdateEventDispatcher;
inline Publisher<float> fixedUpdateEventDispatcher;