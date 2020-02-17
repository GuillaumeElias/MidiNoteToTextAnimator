
#include "../JuceLibraryCode/JuceHeader.h"
#include "AnimatedTextComponent.h"

//==============================================================================
AnimatedTextComponent::AnimatedTextComponent(AudioProcessorValueTreeState & vts)
    : valueTreeState(vts)
{
    mode = *valueTreeState.getRawParameterValue("mode");
    valueTreeState.addParameterListener("mode", this);

    int speed = *valueTreeState.getRawParameterValue("fixedSpeed");
    valueTreeState.addParameterListener("fixedSpeed", this);
    setFramesPerSecond(speed);
}

//==============================================================================
AnimatedTextComponent::~AnimatedTextComponent()
{
}

//==============================================================================
void AnimatedTextComponent::paint (Graphics& g)
{

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::white);
    g.setFont (14.0f);
    g.drawText (text.substring(0,counter), getLocalBounds(),
                Justification::centred, true);   // draw some placeholder text
}

//==============================================================================
void AnimatedTextComponent::update()
{
    if (mode.get() == 2) //fixed speed
    {
        showNextLetter();
    }
}

//==============================================================================
void AnimatedTextComponent::resized()
{
   
}

//==============================================================================
void AnimatedTextComponent::setText(String txt) 
{
    this->counter = 0;
    this->text = txt;
}

//==============================================================================
void AnimatedTextComponent::showNextLetter()
{
    if (counter < text.length())
    {
        counter++;
    }
}

//==============================================================================
void AnimatedTextComponent::parameterChanged(const String &parameterID, float newValue)
{
    if (parameterID == "mode")
    {
        mode.set(newValue);
    }
    else if (parameterID == "fixedSpeed")
    {
        setFramesPerSecond(newValue);
    }
}
