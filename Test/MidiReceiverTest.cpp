#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../Source/Midi/MidiReceiver.h"
#include "../Source/UI/AnimatedTextComponent.h"

namespace test
{

    class MockComponent : public UpdatableComponent
    {
    public:
        int call_count = 0;

        MockComponent() = default;
        void onMidiNoteIn() { call_count++;  };
    };

 
    class MidiReceiverTest : public UnitTest
    {
    public:
        MidiReceiverTest() : UnitTest("MidiReceiverTest") {}

        void runTest() override
        {

            //set up
            MidiReceiver midiReceiver;
            MockComponent component;
            midiReceiver.setComponentMidiUpdater(std::make_unique<ComponentMidiUpdater>(&component));


            //call with empty buffer

            beginTest("Empty midi buffer test");

            MidiBuffer midiBuffer;
            midiReceiver.handleMidiMessage(midiBuffer);

            expect(component.call_count == 0);

        }
    };
}