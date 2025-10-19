#pragma once

namespace Settings {
    constexpr float FIXED_UPDATE_PER_SECOND = 60;
    inline int fps = 60;

    inline float windowSize = 5;
    inline std::string windowName = "Zero Game Engine";

    constexpr float FIXED_UPDATE_INTERVAL_IN_SECONDS = 1.0f / FIXED_UPDATE_PER_SECOND;
}