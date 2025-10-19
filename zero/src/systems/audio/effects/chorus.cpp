#include "audioEffect.h"

void Chorus::applyEffect(const ALuint effectId) const {
    alEffecti(effectId, AL_EFFECT_TYPE, getEffectType());

    alEffecti(effectId, AL_CHORUS_WAVEFORM, waveForm);
    alEffectf(effectId, AL_CHORUS_PHASE, phase);
    alEffectf(effectId, AL_CHORUS_RATE, rate);
    alEffectf(effectId, AL_CHORUS_DEPTH, depth);
    alEffectf(effectId, AL_CHORUS_FEEDBACK, feedback);
    alEffectf(effectId, AL_CHORUS_DELAY, delay);
}
