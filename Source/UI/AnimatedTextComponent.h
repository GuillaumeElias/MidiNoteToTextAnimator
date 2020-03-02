#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../PluginProcessor.h"
#include "UpdatableComponent.h"

class AnimatedTextComponent  : public AnimatedAppComponent, 
                               public UpdatableComponent, 
                               private AudioProcessorValueTreeState::Listener,
                               private ComboBox::Listener
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

    void showNextLetter();

    void syncCurrentFont();

    AudioProcessorValueTreeState & valueTreeState;

    int counter;
    String text;

    Atomic<int> mode;

    Font currentFont;

    ComboBox fontSizeSelector;
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> fontSizeComboboxAttachment;

    ComboBox fontTypeSelector;
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> fontTypeComboboxAttachment;

    ComboBox backgroundColorSelector;
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> backgroundColorComboboxAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AnimatedTextComponent)
};
