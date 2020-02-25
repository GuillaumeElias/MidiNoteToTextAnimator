#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MidiNoteToTextAudioProcessorEditor::MidiNoteToTextAudioProcessorEditor (MidiNoteToTextAudioProcessor& p, AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p)
    , processor (p)
    , valueTreeState(vts)
    , paceParameters(vts)
    , animatedText(vts)
{

    setSize (400, 300);

    
    //text input
    textInput.addListener(this);


    //input label
    inputLabel.setText("Text input:", dontSendNotification);
    inputLabel.attachToComponent(&textInput, true);
    inputLabel.setColour(Label::textColourId, Colours::orange);
    inputLabel.setJustificationType(Justification::right);

    //displaylabel

    animatedText.setText(textInput.getText());

    addAndMakeVisible(paceParameters);
    addAndMakeVisible(inputLabel);
    addAndMakeVisible(textInput);
    addAndMakeVisible(animatedText);
}

//==============================================================================
MidiNoteToTextAudioProcessorEditor::~MidiNoteToTextAudioProcessorEditor()
{
    textInput.removeListener(this);
}

//==============================================================================
void MidiNoteToTextAudioProcessorEditor::textEditorTextChanged(TextEditor &) 
{
    animatedText.setText(textInput.getText());
}

//==============================================================================
void MidiNoteToTextAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll(Colours::black);
}

//==============================================================================
void MidiNoteToTextAudioProcessorEditor::resized()
{

    paceParameters.setBounds(5, 5, getWidth(), 50);

    textInput.setBounds(100, 60, getWidth() - 110, 20);

    animatedText.setBounds(0, 150, getWidth(), 20);

}

//==============================================================================
AnimatedTextComponent * MidiNoteToTextAudioProcessorEditor::getAnimatedTextComponent()
{
    return &animatedText;
}