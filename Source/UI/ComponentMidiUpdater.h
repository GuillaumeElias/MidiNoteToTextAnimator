#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "UpdatableComponent.h"

class ComponentMidiUpdater : public AsyncUpdater
{
public:
    ComponentMidiUpdater(UpdatableComponent * textComponent);
    void handleAsyncUpdate() override;
private:
    UpdatableComponent * component;

};