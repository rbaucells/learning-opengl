#include "audioEffect.h"

void RingModulator::applyEffect(const ALuint effectId) const {
    alEffecti(effectId, AL_EFFECT_TYPE, getEffectType());

    alEffectf(effectId, AL_RING_MODULATOR_FREQUENCY, frequency);
    alEffectf(effectId, AL_RING_MODULATOR_HIGHPASS_CUTOFF, highpassCutoff);
    alEffecti(effectId, AL_RING_MODULATOR_WAVEFORM, waveForm);
}
