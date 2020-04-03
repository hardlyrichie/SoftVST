#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
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
	Oscillator oscGUI;

	//Slider attackSlider;
	//Slider decaySlider;
	//Slider sustainSlider;
	//Slider releaseSlider;

	//std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> attackSliderAttachment;
	//std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> decaySliderAttachment;
	//std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> sustainSliderAttachment;
	//std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> releaseSliderAttachment;

    SoftVstAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoftVstAudioProcessorEditor)
};
