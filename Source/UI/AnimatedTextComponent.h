#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../PluginProcessor.h"

class AnimatedTextComponent  : public AnimatedAppComponent, private AudioProcessorValueTreeState::Listener
{
public:
    AnimatedTextComponent(AudioProcessorValueTreeState & vts);
    ~AnimatedTextComponent();

    void paint (Graphics&) override;
    void resized() override;
    void update() override;

    void setText(String text);
    void onMidiNoteIn();

private:

    void parameterChanged(const String &parameterID, float newValue);
    void showNextLetter();

    AudioProcessorValueTreeState & valueTreeState;

    int counter;
    String text;

    Atomic<int> mode;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AnimatedTextComponent)
};
