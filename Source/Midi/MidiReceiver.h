

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "../UI/AnimatedTextUpdater.h"

class MidiReceiver : public ReferenceCountedObject
{

public:
    MidiReceiver();

    void setAnimatedTextUpdater(std::unique_ptr<AnimatedTextUpdater> updater);

    void handleMidiMessage(MidiBuffer& midiBuffer);

private:
    void notifyUpdater();

    std::unique_ptr<AnimatedTextUpdater> animatedTextUpdater;
};