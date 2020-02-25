#include "../JuceLibraryCode/JuceHeader.h"
#include "AnimatedTextUpdater.h"
#include "AnimatedTextComponent.h"

//=============================================================================
AnimatedTextUpdater::AnimatedTextUpdater(AnimatedTextComponent * component)
    : textComponent(component)
{

}

//=============================================================================
void AnimatedTextUpdater::handleAsyncUpdate()
{
    textComponent->onMidiNoteIn();
}
