#include "audioEffect.h"

void Reverb::applyEffect(const ALuint effectId) const {
    alEffecti(effectId, AL_EFFECT_TYPE, getEffectType());

    alEffectf(effectId, AL_REVERB_DENSITY, density);
    alEffectf(effectId, AL_REVERB_DIFFUSION, diffusion);
    alEffectf(effectId, AL_REVERB_GAIN, gain);
    alEffectf(effectId, AL_REVERB_GAINHF, gainHf);
    alEffectf(effectId, AL_REVERB_DECAY_TIME, decayTime);
    alEffectf(effectId, AL_REVERB_DECAY_HFRATIO, decayHfRatio);
    alEffectf(effectId, AL_REVERB_REFLECTIONS_GAIN, reflectionsGain);
    alEffectf(effectId, AL_REVERB_REFLECTIONS_DELAY, reflectionsDelay);
    alEffectf(effectId, AL_REVERB_LATE_REVERB_GAIN, lateReverbGain);
    alEffectf(effectId, AL_REVERB_LATE_REVERB_DELAY, lateReverbDelay);
    alEffectf(effectId, AL_REVERB_AIR_ABSORPTION_GAINHF, airAbsorptionGainHf);
    alEffectf(effectId, AL_REVERB_ROOM_ROLLOFF_FACTOR, roomRolloffFactor);
    alEffecti(effectId, AL_REVERB_DECAY_HFLIMIT, decayHfLimit);
}
