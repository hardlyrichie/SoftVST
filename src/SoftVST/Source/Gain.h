#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

#define GAIN_ID "GAIN"
#define GAIN_NAME "Gain"

//==============================================================================
/*
*/
class Gain : public Component
{
public:
    Gain(SoftVstAudioProcessor&);
    ~Gain();

    void paint (Graphics&) override;
    void resized() override;

private:
	Slider gainSlider;
	Label gainLabel;

	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> gainSliderAttachment;

	SoftVstAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Gain)
};
