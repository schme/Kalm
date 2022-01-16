#pragma once

#include "include/maths.h"
#include "TimelineItem.h"

#include <vector>

namespace ks {

struct Timeline
{
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

	constexpr bool itemActive(const TimelineItem &item)
	{
		return item.frameStart <= currentFrame && item.frameEnd >= currentFrame;
	}

    constexpr float getPlaybackTime(float now) {
        return now - playbackStartTime;
    }

    constexpr static const float oneFrameTime = 1.0f/120.0f;

    static Timeline &get() {
        static Timeline timeline;
        return timeline;
    }

	std::vector<TimelineItem> items;

	int frameMin = 0;
	int frameMax = 120 * 60;
	int currentFrame = 0;
	int firstFrame = 0;

	float playbackStartTime = 0.0f;
	float playbackRate = 0.0f;
};

}
