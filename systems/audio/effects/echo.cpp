#include "audioEffect.h"

void Echo::applyEffect(const ALuint effectId) const {
    alEffecti(effectId, AL_EFFECT_TYPE, getEffectType());

    alEffectf(effectId, AL_ECHO_DELAY, delay);
    alEffectf(effectId, AL_ECHO_LRDELAY, lrDelay);
    alEffectf(effectId, AL_ECHO_DAMPING, damping);
    alEffectf(effectId, AL_ECHO_FEEDBACK, feedback);
    alEffectf(effectId, AL_ECHO_SPREAD, spread);
}
