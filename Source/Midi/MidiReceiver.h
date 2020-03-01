

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "../UI/ComponentMidiUpdater.h"

class MidiReceiver : public ReferenceCountedObject
{

public:
    MidiReceiver();

    void setComponentMidiUpdater(std::unique_ptr<ComponentMidiUpdater> updater);

    void handleMidiMessage(MidiBuffer& midiBuffer);

private:
    void notifyUpdater();

    std::unique_ptr<ComponentMidiUpdater> midiComponentUpdater;
};