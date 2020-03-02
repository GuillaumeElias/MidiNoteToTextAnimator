
#include "../JuceLibraryCode/JuceHeader.h"
#include "AnimatedTextComponent.h"
#include "../Constants.h"


//==============================================================================
AnimatedTextComponent::AnimatedTextComponent(AudioProcessorValueTreeState & vts)
    : valueTreeState(vts)
{
    mode = static_cast<int>(*valueTreeState.getRawParameterValue("mode"));
    valueTreeState.addParameterListener("mode", this);

    int speed = static_cast<int>(*valueTreeState.getRawParameterValue("fixedSpeed"));
    valueTreeState.addParameterListener("fixedSpeed", this);
    setFramesPerSecond(speed);

    syncCurrentFont();
   
    //font size
    NormalisableRange<float> range = valueTreeState.getParameterRange("fontSize");
    for (int i = range.getRange().getStart(); i < range.getRange().getEnd(); i+= 2)
    {
        fontSizeSelector.addItem(String(i), i);
    }
    
    fontSizeComboboxAttachment = std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment>(valueTreeState, "fontSize", fontSizeSelector);
    fontSizeSelector.addListener(this);

    //font type
    fontTypeSelector.addItemList(FONTS, 1);
    fontTypeComboboxAttachment = std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment>(valueTreeState, "fontType", fontTypeSelector);
    fontTypeSelector.addListener(this);

    //background color
    backgroundColorSelector.addItem("Black", 1);
    backgroundColorSelector.addItem("White", 2);
    backgroundColorComboboxAttachment = std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment>(valueTreeState, "textBackgroundColor", backgroundColorSelector);
    backgroundColorSelector.addListener(this);

    addAndMakeVisible(fontSizeSelector);
    addAndMakeVisible(fontTypeSelector);
    addAndMakeVisible(backgroundColorSelector);
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
    g.setFont (currentFont);
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
    fontSizeSelector.setBounds(5, 5, 100, 20);

    fontTypeSelector.setBounds(105, 5, 100, 20);

    backgroundColorSelector.setBounds(205, 5, 100, 20);

}

//==============================================================================
void AnimatedTextComponent::setText(String txt) 
{
    this->counter = 0;
    this->text = txt;
}

//==============================================================================
void AnimatedTextComponent::onMidiNoteIn()
{
    if (mode.get() == 1 && isShowing())
    {
        showNextLetter();
    }
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
        mode.set(static_cast<int>(newValue));
    }
    else if (parameterID == "fixedSpeed")
    {
        setFramesPerSecond(static_cast<int>(newValue));
    }
}

//==============================================================================
void AnimatedTextComponent::comboBoxChanged(ComboBox * comboBoxThatHasChanged)
{
    
    if (comboBoxThatHasChanged == &fontSizeSelector)
    {
        int fontHeight = fontSizeSelector.getSelectedIdAsValue().getValue();
        currentFont.setHeight(fontHeight);
    }
    else if (comboBoxThatHasChanged == &fontTypeSelector)
    {
        String fontType = FONTS[fontTypeSelector.getSelectedItemIndex()];
        currentFont.setTypefaceName(fontType);
    }

    repaint();
}

//==============================================================================
void AnimatedTextComponent::syncCurrentFont()
{
    String typefaceName = FONTS[*valueTreeState.getRawParameterValue("fontType") - 1];
    int fontHeight = *valueTreeState.getRawParameterValue("fontSize");

    currentFont.setTypefaceName(typefaceName);
    currentFont.setHeight(fontHeight);
}
