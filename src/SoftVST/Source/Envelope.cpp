#include <JuceHeader.h>
#include "Envelope.h"

//==============================================================================
Envelope::Envelope(SoftVstAudioProcessor& p) : processor(p)
{
	setSize(400, 400);

	// Configure ADSR slider settings
	attackSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	attackSlider.setRange(0.0f, 5000.0f);
	attackSlider.setValue(1.0f);
	attackSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 70, 20);
	attackSlider.setTextValueSuffix("ms");
	addAndMakeVisible(attackSlider);

	decaySlider.setSliderStyle(Slider::SliderStyle::Rotary);
	decaySlider.setRange(0.0f, 5000.0f);
	decaySlider.setValue(600.0f);
	decaySlider.setTextBoxStyle(Slider::TextBoxBelow, true, 70, 20);
	decaySlider.setTextValueSuffix("ms");
	addAndMakeVisible(decaySlider);

	sustainSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	sustainSlider.setRange(-100.0f, 0.0f);
	sustainSlider.setValue(-6.0f);
	sustainSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 70, 20);
	sustainSlider.setTextValueSuffix("dB");
	// TODO: Fix skewfactor. Doesn't seem to be working
	sustainSlider.setSkewFactorFromMidPoint(-12.0);
	addAndMakeVisible(sustainSlider);

	releaseSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	releaseSlider.setRange(0.0f, 5000.0f);
	releaseSlider.setValue(600.0f);
	releaseSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 70, 20);
	releaseSlider.setTextValueSuffix("ms");
	addAndMakeVisible(releaseSlider);

	// Setup apvts attachments
	attackSliderAttachment =
		std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.apvts,
			ATTACK_ID, attackSlider);
	decaySliderAttachment =
		std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.apvts,
			DECAY_ID, decaySlider);
	sustainSliderAttachment =
		std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.apvts,
			SUSTAIN_ID, sustainSlider);
	releaseSliderAttachment =
		std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.apvts,
			RELEASE_ID, releaseSlider);
}

Envelope::~Envelope()
{
}

void Envelope::paint (Graphics& g)
{
	g.fillAll(Colours::rebeccapurple);

	g.setFont(Font(18.0f, Font::bold));
	g.setColour(Colours::white);

	g.drawText("A", 50 - 10, 30, 20, 20, Justification::centredTop);
	g.drawText("D", 150 - 10, 30, 20, 20, Justification::centredTop);
	g.drawText("S", 250 - 10, 30, 20, 20, Justification::centredTop);
	g.drawText("R", 350 - 10, 30, 20, 20, Justification::centredTop);
}

void Envelope::resized()
{
	juce::Rectangle<int> area = getLocalBounds();

	const int sliderWidth = 100;
	const int sliderHeight = 200;

	attackSlider.setBounds(area.removeFromLeft(sliderWidth).removeFromTop(sliderHeight).withTrimmedTop(10));
	decaySlider.setBounds(area.removeFromLeft(sliderWidth).removeFromTop(sliderHeight).withTrimmedTop(10));
	sustainSlider.setBounds(area.removeFromLeft(sliderWidth).removeFromTop(sliderHeight).withTrimmedTop(10));
	releaseSlider.setBounds(area.removeFromLeft(sliderWidth).removeFromTop(sliderHeight).withTrimmedTop(10));
}
