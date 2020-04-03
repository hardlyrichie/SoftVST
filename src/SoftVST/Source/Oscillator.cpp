/*
  ==============================================================================

    Oscillator.cpp
    Created: 2 Apr 2020 11:58:19pm
    Author:  rgao

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Oscillator.h"

//==============================================================================
Oscillator::Oscillator(SoftVstAudioProcessor& p) : processor(p)
{
	setSize(400, 400);

	oscMenu.addItem("Sine", 1);
	oscMenu.addItem("Saw", 2);
	oscMenu.addItem("Square", 3);
	oscMenu.setJustificationType(Justification::centred); 
	addAndMakeVisible(oscMenu);

	oscMenuAttachment = 
		std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment>(processor.apvts, 
		OSC_ID, oscMenu);

}

Oscillator::~Oscillator()
{
}

void Oscillator::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    //g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    //g.setColour (Colours::grey);
    //g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    //g.setColour (Colours::white);
    //g.setFont (14.0f);
    //g.drawText ("Oscillator", getLocalBounds(),
    //            Justification::centred, true);   // draw some placeholder text
	g.fillAll(Colours::black);
}

void Oscillator::resized()
{
	juce::Rectangle<int> area = getLocalBounds().reduced(40);

	oscMenu.setBounds(area.removeFromTop(20));
}
