#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class PaceParametersComponent : public Component, private ComboBox::Listener, private TextEditor::Listener
{
public:
    PaceParametersComponent(AudioProcessorValueTreeState & vts);
    ~PaceParametersComponent();

    void paint (Graphics&) override;
    void resized() override;

private:
    void comboBoxChanged(ComboBox *comboBoxThatHasChanged) override;
    void textEditorTextChanged(TextEditor &) override;

    AudioProcessorValueTreeState & valueTreeState;

    ComboBox modeSelector;
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> modeComboboxAttachment;

    TextEditor fixedSpeedInput;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PaceParametersComponent)
};
