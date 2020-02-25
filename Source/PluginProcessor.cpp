#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MidiNoteToTextAudioProcessor::MidiNoteToTextAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
    , parameters(*this, nullptr, Identifier("MidiNoteToText"),
        {
            std::make_unique<AudioParameterInt>("mode",            // parameter ID
                                                   "Mode",            // parameter name
                                                   1,              // minimum value
                                                   2,              // maximum value
                                                   1),             // default value
            std::make_unique<AudioParameterInt>("fixedSpeed",      // parameter ID
                                                  "FixedSpeed",     // parameter name
                                                   0,              // minimum value
                                                   100,              // maximum value
                                                   10)             // default value
        })
#endif
{
}

//==============================================================================
MidiNoteToTextAudioProcessor::~MidiNoteToTextAudioProcessor()
{
}

//==============================================================================
const String MidiNoteToTextAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

//==============================================================================
bool MidiNoteToTextAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

//==============================================================================
bool MidiNoteToTextAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

//==============================================================================
bool MidiNoteToTextAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

//==============================================================================
double MidiNoteToTextAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

//==============================================================================
int MidiNoteToTextAudioProcessor::getNumPrograms()
{
    return 1;
}

//==============================================================================
int MidiNoteToTextAudioProcessor::getCurrentProgram()
{
    return 0;
}

//==============================================================================
void MidiNoteToTextAudioProcessor::setCurrentProgram (int index)
{
}

//==============================================================================
const String MidiNoteToTextAudioProcessor::getProgramName (int index)
{
    return {};
}

//==============================================================================
void MidiNoteToTextAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void MidiNoteToTextAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    Logger::outputDebugString("prepareToPlay");

    reset();
}

//==============================================================================
void MidiNoteToTextAudioProcessor::releaseResources()
{
}

//==============================================================================
#ifndef JucePlugin_PreferredChannelConfigurations
bool MidiNoteToTextAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

//==============================================================================
void MidiNoteToTextAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{

    midiReceiver.handleMidiMessage(midiMessages);

    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

  
    //nothing to do for audio
}

//==============================================================================
bool MidiNoteToTextAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

//==============================================================================
AudioProcessorEditor* MidiNoteToTextAudioProcessor::createEditor()
{
    MidiNoteToTextAudioProcessorEditor * editor = new MidiNoteToTextAudioProcessorEditor(*this, parameters);

    //initialize and link updaters to components
    midiReceiver.setAnimatedTextUpdater(std::make_unique<AnimatedTextUpdater>(editor->getAnimatedTextComponent()));

    return editor;
}

//==============================================================================
void MidiNoteToTextAudioProcessor::getStateInformation(MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

//==============================================================================
void MidiNoteToTextAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(ValueTree::fromXml(*xmlState));
}


//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MidiNoteToTextAudioProcessor();
}