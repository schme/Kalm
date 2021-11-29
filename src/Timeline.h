#pragma once

#include "gui/TimelineGui.h"

#include "math.h"

namespace ks {

class Timeline : public TimelineGui
{
public:
    void init();

    static Timeline &get() {
        static Timeline timeline;
        return timeline;
    }

    constexpr static const float oneFrameTime = 1.0f/60.0f;

    constexpr float getPlaybackTime(float now) {
        return now - playbackStartTime;
    }

    void togglePlayback(float now) {
        float &currentPlaybackRate = Timeline::get().playbackRate;
        if (currentPlaybackRate > 0.0f)
            Timeline::get().playbackRate = 0.0f;
        else {
            currentPlaybackRate = 1.0f;
            // Calculate relative to start frame
            playbackStartTime = now - (currentFrame * oneFrameTime);
        }
    }

    float timestep(float now) {

        if (playbackRate == 0.0f)
            return currentFrame * oneFrameTime;

        float maxTime = frameMax * oneFrameTime;
        float time = (now - playbackStartTime);
        // float modulo
        time = time - maxTime * std::floor(time / maxTime);
        currentFrame = int(time / maxTime * frameMax);

        return time;
    }
};

}
