
#include "../JuceLibraryCode/JuceHeader.h"
#include "AnimatedTextComponent.h"
#include "../Constants.h"


//==============================================================================
AnimatedTextComponent::AnimatedTextComponent(AudioProcessorValueTreeState & vts)
    : valueTreeState(vts)
    , currentJustification(Justification::Flags::left)
    , currentBackgroundColour(getLookAndFeel().findColour(ResizableWindow::backgroundColourId))
    , currentTextColour(Colours::white)
    , fullscreen(false)
{
    mode = static_cast<int>(*valueTreeState.getRawParameterValue("mode"));
    valueTreeState.addParameterListener("mode", this);

    int speed = static_cast<int>(*valueTreeState.getRawParameterValue("fixedSpeed"));
    valueTreeState.addParameterListener("fixedSpeed", this);
    setFramesPerSecond(speed);

    syncCurrentFont();
    syncJustification();

   
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

    //text color
    textColorButton.setButtonText("text color");
    textColorButton.addMouseListener(this, true);

    textColorSelector.setName("text");
    textColorSelector.addChangeListener(this);

    //background color
    backgroundColorButton.setButtonText("background color");
    backgroundColorButton.addMouseListener(this, true);

    backgroundColorSelector.setName("background");
    backgroundColorSelector.setCurrentColour(findColour(TextButton::buttonColourId));
    backgroundColorSelector.addChangeListener(this);

    //justification
    justificationSelector.addItemList(JUSTIFICATIONS, 1);
    justificationComboboxAttachment = std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment>(valueTreeState, "textJustification", justificationSelector);
    justificationSelector.addListener(this);

    //bold
    boldCheckbox.setButtonText("Bold");
    boldCheckbox.setToggleState(valueTreeState.getParameterAsValue("textBold").getValue(), false);
    boldCheckbox.onClick = [this]{ updateToggleState(&boldCheckbox); };

    //italic
    italicCheckbox.setButtonText("Italic");
    italicCheckbox.setToggleState(valueTreeState.getParameterAsValue("textItalic").getValue(), false);
    italicCheckbox.onClick = [this] { updateToggleState(&italicCheckbox); };

    //fullcreen button
    Image image = ImageFileFormat::loadFrom(BinaryData::fullscbutton_png, (size_t)BinaryData::fullscbutton_pngSize);
    fullScreenButton.setImages(true, true, true,
        image, 1.0f, Colours::transparentWhite,
        image, 0.6f, Colours::transparentWhite,
        image, 0.3f, Colours::transparentWhite,
        0.0f);
    fullScreenButton.onClick = [this] { switchToFullScreen(); };

    addAndMakeVisible(fontSizeSelector);
    addAndMakeVisible(fontTypeSelector);
    addAndMakeVisible(textColorButton);
    addAndMakeVisible(backgroundColorButton);
    addAndMakeVisible(justificationSelector);
    addAndMakeVisible(boldCheckbox);
    addAndMakeVisible(italicCheckbox);
    addAndMakeVisible(fullScreenButton);
}

//==============================================================================
AnimatedTextComponent::~AnimatedTextComponent()
{
}

//==============================================================================
void AnimatedTextComponent::paint (Graphics& g)
{

    g.fillAll (currentBackgroundColour);

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);

    g.setColour (currentTextColour);
    g.setFont (currentFont);
    g.drawText (text.substring(0,counter), getLocalBounds().withTop(30),
        currentJustification, true);
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

    textColorSelector.setBounds(205, 5, 160, 180);
    textColorButton.setBounds(205, 5, 100, 20);

    backgroundColorSelector.setBounds(305, 5, 160, 180);
    backgroundColorButton.setBounds(305, 5, 100, 20);

    justificationSelector.setBounds(405, 5, 100, 20);

    boldCheckbox.setBounds(505, 5, 50, 20);

    italicCheckbox.setBounds(555, 5, 50, 20);

    fullScreenButton.setBounds(getWidth() - 25, 5, 20, 20);
}

