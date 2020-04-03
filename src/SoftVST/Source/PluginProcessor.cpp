#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SoftVstAudioProcessor::SoftVstAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, "Parameters", createParameters())
#endif
{
	synth.clearVoices();

	// Polyphonic synthesiser with 5 voices
	for (int i = 0; i < 5; i++)
	{
		synth.addVoice(new SynthVoice());
	}

	synth.clearSounds();
	synth.addSound(new SynthSound());

	DBG("****SoftVST Plugin Loaded****");
}

SoftVstAudioProcessor::~SoftVstAudioProcessor()
{
}

//==============================================================================
const String SoftVstAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SoftVstAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SoftVstAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SoftVstAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SoftVstAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SoftVstAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SoftVstAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SoftVstAudioProcessor::setCurrentProgram (int index)
{
}

const String SoftVstAudioProcessor::getProgramName (int index)
{
    return {};
}

void SoftVstAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void SoftVstAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	ignoreUnused(samplesPerBlock);
	lastSampleRate = sampleRate;
	synth.setCurrentPlaybackSampleRate(lastSampleRate);
}

void SoftVstAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SoftVstAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void SoftVstAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
	for (int i = 0; i < synth.getNumVoices(); i++)
	{
		if ((synthVoice = dynamic_cast<SynthVoice*>(synth.getVoice(i))))
		{
			float* osc = (float*)apvts.getRawParameterValue(OSC_ID);
			synthVoice->setOscType(osc);

			float* attack = (float*)apvts.getRawParameterValue(ATTACK_ID);
			float* decay = (float*)apvts.getRawParameterValue(DECAY_ID);
			float* sustain = (float*)apvts.getRawParameterValue(SUSTAIN_ID);
			float* release = (float*)apvts.getRawParameterValue(RELEASE_ID);
			synthVoice->setADSR(attack, decay, sustain, release);
		}
	}


	buffer.clear();

	synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
bool SoftVstAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* SoftVstAudioProcessor::createEditor()
{
    return new SoftVstAudioProcessorEditor (*this);
}

//==============================================================================
void SoftVstAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SoftVstAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SoftVstAudioProcessor();
}

AudioProcessorValueTreeState::ParameterLayout SoftVstAudioProcessor::createParameters()
{
	std::vector<std::unique_ptr<RangedAudioParameter>> parameters;

	// ADSR parameters 
	// TODO: Look into and fix sustain range
	parameters.push_back(std::make_unique<AudioParameterFloat>(ATTACK_ID, ATTACK_NAME, 0.0f, 5000.0f, 0.5f));
	parameters.push_back(std::make_unique<AudioParameterFloat>(DECAY_ID, DECAY_NAME, 0.0f, 5000.0f, 1000.0f));
	parameters.push_back(std::make_unique<AudioParameterFloat>(SUSTAIN_ID, SUSTAIN_NAME, 0.0f, 5000.0f, 0.1f));
	parameters.push_back(std::make_unique<AudioParameterFloat>(RELEASE_ID, RELEASE_NAME, 0.0f, 5000.0f, 15.0f));

	// Oscillator parameter
	parameters.push_back(std::make_unique<AudioParameterChoice>(OSC_ID, OSC_NAME,
		StringArray("Sine", "Saw", "Square"), 0));


	return { parameters.begin(), parameters.end() };
}
