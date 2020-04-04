#include <JuceHeader.h>
#include "Gain.h"

//==============================================================================
Gain::Gain(SoftVstAudioProcessor& p) : processor(p)
{
	setSize(400, 400);

	gainSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
	gainSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 20);
	gainSlider.setTextValueSuffix("dB");
	gainSlider.setRange(-60.0f, 0.0f, 0.01f);
	gainSlider.setValue(-3.0f);
	addAndMakeVisible(gainSlider);

	gainLabel.setFont(Font(24.0f, Font::bold));
	gainLabel.setText("Gain Slider", dontSendNotification);
	gainLabel.attachToComponent(&gainSlider, false);
	gainLabel.setJustificationType(Justification::centred);
	addAndMakeVisible(gainLabel);

	gainSliderAttachment =
		std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.apvts,
			GAIN_ID, gainSlider);
}

Gain::~Gain()
{
}

void Gain::paint (Graphics& g)
{
	g.fillAll(Colours::aquamarine);
}

void Gain::resized()
{
	juce::Rectangle<int> area = getLocalBounds().reduced(70);

	gainSlider.setBounds(area);
}
