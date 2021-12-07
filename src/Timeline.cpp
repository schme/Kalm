#include "Timeline.h"

namespace ks {

void Timeline::init()
{
	this->frameMax = 120 * 60;
	this->currentFrame = 0;

	items.push_back({"Main", 0, 120, TimelineItem::Type::Scene});
	items.push_back({"Scene0", 120, 240, TimelineItem::Type::Scene});

	sequenceOptions =
		ImSequencer::SEQUENCER_OPTIONS::SEQUENCER_EDIT_ALL
		| ImSequencer::SEQUENCER_OPTIONS::SEQUENCER_ADD
		| ImSequencer::SEQUENCER_OPTIONS::SEQUENCER_DEL
		| ImSequencer::SEQUENCER_OPTIONS::SEQUENCER_COPYPASTE
	;
}

}
