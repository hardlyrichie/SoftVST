#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SoftVstAudioProcessorEditor::SoftVstAudioProcessorEditor (SoftVstAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p), envGUI(p), oscGUI(p), filterGUI(p)
{
    setSize (800, 800);

	addAndMakeVisible(envGUI);
	addAndMakeVisible(oscGUI);
	addAndMakeVisible(filterGUI);
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

	//const int componentSize{ 100 };
}
