#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

#define ATTACK_ID "ATTACK"
#define ATTACK_NAME "Attack"
#define DECAY_ID "DECAY"
#define DECAY_NAME "Decay"
#define SUSTAIN_ID "SUSTAIN"
#define SUSTAIN_NAME "Sustain"
#define RELEASE_ID "RELEASE"
#define RELEASE_NAME "Release"

//==============================================================================
/*
*/
class Envelope : public Component
{
public:
    Envelope(SoftVstAudioProcessor&);
    ~Envelope();

    void paint (Graphics&) override;
    void resized() override;

private:
	Slider attackSlider;
	Slider decaySlider;
	Slider sustainSlider;
	Slider releaseSlider;

	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> attackSliderAttachment;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> decaySliderAttachment;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> sustainSliderAttachment;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> releaseSliderAttachment;

	SoftVstAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Envelope)
};
