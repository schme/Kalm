#include "Timeline.h"

namespace ks {

void Timeline::init()
{
	this->frameMax = 120 * 60;
	this->currentFrame = 0;

	sequenceOptions =
		ImSequencer::SEQUENCER_OPTIONS::SEQUENCER_EDIT_ALL
		| ImSequencer::SEQUENCER_OPTIONS::SEQUENCER_ADD
		| ImSequencer::SEQUENCER_OPTIONS::SEQUENCER_DEL
		| ImSequencer::SEQUENCER_OPTIONS::SEQUENCER_COPYPASTE
	;
}

}
