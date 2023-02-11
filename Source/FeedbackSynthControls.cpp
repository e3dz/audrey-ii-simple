#include "FeedbackSynthControls.h"
#include <functional>

using namespace infrasonic;
using namespace infrasonic::FeedbackSynth;
using namespace daisy;

constexpr Pin FeedbackSynth::Controls::kPotAdcPin1;
constexpr Pin FeedbackSynth::Controls::kPotAdcPin2;
constexpr Pin FeedbackSynth::Controls::kMuxAdcPin;
constexpr Pin FeedbackSynth::Controls::kMuxAddrPin0;
constexpr Pin FeedbackSynth::Controls::kMuxAddrPin1;
constexpr Pin FeedbackSynth::Controls::kMuxAddrPin2;

void Controls::Init(DaisySeed &hw, Engine &engine) {
    params_.Init(hw.AudioSampleRate() / hw.AudioBlockSize());
    initADCs(hw);
    registerParams(engine);
}

void Controls::Update(DaisySeed &hw) {
    params_.UpdateNormalized(Parameter::StringPitch,        1.0f - hw.adc.GetMuxFloat(0, 0));
    params_.UpdateNormalized(Parameter::FeedbackGain,       1.0f - hw.adc.GetMuxFloat(0, 1));
    params_.UpdateNormalized(Parameter::FeedbackDelay,      1.0f - hw.adc.GetMuxFloat(0, 2));
    params_.UpdateNormalized(Parameter::FeedbackLPFCutoff,  1.0f - hw.adc.GetMuxFloat(0, 3));
    params_.UpdateNormalized(Parameter::FeedbackHPFCutoff,  1.0f - hw.adc.GetMuxFloat(0, 4));
    params_.UpdateNormalized(Parameter::EchoDelayTime,      1.0f - hw.adc.GetMuxFloat(0, 5));
    params_.UpdateNormalized(Parameter::EchoDelayFeedback,  1.0f - hw.adc.GetMuxFloat(0, 6));
    params_.UpdateNormalized(Parameter::EchoDelaySend,      1.0f - hw.adc.GetMuxFloat(0, 7));

    params_.UpdateNormalized(Parameter::ReverbTime,         1.0f - hw.adc.GetFloat(1));
    params_.UpdateNormalized(Parameter::ReverbMix,          1.0f - hw.adc.GetFloat(2));
}

void Controls::initADCs(DaisySeed &hw) {
    AdcChannelConfig config[kNumAdcChannels];

    // First channel = multiplexed 8x
    config[0].InitMux(kMuxAdcPin, 8, kMuxAddrPin0, kMuxAddrPin1, kMuxAddrPin2);
    config[1].InitSingle(kPotAdcPin1);
    config[2].InitSingle(kPotAdcPin2);

    hw.adc.Init(config, kNumAdcChannels);
    hw.adc.Start();
}

void Controls::registerParams(Engine &engine) {
    using namespace std::placeholders;

    // Pitch as nn
    params_.Register(Parameter::StringPitch, 40.0f, 16.0f, 72.0f, std::bind(&Engine::SetStringPitch, &engine, _1), 0.2f);

    // Feedback Gain in dbFS
    params_.Register(Parameter::FeedbackGain, -60.0f, -60.0f, 12.0f, std::bind(&Engine::SetFeedbackGain, &engine, _1));

    // Feedback delay in seconds
    params_.Register(Parameter::FeedbackDelay, 0.001f, 0.001f, 0.1f, std::bind(&Engine::SetFeedbackDelay, &engine, _1), 1.0f, daisysp::Mapping::EXP);

    // Feedback filter cutoff in hz
    params_.Register(Parameter::FeedbackLPFCutoff, 18000.0f, 100.0f, 18000.0f, std::bind(&Engine::SetFeedbackLPFCutoff, &engine, _1), 0.05f, daisysp::Mapping::LOG);
    params_.Register(Parameter::FeedbackHPFCutoff, 250.0f, 10.0f, 4000.0f, std::bind(&Engine::SetFeedbackHPFCutoff, &engine, _1), 0.05f, daisysp::Mapping::LOG);

    // Echo Delay time in s
    params_.Register(Parameter::EchoDelayTime, 0.5f, 0.05f, 5.0f, std::bind(&Engine::SetEchoDelayTime, &engine, _1), 0.1f, daisysp::Mapping::EXP);

    // Echo Delay feedback
    params_.Register(Parameter::EchoDelayFeedback, 0.0f, 0.0f, 1.5f, std::bind(&Engine::SetEchoDelayFeedback, &engine, _1));

    // Echo Delay send
    params_.Register(Parameter::EchoDelaySend, 0.0f, 0.0f, 1.0f, std::bind(&Engine::SetEchoDelaySendAmount, &engine, _1), 0.05f, daisysp::Mapping::EXP);

    // Reverb Mix
    params_.Register(Parameter::ReverbMix, 0.0f, 0.0f, 1.0f, std::bind(&Engine::SetReverbMix, &engine, _1));

    // Reverb Tiem
    params_.Register(Parameter::ReverbTime, 0.0f, 0.0f, 1.0f, std::bind(&Engine::SetReverbTime, &engine, _1));
}
