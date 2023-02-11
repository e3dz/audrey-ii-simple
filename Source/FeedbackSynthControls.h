#pragma once
#ifndef INFS_FEEDBACKSYNTHCONTROLS_H
#define INFS_FEEDBACKSYNTHCONTROLS_H

#include <daisy.h>
#include <daisy_seed.h>
#include "FeedbackSynthEngine.h"
#include "ParameterRegistry.h"

namespace infrasonic {
namespace FeedbackSynth {

class Controls {

public:

    Controls() = default;
    ~Controls() = default;

    void Init(daisy::DaisySeed &hw, Engine &engine);

    void Update(daisy::DaisySeed &hw);

    void Process() {
        params_.Process();
    }

private:

    static const     size_t     kNumAdcChannels = 3;

    static constexpr daisy::Pin kPotAdcPin1     = daisy::seed::A1;
    static constexpr daisy::Pin kPotAdcPin2     = daisy::seed::A2;
    static constexpr daisy::Pin kMuxAdcPin      = daisy::seed::A0;
    static constexpr daisy::Pin kMuxAddrPin0    = daisy::seed::D1;
    static constexpr daisy::Pin kMuxAddrPin1    = daisy::seed::D2;
    static constexpr daisy::Pin kMuxAddrPin2    = daisy::seed::D3;

    /// Identifies a parameter of the synth engine
    enum class Parameter {
        StringPitch,
        FeedbackGain,
        FeedbackDelay,
        FeedbackLPFCutoff,
        FeedbackHPFCutoff,
        EchoDelayTime,
        EchoDelayFeedback,
        EchoDelaySend,
        ReverbMix,
        ReverbTime
    };

    using Parameters = ParameterRegistry<Parameter>;

    Parameters params_;

    void initADCs(daisy::DaisySeed &hw);
    void registerParams(Engine &engine);
};


}
}

#endif
