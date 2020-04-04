#pragma once

#include <JuceHeader.h>
#include "SynthSound.h"
#include "SynthVoice.h"

#define ATTACK_ID "ATTACK"
#define ATTACK_NAME "Attack"
#define DECAY_ID "DECAY"
#define DECAY_NAME "Decay"
#define SUSTAIN_ID "SUSTAIN"
#define SUSTAIN_NAME "Sustain"
#define RELEASE_ID "RELEASE"
#define RELEASE_NAME "Release"
#define OSC_ID "OSC"
#define OSC_NAME "Osc"
#define FILTER_ID "FILTER"
#define FILTER_NAME "Filter"
#define FREQ_ID "FREQ"
#define FREQ_NAME "Freq"
#define RES_ID "RES"
#define RES_NAME "Res"
#define GAIN_ID "GAIN"
#define GAIN_NAME "Gain"

//==============================================================================
/**
*/
class SoftVstAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    SoftVstAudioProcessor();
    ~SoftVstAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

	AudioProcessorValueTreeState apvts;
	AudioProcessorValueTreeState::ParameterLayout createParameters();

private:
	Synthesiser synth;
	SynthVoice* synthVoice;

	double lastSampleRate;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoftVstAudioProcessor)
};
