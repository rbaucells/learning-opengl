#pragma once
#include <string>

#include "OpenAL.h"

struct Sound {
    Sound(const std::string& path);
    ~Sound();
private:
};

class AudioManager {
public:
    AudioManager();

    void playSound(const Sound& sound);

    ~AudioManager();

    static AudioManager* mainManager;
};