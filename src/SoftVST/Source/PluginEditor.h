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
class SoftVstAudioProcessorEditor  : public AudioProcessorEditor,
	public Slider::Listener
{
public:
    SoftVstAudioProcessorEditor (SoftVstAudioProcessor&);
    ~SoftVstAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

	void sliderValueChanged(Slider* slider) override;

private:
	Slider attackSlider;

    SoftVstAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoftVstAudioProcessorEditor)
};
