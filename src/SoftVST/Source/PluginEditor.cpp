#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SoftVstAudioProcessorEditor::SoftVstAudioProcessorEditor (SoftVstAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p), envGUI(p), oscGUI(p), filterGUI(p)
{
    setSize (800, 800);

	gainSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
	gainSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 20);
	gainSlider.setRange(-60.0f, 0.0f, 0.01f);
	gainSlider.setValue(-20.0f);
	addAndMakeVisible(gainSlider);

	addAndMakeVisible(envGUI);
	addAndMakeVisible(oscGUI);
	addAndMakeVisible(filterGUI);

	gainSliderAttachment =
		std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.apvts,
			GAIN_ID, gainSlider);
}

SoftVstAudioProcessorEditor::~SoftVstAudioProcessorEditor()
{
}

//==============================================================================
void SoftVstAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    //g.setColour (Colours::white);
    //g.setFont (15.0f);
    //g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);

	g.fillAll(Colours::black);
}

void SoftVstAudioProcessorEditor::resized()
{
	juce::Rectangle<int> area = getLocalBounds();
	
	const int componentWidth = 400;
	const int componentHeight = 400;

	//envGUI.setBounds(area.removeFromLeft(componentWidth).removeFromTop(componentHeight));
	//oscGUI.setBounds(area.removeFromLeft(componentWidth).removeFromTop(componentHeight));
	filterGUI.setBounds(area.removeFromLeft(componentWidth).removeFromTop(componentHeight));
	gainSlider.setBounds(getWidth() / 2 - 50, getHeight() / 2 - 75, 100, 150);

	//const int componentSize{ 100 };
}
