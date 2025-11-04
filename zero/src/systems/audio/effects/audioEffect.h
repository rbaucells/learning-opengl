#pragma once

#define AL_ALEXT_PROTOTYPES
#include <efx.h>

#include "math++/math.h"

struct AudioEffect {
    virtual void applyEffect(ALuint effectId) const = 0;

    [[nodiscard]] constexpr virtual ALenum getEffectType() const = 0;

    virtual ~AudioEffect() = default;
};

struct EaxReverb final : public AudioEffect {
    void applyEffect(ALuint effectId) const override;

    [[nodiscard]] constexpr ALenum getEffectType() const override {
        return AL_EFFECT_EAXREVERB;
    }

    // [0.0, 1.0]
    float density = 1.0;
    // [0.0, 1.0]
    float diffusion = 1.0;
    // [0.0, 1.0]
    float gain = 0.32;
    // [0.0, 1.0]
    float gainHf = 0.89;
    // [0.0, 1.0]
    float gainLf = 0.0;
    // [0.1, 20.0] (seconds)
    float decayTime = 1.49;
    // [0.1, 2.0]
    float decayHfRatio = 0.83;
    // [0.1, 2.0]
    float decayLfRatio = 1.0;
    // [0.0, 3.16]
    float reflectionsGain = 0.05;
    // [0.0, 0.3]
    float reflectionsDelay = 0.007;
    // [[-1.0, -1.0, -1.0], [1.0, 1.0, 1.0]] (Vector<3>)
    Vector<3> reflectionsPan = {0, 0, 0};
    // [0.0, 10.0]
    float lateReverbGain = 1.26;
    // [0.0, 0.1] (seconds)
    float lateReverbDelay = 0.011;
    // [[-1.0, -1.0, -1.0], [1.0, 1.0, 1.0]] (Vector<3>)
    Vector<3> lateReverbPan = {0, 0, 0};
    // [0.075, 0.25]
    float echoTime = 0.25;
    // [0.0, 1.0]
    float echoDepth = 0.0;
    // [0.04, 4.0]
    float modulationTime = 0.25;
    // [0.0, 1.0]
    float modulationDepth = 0.0;
    // [0.892, 1.0]
    float airAbsorptionGainHf = 0.994;
    // [1000.0, 20000.0] (hz)
    float hfReference = 5000.0;
    // [20.0, 1000.0] (hz)
    float lfReference = 250.0;
    // [0.0, 10.0]
    float roomRolloffFactor = 0.0;
    // [true, false] (ON/OFF)
    bool decayHfLimit = true;
};

struct Reverb final : public AudioEffect {
    void applyEffect(ALuint effectId) const override;

    [[nodiscard]] constexpr ALenum getEffectType() const override {
        return AL_EFFECT_REVERB;
    }

    // [0.0, 1.0]
    float density = 1.0;
    // [0.0, 1.0]
    float diffusion = 1.0;
    // [0.0, 1.0]
    float gain = 0.32;
    // [0.0, 1.0]
    float gainHf = 0.89;
    // [0.1, 20.0] (seconds)
    float decayTime = 1.49;
    // [0.1, 2.0]
    float decayHfRatio = 0.83;
    // [0.0, 3.16]
    float reflectionsGain = 0.05;
    // [0.0, 0.3]
    float reflectionsDelay = 0.007;
    // [0.0, 10.0]
    float lateReverbGain = 1.26;
    // [0.0, 0.1] (seconds)
    float lateReverbDelay = 0.011;
    // [0.892, 1.0]
    float airAbsorptionGainHf = 0.994;
    // [0.0, 10.0]
    float roomRolloffFactor = 0.0;
    // [true, false] (ON/OFF)
    bool decayHfLimit = true;
};

struct Chorus final : public AudioEffect {
    enum WaveForm {
        sinusoid = AL_CHORUS_WAVEFORM_SINUSOID,
        triangle = AL_CHORUS_WAVEFORM_TRIANGLE,
    };

    void applyEffect(ALuint effectId) const override;

    [[nodiscard]] constexpr ALenum getEffectType() const override {
        return AL_EFFECT_CHORUS;
    }

    // [sinusoid, triangle] (Sin, Triangle)
    WaveForm waveForm = triangle;
    // [-180, 180] (degrees)
    float phase = 90;
    // [0.0, 10.0] (hz)
    float rate = 1.1;
    // [0.0, 1.0]
    float depth = 0.1;
    // [-1.0, 1.0]
    float feedback = 0.25;
    // [0.0, 0.016] (seconds)
    float delay = 0.016;
};

