#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

#define FILTER_ID "FILTER"
#define FILTER_NAME "Filter"
#define FREQ_ID "FREQ"
#define FREQ_NAME "Freq"
#define RES_ID "RES"
#define RES_NAME "Res"

//==============================================================================
/*
*/
class Filter : public Component
{
public:
    Filter(SoftVstAudioProcessor&);
    ~Filter();

    void paint (Graphics&) override;
    void resized() override;

private:
	ComboBox filterMenu;
	Slider freqSlider;
	Slider resSlider;
	Label filterLabel;
	Label freqLabel;
	Label resLabel;

	std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> filterMenuAttachment;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> freqSliderAttachment;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> resSliderAttachment;

	SoftVstAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Filter)
};
