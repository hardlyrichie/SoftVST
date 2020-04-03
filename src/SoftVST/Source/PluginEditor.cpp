#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SoftVstAudioProcessorEditor::SoftVstAudioProcessorEditor (SoftVstAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p), oscGUI(p)
{
    setSize (400, 200);

	addAndMakeVisible(oscGUI);

	// Configure ADSR slider settings
	/*attackSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	attackSlider.setRange(0.1f, 5000.0f);
	attackSlider.setValue(1.0f);
	attackSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 70, 20);
	addAndMakeVisible(attackSlider);

	decaySlider.setSliderStyle(Slider::SliderStyle::Rotary);
	decaySlider.setRange(0.1f, 5000.0f);
	decaySlider.setValue(1.0f);
	decaySlider.setTextBoxStyle(Slider::TextBoxBelow, true, 70, 20);
	addAndMakeVisible(attackSlider);

	sustainSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	sustainSlider.setRange(0.1f, 5000.0f);
	sustainSlider.setValue(1.0f);
	sustainSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 70, 20);
	addAndMakeVisible(attackSlider);

	releaseSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	releaseSlider.setRange(0.1f, 5000.0f);
	releaseSlider.setValue(1.0f);
	releaseSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 70, 20);
	addAndMakeVisible(attackSlider);*/

	//// Setup apvts attachments
	//attackSliderAttachment = 
	//	std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.apvts, 
	//	ATTACK_ID, attackSlider);
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
	
	const int componentWidth = 200;
	const int componentHeight = 200;

	oscGUI.setBounds(area.removeFromLeft(componentWidth).removeFromTop(componentHeight));

	//const int componentSize{ 100 };

	//attackSlider.setBounds(bounds.removeFromTop(200).withSizeKeepingCentre(componentSize, componentSize));
}
