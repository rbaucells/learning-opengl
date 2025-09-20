#include "audioEffect.h"

void Flanger::applyEffect(ALuint effectId) const {
    alEffecti(effectId, AL_EFFECT_TYPE, getEffectType());

    alEffecti(effectId, AL_FLANGER_WAVEFORM, waveForm);
    alEffectf(effectId, AL_FLANGER_PHASE, phase);
    alEffectf(effectId, AL_FLANGER_RATE, rate);
    alEffectf(effectId, AL_FLANGER_DEPTH, depth);
    alEffectf(effectId, AL_FLANGER_FEEDBACK, feedback);
    alEffectf(effectId, AL_FLANGER_DELAY, delay);
}
