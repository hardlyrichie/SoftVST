#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Envelope.h"
#include "Oscillator.h"

//==============================================================================
/**
*/
class SoftVstAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    SoftVstAudioProcessorEditor (SoftVstAudioProcessor&);
    ~SoftVstAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
	Envelope envGUI;
	Oscillator oscGUI;

    SoftVstAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoftVstAudioProcessorEditor)
};
