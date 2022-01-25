#pragma once

#include "include/maths.h"
#include "TimelineItem.h"

#include <vector>

namespace ks {

struct Timeline
{
    void init();

    void togglePlayback(float now);
    float advanceTimestep(float now);

	constexpr bool itemActive(const TimelineItem &item)	{
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
