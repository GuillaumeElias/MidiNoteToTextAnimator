#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class PluginLookAndFeel : public LookAndFeel_V4
{
public:
    PluginLookAndFeel();

    void drawTickBox(Graphics& g, Component& component,
        float x, float y, float w, float h,
        const bool ticked,
        const bool isEnabled,
        const bool shouldDrawButtonAsHighlighted,
        const bool shouldDrawButtonAsDown) override;
};