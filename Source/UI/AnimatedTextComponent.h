#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../PluginProcessor.h"
#include "UpdatableComponent.h"

class AnimatedTextComponent  : public AnimatedAppComponent, 
                               public UpdatableComponent, 
                               private AudioProcessorValueTreeState::Listener,
                               private ComboBox::Listener,
                               private ChangeListener,
                               private MouseListener
{
public:
    AnimatedTextComponent(AudioProcessorValueTreeState & vts);
    ~AnimatedTextComponent();

    void paint (Graphics&) override;
    void resized() override;
    void update() override;

    void setText(String text);
    void onMidiNoteIn() override;

private:

    void parameterChanged(const String &parameterID, float newValue) override;
    void comboBoxChanged(ComboBox *comboBoxThatHasChanged) override;
    void changeListenerCallback(ChangeBroadcaster* source) override;
    void mouseDown(const MouseEvent& event) override;

    void showNextLetter();

    void syncCurrentFont();
    void syncJustification();

    void updateToggleState(ToggleButton* button);

    AudioProcessorValueTreeState & valueTreeState;

    int counter;
    String text;

    Atomic<int> mode;

    Justification currentJustification;
    Font currentFont;
    Colour currentBackgroundColour;
    Colour currentTextColour;

    ComboBox fontSizeSelector;
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> fontSizeComboboxAttachment;

    ComboBox fontTypeSelector;
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> fontTypeComboboxAttachment;

    TextButton textColorButton;
    ColourSelector textColorSelector;

    TextButton backgroundColorButton;
    ColourSelector backgroundColorSelector;

    ComboBox justificationSelector;
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> justificationComboboxAttachment;

    ToggleButton boldCheckbox;
    ToggleButton italicCheckbox;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AnimatedTextComponent)
};
