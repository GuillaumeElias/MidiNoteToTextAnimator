#include "PluginLookAndFeel.h"

const int BORDER = 4;
const int TWO_BORDERS = 2 * BORDER;
const Typeface::Ptr COURIER_TYPEFACE = Typeface::createSystemTypefaceFor(BinaryData::Courier__Prime_Sans_ttf, BinaryData::Courier__Prime_Sans_ttfSize);
const Colour CHECKBOX_BACKGROUND_COLOUR = Colour(38, 50, 56);

//================================================================================
PluginLookAndFeel::PluginLookAndFeel()
{
    LookAndFeel::getDefaultLookAndFeel().setDefaultSansSerifTypeface(COURIER_TYPEFACE);
    setColour(Slider::thumbColourId, Colours::white);
    setColour(Label::textColourId, Colours::white);
}

//================================================================================
void PluginLookAndFeel::drawTickBox(Graphics& g, Component& component,
    float x, float y, float w, float h,
    const bool ticked,
    const bool isEnabled,
    const bool shouldDrawButtonAsHighlighted,
    const bool shouldDrawButtonAsDown)
{
    g.setColour(CHECKBOX_BACKGROUND_COLOUR);
    g.fillRect(x, y, w, h);

    if (ticked)
    {
        g.setColour(Colours::white);
        g.fillRect(x + BORDER, y + BORDER, w - TWO_BORDERS, h - TWO_BORDERS);
    }
}