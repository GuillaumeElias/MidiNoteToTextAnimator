#include "../JuceLibraryCode/JuceHeader.h"
#include "ComponentMidiUpdater.h"

//=============================================================================
ComponentMidiUpdater::ComponentMidiUpdater(UpdatableComponent * c)
    : component(c)
{

}

//=============================================================================
void ComponentMidiUpdater::handleAsyncUpdate()
{
    component->onMidiNoteIn();
}
