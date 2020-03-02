#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "MidiReceiverTest.h"

namespace test
{

    static MidiReceiverTest testMidiReceiver;

    //==================================================
    void RunAllTests()
    {
        Logger::outputDebugString("===== TEST BUILD - Running tests first");

        int failures = 0;

        UnitTestRunner unitTestRunner;
        unitTestRunner.setPassesAreLogged(true);
        unitTestRunner.runAllTests();
        for (int i = 0; i < unitTestRunner.getNumResults(); ++i) {
            failures += unitTestRunner.getResult(i)->failures;
        }

        Logger::outputDebugString("===== TEST BUILD - number of tests: " + String(unitTestRunner.getNumResults()) + " number of failures : " + String(failures));
    }


}


