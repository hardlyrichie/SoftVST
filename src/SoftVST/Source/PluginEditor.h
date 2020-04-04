#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Envelope.h"
#include "Oscillator.h"
#include "Filter.h"

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
	Filter filterGUI;
	Slider gainSlider;

	Label title;
	Label gainLabel;

	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> gainSliderAttachment;

    SoftVstAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoftVstAudioProcessorEditor)
};