struct Distortion final : public AudioEffect {
    void applyEffect(ALuint effectId) const override;

    [[nodiscard]] constexpr ALenum getEffectType() const override {
        return AL_EFFECT_DISTORTION;
    }

    // [0.0, 1.0]
    float edge = 0.2;
    // [0.01, 1.0]
    float gain = 0.05;
    // [80.0, 24000.0]
    float lowpassCutoff = 8000.0;
    // [80.0, 24000.0]
    float eqCenter = 3600.0;
    // [80.0, 24000.0]
    float eqBandwidth = 3600.0;
};

struct Echo final : public AudioEffect {
    void applyEffect(ALuint effectId) const override;

    [[nodiscard]] constexpr ALenum getEffectType() const override {
        return AL_EFFECT_ECHO;
    }

    // [0.0, 0.207] (seconds)
    float delay = 0.1;
    // [0.0, 0.404] (seconds)
    float lrDelay = 0.1;
    // [0.0, 0.99]
    float damping = 0.5;
    // [0.0, 1.0]
    float feedback = 0.5;
    // [-1.0, 1.0]
    float spread = -1.0;
};

struct Flanger final : public AudioEffect {
    enum WaveForm {
        sinusoid = AL_FLANGER_WAVEFORM_SINUSOID,
        triangle = AL_FLANGER_WAVEFORM_TRIANGLE,
    };

    void applyEffect(ALuint effectId) const override;

    [[nodiscard]] constexpr ALenum getEffectType() const override {
        return AL_EFFECT_FLANGER;
    }

    // [sinusoid, triangle] (Sin, Triangle)
    WaveForm waveForm = triangle;
    // [-180, 180] (degrees)
    float phase = 0.0;
    // [0.0, 10.0] (hz)
    float rate = 0.27;
    // [0.0, 1.0]
    float depth = 1.0;
    // [-1.0, 1.0]
    float feedback = -0.5;
    // [0.0, 0.004] (seconds)
    float delay = 0.002;
};

struct FrequencyShifter final : public AudioEffect {
    enum Direction {
        down = AL_FREQUENCY_SHIFTER_DIRECTION_DOWN,
        up = AL_FREQUENCY_SHIFTER_DIRECTION_UP,
        off = AL_FREQUENCY_SHIFTER_DIRECTION_OFF
    };

    void applyEffect(ALuint effectId) const override;

    [[nodiscard]] constexpr ALenum getEffectType() const override {
        return AL_EFFECT_FREQUENCY_SHIFTER;
    }

    // default params
    float frequencyHz = 0;
    Direction leftDirection = down;
    Direction rightDirection = down;
};

struct VocalMorpher final : public AudioEffect {
    enum PhonemeType {
        a = AL_VOCAL_MORPHER_PHONEME_A,
        e = AL_VOCAL_MORPHER_PHONEME_E,
        i = AL_VOCAL_MORPHER_PHONEME_I,
        o = AL_VOCAL_MORPHER_PHONEME_O,
        u = AL_VOCAL_MORPHER_PHONEME_U,
        aa = AL_VOCAL_MORPHER_PHONEME_AA,
        ae = AL_VOCAL_MORPHER_PHONEME_AE,
        ah = AL_VOCAL_MORPHER_PHONEME_AH,
        ao = AL_VOCAL_MORPHER_PHONEME_AO,
        eh = AL_VOCAL_MORPHER_PHONEME_EH,
        er = AL_VOCAL_MORPHER_PHONEME_ER,
        ih = AL_VOCAL_MORPHER_PHONEME_IH,
        iy = AL_VOCAL_MORPHER_PHONEME_IY,
        uh = AL_VOCAL_MORPHER_PHONEME_UH,
        uw = AL_VOCAL_MORPHER_PHONEME_UW,
        b = AL_VOCAL_MORPHER_PHONEME_B,
        d = AL_VOCAL_MORPHER_PHONEME_D,
        f = AL_VOCAL_MORPHER_PHONEME_F,
        g = AL_VOCAL_MORPHER_PHONEME_G,
        j = AL_VOCAL_MORPHER_PHONEME_J,
        k = AL_VOCAL_MORPHER_PHONEME_K,
        l = AL_VOCAL_MORPHER_PHONEME_L,
        m = AL_VOCAL_MORPHER_PHONEME_M,
        n = AL_VOCAL_MORPHER_PHONEME_N,
        p = AL_VOCAL_MORPHER_PHONEME_P,
        r = AL_VOCAL_MORPHER_PHONEME_R,
        s = AL_VOCAL_MORPHER_PHONEME_S,
        t = AL_VOCAL_MORPHER_PHONEME_T,
        v = AL_VOCAL_MORPHER_PHONEME_V,
        z = AL_VOCAL_MORPHER_PHONEME_Z,
    };

