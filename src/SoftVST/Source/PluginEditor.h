/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

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
	Slider attackSlider;

	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> attackSliderAttachment;

    SoftVstAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoftVstAudioProcessorEditor)
};
