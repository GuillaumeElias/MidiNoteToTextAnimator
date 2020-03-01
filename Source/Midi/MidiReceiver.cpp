

#include "MidiReceiver.h"

//=====================================================================================
MidiReceiver::MidiReceiver()
{
}

//=====================================================================================
void MidiReceiver::setComponentMidiUpdater(std::unique_ptr<ComponentMidiUpdater> updater)
{
    midiComponentUpdater = std::move(updater);
}

//=====================================================================================
void MidiReceiver::handleMidiMessage(MidiBuffer & midiBuffer)
{

    MidiBuffer::Iterator bufferIterator(midiBuffer);
    MidiMessage midiEvent;
    int samplePosition;

    while (bufferIterator.getNextEvent(midiEvent, samplePosition))
    {
        if (midiEvent.isNoteOn())
        {
            Logger::outputDebugString("note on");
            notifyUpdater();
        }
    }
}


//=====================================================================================
void MidiReceiver::notifyUpdater()
{
    if (midiComponentUpdater)
    {
        midiComponentUpdater->triggerAsyncUpdate();
    }
}


/*//=====================================================================================
void MidiReceiver::handleIncomingMidiMessage(MidiInput* source, const MidiMessage& message)
{
    const ScopedValueSetter<bool> scopedInputFlag(isAddingFromMidiInput, true);
    keyboardState.processNextMidiEvent(message);
    
    Logger::outputDebugString("incoming");
}

//=====================================================================================
void MidiReceiver::handleNoteOn(MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity)
{
    Logger::outputDebugString("note on");
}

//=====================================================================================
void MidiReceiver::handleNoteOff(MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity)
{
    Logger::outputDebugString("note off");
}*/

