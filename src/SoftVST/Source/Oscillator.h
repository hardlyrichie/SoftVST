#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

#define OSC_ID "OSC"
#define OSC_NAME "Osc"

//==============================================================================
/*
*/
class Oscillator : public Component
{
public:
    Oscillator(SoftVstAudioProcessor&);
    ~Oscillator();

    void paint (Graphics&) override;
    void resized() override;

private:
	ComboBox oscMenu;

	std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> oscMenuAttachment;

	SoftVstAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Oscillator)
};
