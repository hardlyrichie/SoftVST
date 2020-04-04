#include <JuceHeader.h>
#include "Filter.h"

//==============================================================================
Filter::Filter(SoftVstAudioProcessor& p) : processor(p)
{
	setSize(400, 400);

	// Configure combobox settings
	filterMenu.addItem("Low-pass", 1);
	filterMenu.addItem("High-pass", 2);
	filterMenu.addItem("Band-pass", 3);
	filterMenu.setJustificationType(Justification::centred);
	addAndMakeVisible(filterMenu);

	// Configure slider settings
	freqSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	freqSlider.setRange(20.0f, 10000.0f);
	freqSlider.setValue(400.0f);
	freqSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 70, 20);
	// TODO: Fix skewfactor
	freqSlider.setSkewFactorFromMidPoint(1000.0);
	addAndMakeVisible(freqSlider);

	resSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	resSlider.setRange(1.0f, 5.0f);
	resSlider.setValue(1.0f);
	resSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 70, 20);
	addAndMakeVisible(resSlider);

	// Setup apvts attachments
	filterMenuAttachment =
		std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment>(processor.apvts,
			FILTER_ID, filterMenu);
	freqSliderAttachment =
		std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.apvts,
			FREQ_ID, freqSlider);
	resSliderAttachment =
		std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.apvts,
			RES_ID, resSlider);
}

Filter::~Filter()
{
}

void Filter::paint (Graphics& g)
{
	g.fillAll(Colours::black);
}

void Filter::resized()
{
	juce::Rectangle<int> area = getLocalBounds().reduced(40);

	// TODO: Fix layout
	const int componentSize = 70;

	filterMenu.setBounds(area.removeFromTop(20));
	freqSlider.setBounds(area.removeFromTop(50).withSizeKeepingCentre(componentSize, componentSize));
	resSlider.setBounds(area.removeFromTop(50).withSizeKeepingCentre(componentSize, componentSize));
}