//==============================================================================
void AnimatedTextComponent::setText(String txt) 
{
    this->counter = 0;
    this->text = txt;
}

//==============================================================================
void AnimatedTextComponent::setOnFullScreenLambda(std::function<void (bool)> lambda)
{
    onFullScreenLambda = lambda;
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
    else if (comboBoxThatHasChanged == &justificationSelector)
    {
        currentJustification = Justification(JUSTIFICATIONS_ARRAY_FLAGS[justificationSelector.getSelectedItemIndex()]);
    }

    repaint();
}

//==============================================================================
void AnimatedTextComponent::changeListenerCallback(ChangeBroadcaster * source)
{
    if (source == &textColorSelector)
    {
        currentTextColour = textColorSelector.getCurrentColour();
        textColorButton.setColour(TextButton::textColourOffId, currentTextColour);
    }
    else if (source == &backgroundColorSelector)
    {
        currentBackgroundColour = backgroundColorSelector.getCurrentColour();
        backgroundColorButton.setColour(TextButton::textColourOffId, currentBackgroundColour);

    }

    repaint();
}

//==============================================================================
void AnimatedTextComponent::mouseDown(const MouseEvent & event)
{
    if (event.originalComponent == &backgroundColorButton)
    {
        //CallOutBox::launchAsynchronously(&backgroundColorSelector, backgroundColorSelector.getBounds(), this); //TODO subclass CallOutBox and use this

        CallOutBox callOut(backgroundColorSelector, backgroundColorButton.getBounds().expanded(1,2), this);
        callOut.runModalLoop();
    }
    else if (event.originalComponent == &textColorButton)
    {
        CallOutBox callOut(textColorSelector, textColorButton.getBounds().expanded(1, 2), this);
        callOut.runModalLoop();
    }
}

//==============================================================================
void AnimatedTextComponent::syncCurrentFont()
{
    String typefaceName = FONTS[*valueTreeState.getRawParameterValue("fontType") - 1];
    int fontHeight = *valueTreeState.getRawParameterValue("fontSize");
    bool bold = valueTreeState.getParameterAsValue("textBold").getValue();
    bool italic = valueTreeState.getParameterAsValue("textItalic").getValue();

    currentFont.setTypefaceName(typefaceName);
    currentFont.setHeight(fontHeight);
    currentFont.setBold(bold);
    currentFont.setItalic(italic);
}

//==============================================================================
void AnimatedTextComponent::syncJustification()
{
    int index = *valueTreeState.getRawParameterValue("textJustification") - 1;

    currentJustification = Justification(JUSTIFICATIONS_ARRAY_FLAGS[index]);
}

//==============================================================================
void AnimatedTextComponent::updateToggleState(ToggleButton * button)
{
    if (button == &boldCheckbox)
    {
        bool bold = boldCheckbox.getToggleState();
        currentFont.setBold(bold);
        valueTreeState.getParameter("textBold")->setValueNotifyingHost(bold);
    }
    else if (button == &italicCheckbox)
    {
        bool italic = italicCheckbox.getToggleState();
        currentFont.setItalic(italic);
        valueTreeState.getParameter("textItalic")->setValueNotifyingHost(italic);
    }

    repaint();
}

//==============================================================================
void AnimatedTextComponent::switchToFullScreen()
{
    fullscreen = !fullscreen;

    if (onFullScreenLambda)
    {
        onFullScreenLambda(fullscreen);
    }

    fontSizeSelector.setVisible(!fullscreen);
    fontTypeSelector.setVisible(!fullscreen);
    textColorSelector.setVisible(!fullscreen);
    textColorButton.setVisible(!fullscreen);
    backgroundColorSelector.setVisible(!fullscreen);
    backgroundColorButton.setVisible(!fullscreen);
    justificationSelector.setVisible(!fullscreen);
    boldCheckbox.setVisible(!fullscreen);
    italicCheckbox.setVisible(!fullscreen);
}
