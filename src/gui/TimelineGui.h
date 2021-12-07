#pragma once

#include "ImSequencer.h"
#include "TimelineItem.h"

#include <vector>

namespace ks {

class TimelineGui : public ImSequencer::SequenceInterface
{
public:
	explicit TimelineGui() {};
	explicit TimelineGui(TimelineGui *) {};

	int GetFrameMin() const override;
	int GetFrameMax() const override;
	int GetItemCount() const override;

	void Get(int index, int** start, int** end, int* type, unsigned int* color) override;

#if 0
	void CustomDraw(int /*index*/, ImDrawList* /*draw_list*/, const ImRect& /*rc*/, const ImRect& /*legendRect*/, const ImRect& /*clippingRect*/, const ImRect& /*legendClippingRect*/) override;
	void CustomDrawCompact(int /*index*/, ImDrawList* /*draw_list*/, const ImRect& /*rc*/, const ImRect& /*clippingRect*/) override;
#endif

	std::vector<TimelineItem> items;
	int frameMin = 0;
	int frameMax = 120;

	int selectedEntry = -1;
	int firstFrame = 0;
	int currentFrame = 0;
	int sequenceOptions = 0;
	bool expanded = true;

    float playbackRate = 0.0f;
    float playbackStartTime = 0.0f;
};

}
