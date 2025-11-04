#include "audioEffect.h"

void EaxReverb::applyEffect(const ALuint effectId) const {
    alEffecti(effectId, AL_EFFECT_TYPE, getEffectType());

    alEffectf(effectId, AL_EAXREVERB_DENSITY, density);
    alEffectf(effectId, AL_EAXREVERB_DIFFUSION, diffusion);
    alEffectf(effectId, AL_EAXREVERB_GAIN, gain);
    alEffectf(effectId, AL_EAXREVERB_GAINHF, gainHf);
    alEffectf(effectId, AL_EAXREVERB_GAINLF, gainLf);
    alEffectf(effectId, AL_EAXREVERB_DECAY_TIME, decayTime);
    alEffectf(effectId, AL_EAXREVERB_DECAY_HFRATIO, decayHfRatio);
    alEffectf(effectId, AL_EAXREVERB_DECAY_LFRATIO, decayLfRatio);
    alEffectf(effectId, AL_EAXREVERB_REFLECTIONS_GAIN, reflectionsGain);
    alEffectf(effectId, AL_EAXREVERB_REFLECTIONS_DELAY, reflectionsDelay);

    const float reflectionsPanArray[3] = {reflectionsPan[0], reflectionsPan[1], reflectionsPan[2]};
    alEffectfv(effectId, AL_EAXREVERB_REFLECTIONS_PAN, reflectionsPanArray);

    alEffectf(effectId, AL_EAXREVERB_LATE_REVERB_GAIN, lateReverbGain);
    alEffectf(effectId, AL_EAXREVERB_LATE_REVERB_DELAY, lateReverbDelay);

    const float lateReflectionsPanArray[3] = {lateReverbPan[0], lateReverbPan[1], lateReverbPan[2]};
    alEffectfv(effectId, AL_EAXREVERB_LATE_REVERB_PAN, lateReflectionsPanArray);

    alEffectf(effectId, AL_EAXREVERB_ECHO_TIME, echoTime);
    alEffectf(effectId, AL_EAXREVERB_ECHO_DEPTH, echoDepth);
    alEffectf(effectId, AL_EAXREVERB_MODULATION_TIME, modulationTime);
    alEffectf(effectId, AL_EAXREVERB_MODULATION_DEPTH, modulationDepth);
    alEffectf(effectId, AL_EAXREVERB_AIR_ABSORPTION_GAINHF, airAbsorptionGainHf);
    alEffectf(effectId, AL_EAXREVERB_HFREFERENCE, hfReference);
    alEffectf(effectId, AL_EAXREVERB_LFREFERENCE, lfReference);
    alEffectf(effectId, AL_EAXREVERB_ROOM_ROLLOFF_FACTOR, roomRolloffFactor);
    alEffecti(effectId, AL_EAXREVERB_DECAY_HFLIMIT, decayHfLimit);
}
