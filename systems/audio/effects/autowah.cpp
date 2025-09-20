#include "audioEffect.h"

void Autowah::applyEffect(const ALuint effectId) const {
    alEffecti(effectId, AL_EFFECT_TYPE, getEffectType());

    alEffectf(effectId, AL_AUTOWAH_ATTACK_TIME, attackTime);
    alEffectf(effectId, AL_AUTOWAH_RELEASE_TIME, releaseTime);
    alEffectf(effectId, AL_AUTOWAH_RESONANCE, resonance);
    alEffectf(effectId, AL_AUTOWAH_PEAK_GAIN, peakGain);
}
