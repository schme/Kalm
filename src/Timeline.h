#pragma once

#include "gui/TimelineGui.h"

#include "include/maths.h"

namespace ks {

struct Timeline : public TimelineGui
{
public:
    void init();

    void togglePlayback(float now) {
        float &currentPlaybackRate = Timeline::get().playbackRate;
        if (currentPlaybackRate > 0.0f)
            playbackRate = 0.0f;
        else {
            currentPlaybackRate = 1.0f;

            // Calculate relative to start frame
            playbackStartTime = now - (currentFrame * oneFrameTime);
        }
    }

    float advanceTimestep(float now) {
        if (math::isAboutZero(playbackRate))
            return currentFrame * oneFrameTime;

        float maxTime = frameMax * oneFrameTime;
        float time = (now - playbackStartTime);

        // float modulo
        time = time - maxTime * std::floor(time / maxTime);
        currentFrame = int(time / maxTime * frameMax);

        return time;
    }

    constexpr float getPlaybackTime(float now) {
        return now - playbackStartTime;
    }

    constexpr static const float oneFrameTime = 1.0f/120.0f;

    static Timeline &get() {
        static Timeline timeline;
        return timeline;
    }
};

}
