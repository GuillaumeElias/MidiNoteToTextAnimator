#include "../../JuceLibraryCode/JuceHeader.h"
#include "PaceParametersComponent.h"
#include "../PluginProcessor.h"

namespace
{
    //==================================================================
    void setParameterValue(AudioProcessorValueTreeState & vts, float value)
    {
        auto parameter = vts.getParameter("fixedSpeed");
        auto range = vts.getParameterRange("fixedSpeed");

        parameter->beginChangeGesture();
        parameter->setValueNotifyingHost(range.convertTo0to1(value));
        parameter->endChangeGesture();
    }
}

//==============================================================================
PaceParametersComponent::PaceParametersComponent(AudioProcessorValueTreeState & vts)
    : valueTreeState(vts)
{
   
    //mode selector
    modeSelector.addItem("Midi note in", 1);
    modeSelector.addItem("Fixed speed", 2);
    modeComboboxAttachment = std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment>(valueTreeState, "mode", modeSelector);
    modeSelector.addListener(this);

    //speed input
    fixedSpeedInput.setText(String(*valueTreeState.getRawParameterValue("fixedSpeed")));
    fixedSpeedInput.setInputRestrictions(3, "0123456789");
    fixedSpeedInput.addListener(this);
    if (*valueTreeState.getRawParameterValue("mode") == 2)
    {
        fixedSpeedInput.setVisible(true);
    }

    //skip spaces button
    skipSpacesCheckbox.setButtonText("Skip spaces");
    skipSpacesCheckbox.setToggleState(valueTreeState.getParameterAsValue("skipSpaces").getValue(), false);
    skipSpacesCheckbox.onClick = [this] { valueTreeState.getParameter("skipSpaces")->setValueNotifyingHost(skipSpacesCheckbox.getToggleState()); };

    addAndMakeVisible(modeSelector);
    addChildComponent(fixedSpeedInput);
    addAndMakeVisible(skipSpacesCheckbox);
    
}

//==============================================================================
PaceParametersComponent::~PaceParametersComponent()
{
    
}

//==============================================================================
void PaceParametersComponent::paint (Graphics& g)
{
    
}

//==============================================================================
void PaceParametersComponent::resized()
{
    modeSelector.setBounds(5, 0, 200, 20);
    fixedSpeedInput.setBounds(230, 0, 30, 20);
    skipSpacesCheckbox.setBounds(getWidth() - 205, 0, 100, 20);
}

//==============================================================================
void PaceParametersComponent::comboBoxChanged(ComboBox *comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged->getSelectedId() == 2)
    {
        fixedSpeedInput.setVisible(true);
    }
    else
    {
        fixedSpeedInput.setVisible(false);
    }
}

//==============================================================================
void PaceParametersComponent::textEditorTextChanged(TextEditor &)
{
    setParameterValue(valueTreeState, fixedSpeedInput.getText().getFloatValue());
}


