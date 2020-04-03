/*
  ==============================================================================

    Envelope.cpp
    Created: 3 Apr 2020 1:27:08am
    Author:  rgao

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Envelope.h"

//==============================================================================
Envelope::Envelope(SoftVstAudioProcessor& p) : processor(p)
{
	setSize(400, 400);

	// Configure ADSR slider settings
	attackSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	attackSlider.setRange(0.1f, 5000.0f);
	attackSlider.setValue(1.0f);
	attackSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 70, 20);
	addAndMakeVisible(attackSlider);

	decaySlider.setSliderStyle(Slider::SliderStyle::Rotary);
	decaySlider.setRange(0.1f, 5000.0f);
	decaySlider.setValue(1.0f);
	decaySlider.setTextBoxStyle(Slider::TextBoxBelow, true, 70, 20);
	addAndMakeVisible(decaySlider);

	sustainSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	sustainSlider.setRange(0.1f, 5000.0f);
	sustainSlider.setValue(1.0f);
	sustainSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 70, 20);
	addAndMakeVisible(sustainSlider);

	releaseSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	releaseSlider.setRange(0.1f, 5000.0f);
	releaseSlider.setValue(1.0f);
	releaseSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 70, 20);
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
	g.fillAll(Colours::black);
}

void Envelope::resized()
{
	juce::Rectangle<int> area = getLocalBounds().reduced(40);

	// TODO: Fix layout
	const int componentSize = 70;

	attackSlider.setBounds(area.removeFromTop(50).withSizeKeepingCentre(componentSize, componentSize));
	decaySlider.setBounds(area.removeFromTop(50).withSizeKeepingCentre(componentSize, componentSize));
	sustainSlider.setBounds(area.removeFromTop(50).withSizeKeepingCentre(componentSize, componentSize));
	releaseSlider.setBounds(area.removeFromTop(50).withSizeKeepingCentre(componentSize, componentSize));
}
