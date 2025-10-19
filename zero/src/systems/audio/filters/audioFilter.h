#pragma once
#include <al.h>

#define AL_ALEXT_PROTOTYPES
#include <efx.h>

struct AudioFilter {
    virtual void applyFilter(ALuint filterId) const = 0;

    [[nodiscard]] constexpr virtual ALenum getFilterType() const = 0;

    virtual ~AudioFilter() = default;
};

struct LowPass final : public AudioFilter {
    void applyFilter(ALuint filterId) const override;

    [[nodiscard]] constexpr ALenum getFilterType() const override {
        return AL_FILTER_LOWPASS;
    }

    // [0.0, 1.0]
    float gain = 1;
    // [0.0, 1.0]
    float gainHf = 1;
};

struct HighPass final : public AudioFilter {
    void applyFilter(ALuint filterId) const override;

    [[nodiscard]] constexpr ALenum getFilterType() const override {
        return AL_FILTER_HIGHPASS;
    }

    // [0.0, 1.0]
    float gain = 1;
    // [0.0, 1.0]
    float gainLf = 1;
};

struct BandPass final : public AudioFilter {
    void applyFilter(ALuint filterId) const override;

    [[nodiscard]] constexpr ALenum getFilterType() const override {
        return AL_FILTER_BANDPASS;
    }

    // [0.0, 1.0]
    float gain = 1;
    // [0.0, 1.0]
    float gainLf = 1;
    // [0.0, 1.0]
    float gainHf = 1;
};