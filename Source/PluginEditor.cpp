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
    setLookAndFeel(&lookAndFeel);
    setSize(800, 600);

    
    //text input
    textInput.addListener(this);
    textInput.setText("Enter text here");
    textInput.setMultiLine(true);

    //input label
    inputLabel.setText("Text input:", dontSendNotification);
    inputLabel.attachToComponent(&textInput, true);
    inputLabel.setColour(Label::textColourId, Colours::orange);
    inputLabel.setJustificationType(Justification::right);

    //animated text
    animatedText.setText(textInput.getText());
    animatedText.setOnFullScreenLambda([&](bool fullscreen) { 
        paceParameters.setVisible(!fullscreen);
        textInput.setVisible(!fullscreen);
        inputLabel.setVisible(!fullscreen);
        animatedText.setBounds(0, fullscreen ? 0 : 150, getWidth(), getHeight() - (fullscreen ? 0 : 150));
    } );


    addAndMakeVisible(paceParameters);
    addAndMakeVisible(inputLabel);
    addAndMakeVisible(textInput);
    addAndMakeVisible(animatedText);
}

//==============================================================================
MidiNoteToTextAudioProcessorEditor::~MidiNoteToTextAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
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

    textInput.setBounds(100, 60, getWidth() - 110, 50);

    animatedText.setBounds(0, 150, getWidth(), getHeight() - 150);

}

//==============================================================================
AnimatedTextComponent * MidiNoteToTextAudioProcessorEditor::getAnimatedTextComponent()
{
    return &animatedText;
}