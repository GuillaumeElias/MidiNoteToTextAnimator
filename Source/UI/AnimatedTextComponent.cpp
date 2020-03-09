
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
    , fullscreenIcon(ImageFileFormat::loadFrom(BinaryData::fullscbutton_png, (size_t)BinaryData::fullscbutton_pngSize))
    , fullscreenIconBack(ImageFileFormat::loadFrom(BinaryData::fullscbuttonback_png, (size_t)BinaryData::fullscbuttonback_pngSize))
{
    //fetch parameters values and setup listeners
    mode = static_cast<int>(*valueTreeState.getRawParameterValue("mode"));
    valueTreeState.addParameterListener("mode", this);

    skipSpaces = valueTreeState.getParameterAsValue("skipSpaces").getValue();
    valueTreeState.addParameterListener("skipSpaces", this);

    autoRestart = valueTreeState.getParameterAsValue("autoRestart").getValue();
    valueTreeState.addParameterListener("autoRestart", this);

    int speed = static_cast<int>(*valueTreeState.getRawParameterValue("fixedSpeed"));
    valueTreeState.addParameterListener("fixedSpeed", this);
    setFramesPerSecond(speed);

    //sync font and justification
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
    textColorButton.addMouseListener(static_cast<MouseListener*>(this), true);

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

    //restart button
    Image restartImage = ImageFileFormat::loadFrom(BinaryData::restartbutton_png, (size_t)BinaryData::restartbutton_pngSize);
    restartButton.setImages(true, true, true,
        restartImage, 1.0f, Colours::transparentWhite,
        restartImage, 0.6f, Colours::transparentWhite,
        restartImage, 0.3f, Colours::transparentWhite,
        0.0f);
    restartButton.onClick = [this] { counter = 0; };

    //fullcreen button
    setFullscreenButtonIcon(fullscreenIcon);
    fullScreenButton.onClick = [this] { switchToFullScreen(); };

    addAndMakeVisible(fontSizeSelector);
    addAndMakeVisible(fontTypeSelector);
    addAndMakeVisible(textColorButton);
    addAndMakeVisible(backgroundColorButton);
    addAndMakeVisible(justificationSelector);
    addAndMakeVisible(boldCheckbox);
    addAndMakeVisible(italicCheckbox);
    addAndMakeVisible(restartButton);
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

    restartButton.setBounds(getWidth() - 50, 5, 20, 20);

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

        if (skipSpaces && counter < text.length() && text[counter] == ' ') //skip space
        {
            counter++;
        }
    }

    if (autoRestart && counter >= text.length()) //restart if at end
    {
        counter = 0;
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
    else if (parameterID == "skipSpaces")
    {
        skipSpaces = newValue;
    }
    else if (parameterID == "autoRestart")
    {
        autoRestart = newValue;
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
        fullScreenButton.setOpaque(currentBackgroundColour.getBrightness() > 0.95); //quick workaround to show the button if background is white
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

    if (fullscreen)
    {
        setFullscreenButtonIcon(fullscreenIconBack);
    }
    else
    {
        setFullscreenButtonIcon(fullscreenIcon);
    }
}

//==============================================================================
void AnimatedTextComponent::setFullscreenButtonIcon(const Image & icon)
{
    Rectangle<int> oldBounds = fullScreenButton.getBounds();

    fullScreenButton.setImages(true, true, true,
        icon, 1.0f, Colours::transparentWhite,
        icon, 0.6f, Colours::transparentWhite,
        icon, 0.3f, Colours::transparentWhite,
        0.0f);

    fullScreenButton.setBounds(oldBounds); //need to reset the bounds to what they were before changing the image otherwise it will adapt to the new size
}
