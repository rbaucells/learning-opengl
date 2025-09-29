#pragma once
#include <memory>

#include "systems/betterEvents.h"
#include "systems/event.h"

// variables
constexpr float FIXED_UPDATE_PER_SECOND = 60;
constexpr int FPS = 60;

constexpr float FIXED_UPDATE_INTERVAL_IN_SECONDS = 1.0f / FIXED_UPDATE_PER_SECOND;

inline std::shared_ptr<Publisher<float>> updateEventPublisher = Publisher<float>::create();
inline std::shared_ptr<Publisher<float>> lateUpdateEventPublisher = Publisher<float>::create();
inline std::shared_ptr<Publisher<float>> fixedUpdateEventPublisher = Publisher<float>::create();