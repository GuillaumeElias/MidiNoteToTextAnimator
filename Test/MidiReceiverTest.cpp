#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../Source/Midi/MidiReceiver.h"
#include "../Source/UI/AnimatedTextComponent.h"

namespace test
{

    class MockAnimatedTextComponent : public AnimatedTextComponent
    {

    };

    class MockAnimatedTextUpdater : public AnimatedTextUpdater
    {
    public:
        MockAnimatedTextUpdater(AnimatedTextComponent * c) : AnimatedTextUpdater(c){ };
    };

    

    class MidiReceiverTest : public UnitTest
    {
    public:
        MidiReceiverTest() : UnitTest("MidiReceiverTest") {}

        void runTest() override
        {
            beginTest("Part 1");

            MidiReceiver midiReceiver;

            /*AudioProcessorValueTreeState * vts = new AudioProcessorValueTreeState();

            AnimatedTextComponent * c = new AnimatedTextComponent(*vts);
            
            midiReceiver.setAnimatedTextUpdater(std::make_unique<MockAnimatedTextUpdater>(c));*/

            expect(1);

        }
    };
}