/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SoftVstAudioProcessorEditor::SoftVstAudioProcessorEditor (SoftVstAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);

	// Configure ADSR slider settings
	attackSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	attackSlider.setRange(0.1f, 5000.0f);
	attackSlider.setValue(0.1f);
	//attackSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 20.0, 10.0);
	addAndMakeVisible(attackSlider);

	// Setup apvts attachments
	attackSliderAttachment = 
		std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.apvts, 
		ATTACK_ID, attackSlider);
}

SoftVstAudioProcessorEditor::~SoftVstAudioProcessorEditor()
{
}

//==============================================================================
void SoftVstAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
}

void SoftVstAudioProcessorEditor::resized()
{
	auto bounds = getLocalBounds();
	const int componentSize{ 150 };

	attackSlider.setBounds(bounds.removeFromTop(200).withSizeKeepingCentre(componentSize, componentSize));
}
