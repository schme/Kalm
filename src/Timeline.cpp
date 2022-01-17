#include "Timeline.h"

namespace ks {

void Timeline::init()
{
}

void Timeline::togglePlayback(float now) {
	float &currentPlaybackRate = Timeline::get().playbackRate;
	if (currentPlaybackRate > 0.0f)
		playbackRate = 0.0f;
	else {
		currentPlaybackRate = 1.0f;

		// Calculate relative to start frame
		playbackStartTime = now - (currentFrame * oneFrameTime);
	}
}

float Timeline::advanceTimestep(float now) {
	if (math::isAboutZero(playbackRate))
		return currentFrame * oneFrameTime;

	float maxTime = frameMax * oneFrameTime;
	float time = (now - playbackStartTime);

	// float modulo
	time = time - maxTime * std::floor(time / maxTime);
	currentFrame = int(time / maxTime * frameMax);

	return time;
}

}
