#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../Source/Midi/MidiReceiver.h"
#include "../Source/UI/AnimatedTextComponent.h"

namespace test
{
    //==============================================================
    class MockComponent : public UpdatableComponent
    {
    public:
        int call_count = 0;

        MockComponent() = default;
        void onMidiNoteIn() { call_count++;  };
    };

    //==============================================================
    class MidiReceiverTest : public UnitTest
    {
    public:
        //----------------------------------------------------------
        MidiReceiverTest() 
            : UnitTest("MidiReceiverTest")
            , updaterRawPtr ( new ComponentMidiUpdater(&component) )
        {}

        //----------------------------------------------------------
        void initialise() override
        {
            midiReceiver.setComponentMidiUpdater(std::unique_ptr<ComponentMidiUpdater>(updaterRawPtr));
            reset();
        }

        //----------------------------------------------------------
        void reset()
        {
            component.call_count = 0;
            midiBuffer.clear();
        }

        //----------------------------------------------------------
        void runTest() override
        {
            //test 1
            beginTest("Empty midi buffer test");

            midiReceiver.handleMidiMessage(midiBuffer);

            expect(component.call_count == 0);

            reset();

            //test 2
            beginTest("Midi buffer with one note in event");

            midiBuffer.addEvent(MidiMessage::noteOn(1, 64, 0.5f),0);
            midiReceiver.handleMidiMessage(midiBuffer);

            expect(updaterRawPtr->isUpdatePending() == true);

            updaterRawPtr->handleUpdateNowIfNeeded(); //syncronously update component

            expect(component.call_count == 1);

            reset();

            //test 3
            beginTest("Midi buffer with other type of event in buffer");

            midiBuffer.addEvent(MidiMessage::noteOff(1, 127, 0.9f), 14);
            midiReceiver.handleMidiMessage(midiBuffer);

            expect(updaterRawPtr->isUpdatePending() == false);

            updaterRawPtr->handleUpdateNowIfNeeded(); 

            expect(component.call_count == 0);

            reset();

        }

    private:

        MidiReceiver midiReceiver;
        MockComponent component;
        MidiBuffer midiBuffer;

        ComponentMidiUpdater * updaterRawPtr; //keeping raw ptr to avoid adding unnecessary accesser functions
    };
}