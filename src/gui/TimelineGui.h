#pragma once

#include "ImSequencer.h"
#include "TimelineItem.h"

namespace ks {

struct Timeline;

class TimelineGui : public ImSequencer::SequenceInterface
{
public:
	void init(Timeline *);

	explicit TimelineGui() {};
	explicit TimelineGui(TimelineGui *) {};

	int GetFrameMin() const override;
	int GetFrameMax() const override;
	int GetItemCount() const override;

	int GetItemTypeCount() const override;

	const char* GetItemTypeName(int /*typeIndex*/) const override;
	const char* GetItemLabel(int /*index*/) const override;

	void Get(int index, int** start, int** end, int* type, unsigned int* color) override;
	void Add(int /*type*/) override;
	void Del(int /*index*/) override;

#if 0
	void CustomDraw(int /*index*/, ImDrawList* /*draw_list*/, const ImRect& /*rc*/, const ImRect& /*legendRect*/, const ImRect& /*clippingRect*/, const ImRect& /*legendClippingRect*/) override;
	void CustomDrawCompact(int /*index*/, ImDrawList* /*draw_list*/, const ImRect& /*rc*/, const ImRect& /*clippingRect*/) override;
#endif

	Timeline *timeline = nullptr;

	int selectedEntry = -1;
	int sequenceOptions = 0;
	bool expanded = true;
};

}
