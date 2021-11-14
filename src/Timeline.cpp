#include "Timeline.h"

namespace ks {
    void Timeline::init()
    {
        this->frameMax = 120;
        this->currentFrame = 0;

        items.push_back({0, 2, 0});
        items.push_back({10, 18, 0});
        items.push_back({0, 30, 0});
        items.push_back({42, 58, 0});

        sequenceOptions =
            ImSequencer::SEQUENCER_OPTIONS::SEQUENCER_EDIT_ALL
            | ImSequencer::SEQUENCER_OPTIONS::SEQUENCER_ADD
            | ImSequencer::SEQUENCER_OPTIONS::SEQUENCER_DEL
            | ImSequencer::SEQUENCER_OPTIONS::SEQUENCER_COPYPASTE
        ;
    }
}
