#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class AnimatedTextComponent;

class AnimatedTextUpdater : public AsyncUpdater
{
public:
    AnimatedTextUpdater(AnimatedTextComponent * textComponent);
    void handleAsyncUpdate() override;
private:
    AnimatedTextComponent * textComponent;

};