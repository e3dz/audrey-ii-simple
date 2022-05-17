#pragma once
#ifndef IFS_FEEDBACK_SYNTH_ENGINE_H
#define IFS_FEEDBACK_SYNTH_ENGINE_H

#include <daisysp.h>

namespace infrasonic {
namespace FeedbackSynth {

class Engine {

    public:

        Engine() {};
        ~Engine() {};

        Engine(const Engine &other) = delete;
        Engine(Engine &&other) = delete;
        Engine& operator=(const Engine &other) = delete;
        Engine& operator=(Engine &&other) = delete;

        void Init(const float sample_rate);

        void SetStringPitch(const float nn);

        void SetFeedbackGain(const float gain_dbfs);

        void SetFeedbackDelay(const float delay_s);

        void Process(float *outL, float *outR);

    private:
        // long enough for 250ms at 48kHz
        static constexpr size_t kMaxFeedbackDelaySamp = 12000;

        float sample_rate_;
        float fb_gain_ = 0.0f;


        float fb_delay_smooth_coef_;
        float fb_delay_samp_ = 64.f;
        float fb_delay_samp_target_ = 64.f;

        daisysp::WhiteNoise noise_;
        daisysp::String strings_[2];
        daisysp::DelayLine<float, kMaxFeedbackDelaySamp> fb_delayline_[2];
};

}
}

#endif
