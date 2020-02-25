#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "UI/AnimatedTextComponent.h"
#include "UI/PaceParametersComponent.h"

class MidiNoteToTextAudioProcessorEditor : public AudioProcessorEditor, public TextEditor::Listener
{
public:
    MidiNoteToTextAudioProcessorEditor (MidiNoteToTextAudioProcessor&, AudioProcessorValueTreeState& vts);
    ~MidiNoteToTextAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

    void textEditorTextChanged(TextEditor &) override;

    AnimatedTextComponent * getAnimatedTextComponent();

private:
    
    MidiNoteToTextAudioProcessor& processor;

    AudioProcessorValueTreeState& valueTreeState;

    PaceParametersComponent paceParameters;

    TextEditor textInput;
    Label inputLabel;

    AnimatedTextComponent animatedText;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiNoteToTextAudioProcessorEditor)
};