    enum WaveForm {
        sinusoid = AL_VOCAL_MORPHER_WAVEFORM_SINUSOID,
        triangle = AL_VOCAL_MORPHER_WAVEFORM_TRIANGLE,
        saw_tooth = AL_VOCAL_MORPHER_WAVEFORM_SAWTOOTH
    };

    void applyEffect(ALuint effectId) const override;

    [[nodiscard]] constexpr ALenum getEffectType() const override {
        return AL_EFFECT_VOCAL_MORPHER;
    }

    // [0, 29] (phoneme type)
    PhonemeType phonemeA = a;
    // [0, 29] (phoneme type)
    PhonemeType phonemeB = er;
    // [-24, 24] (semitones)
    int phonemeACoarseTuning = 0;
    // [-24, 24] (semitones)
    int phonemeBCoarseTuning = 0;
    // [0, 2] (sin, triangle, saw)
    WaveForm waveForm = sinusoid;
    // [0.0, 10.0]
    float rate = 1.41;
};

struct PitchShifter final : public AudioEffect {
    void applyEffect(ALuint effectId) const override;

    [[nodiscard]] constexpr ALenum getEffectType() const override {
        return AL_EFFECT_PITCH_SHIFTER;
    }

    // [-12, 12] (semitones)
    int coarseTune = 12;
    // [-50, 50] (cents)
    int fineTune = 0;
};

struct RingModulator final : public AudioEffect {
    enum WaveForm {
        sinusoid = AL_RING_MODULATOR_SINUSOID,
        saw_tooth = AL_RING_MODULATOR_SAWTOOTH,
        square = AL_RING_MODULATOR_SQUARE
    };

    void applyEffect(ALuint effectId) const override;

    [[nodiscard]] constexpr ALenum getEffectType() const override {
        return AL_EFFECT_RING_MODULATOR;
    }

    // [0.0, 8000.0] (hz)
    float frequency = 440.0;
    // [0.0, 24000.0] (hz)
    float highpassCutoff = 800.0;
    // [0, 2] (sin, saw, square)
    WaveForm waveForm = sinusoid;
};

struct Autowah final : public AudioEffect {
    void applyEffect(ALuint effectId) const override;

    [[nodiscard]] constexpr ALenum getEffectType() const override {
        return AL_EFFECT_AUTOWAH;
    }

    // [0.0001, 1.0] (seconds)
    float attackTime = 0.06;
    // [0.0001, 1.0] (seconds)
    float releaseTime = 0.06;
    // [2.0, 1000.0]
    float resonance = 1000.0;
    // [0.00003, 31621.0]
    float peakGain = 11.22;
};

struct Compressor final : public AudioEffect {
    void applyEffect(ALuint effectId) const override;

    [[nodiscard]] constexpr ALenum getEffectType() const override {
        return AL_EFFECT_COMPRESSOR;
    }

    bool onOff = true;
};

struct Equalizer final : public AudioEffect {
    void applyEffect(ALuint effectId) const override;

    [[nodiscard]] constexpr ALenum getEffectType() const override {
        return AL_EFFECT_EQUALIZER;
    }

    // [0.126, 7.943]
    float lowGain = 1.0;
    // [50.0, 800.0] (hz)
    float lowCutoff = 200.0;

    // [0.126, 7.943]
    float mid1Gain = 1.0;
    // [200.0, 3000.0] (hz)
    float mid1Center = 500.0;
    // [0.01, 1.0]
    float mid1Width = 1.0;

    // [0.126, 7.943]
    float mid2Gain = 1.0;
    // [1000.0, 8000.0] (hz)
    float mid2Center = 3000.0;
    // [0.01, 1.0]
    float mid2Width = 1.0;

    // [0.126, 7.943]
    float highGain = 1.0;
    // [4000.0, 16000.0] (hz)
    float highCutoff = 6000.0;
};