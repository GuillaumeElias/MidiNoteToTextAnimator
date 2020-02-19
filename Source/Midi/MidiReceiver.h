

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class MidiReceiver : public ReferenceCountedObject
{

public:
    MidiReceiver();

    void handleMidiMessage(MidiBuffer& midiBuffer);

private:

};